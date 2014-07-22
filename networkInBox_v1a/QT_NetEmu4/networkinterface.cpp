#include "networkinterface.h"
#include "buffer.h"
#include "CollectedStatistics.h"

#include <QDebug>
#include <QNetworkInterface>

#include "PpiHeader.h"



PPI_PACKET_HEADER *radio_header;
FILE *fpData;
extern "C" PAirpcapHandle pcap_get_airpcap_handle(pcap_t *p);
u_char TxPacket_tst[PPI802_HEADER_SIZE];

QString NetworkEmulator::getInterfaceHardwareAddress(QString pcapName)
{
    QString interfaceName = pcapName.section('_',1,1);
    QNetworkInterface netInterface =  QNetworkInterface::interfaceFromName(interfaceName);
    if ( netInterface.isValid() )
        return netInterface.hardwareAddress();
    else
        return "";
}

NetworkEmulator::NetworkEmulator(QObject *parent) :
    QObject(parent)
{
    // file logging
    fpData = fopen("rcvData.txt", "a+");
    lookupAdapterInterfaces();
    buffer1 = new Buffer(BUFFER_SIZE);
    buffer2 = new Buffer(BUFFER_SIZE);
    connect(&monitorThread,SIGNAL(statisticsCollected(CollectedStatistics*)),this,SLOT(receiveStatistics(CollectedStatistics*)));

}

NetworkEmulator::~NetworkEmulator()
{
    Stop();
}

void NetworkEmulator::Stop()
{
    monitorThread.stop();

    readerThread1.stop();
    readerThread2.stop();

    buffer1->reset();
    buffer2->reset();
    fclose(fpData);

//    writerThread1.stop();
//    writerThread2.stop();
}

void NetworkEmulator::receiveStatistics(CollectedStatistics* stats)
{
    emit statisticsCollected(stats);
}

void NetworkEmulator::lookupAdapterInterfaces()
{
    pcap_if_t *d;
    pcap_if_t *alldevs;
    u_int i = 0;

    char errbuf[PCAP_ERRBUF_SIZE];

    //An // Create a list of network devices that can be opened with pcap_open()
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        qDebug() << "Error in pcap_findalldevs_ex: " << errbuf  ;
        return;
    }
    //An // Print the device list
    for(d= alldevs, i=0; d != NULL; d= d->next, i++)
    {
        InterfaceData data;
        data.interfaceName = d->name;
        data.hardwareAddress = getInterfaceHardwareAddress(d->name);
        interfaceDataList.append(data);

        emit addAdapterInterface(d->description);
    }

    if (i == 0)
    {
        qDebug() << "No interfaces found! Make sure WinPcap is installed.";
        return;
    }

    pcap_freealldevs(alldevs);
}

void NetworkEmulator::setLossrate(int lossRate1, int lossRate2)
{
    readerThread1.setLossRate(lossRate1);
    readerThread2.setLossRate(lossRate2);
}

void NetworkEmulator::setBandwidth(double bandwidth1,double bandwidth2)
{
    writerThread1.setBandwidthLimit(bandwidth1);
    writerThread2.setBandwidthLimit(bandwidth2);
}

void NetworkEmulator::setMacFilters(QString filterInterfaceOne, QString filterInterfaceTwo )
{
    QString interface1MAC = interfaceDataList[selectedInterfaceIndex1].hardwareAddress;
    QString interface2MAC = interfaceDataList[selectedInterfaceIndex2].hardwareAddress;
    // Filter out packets genereated from the selected interfaces
   /* QString baseFilter1 = QString("(ether src %1) and (not ether dst %2)").arg(interface1MAC).arg(interface2MAC);
    if ( filterInterfaceOne != "")
        baseFilter1 = baseFilter1 + QString(" and (%1)").arg(filterInterfaceOne);
*/
    QString baseFilter1 = QString("(src port 39082)").arg(interface1MAC).arg(interface2MAC);
        /*if ( filterInterfaceOne != "")
            baseFilter1 = baseFilter1 + QString(" and (%1)").arg(filterInterfaceOne);*/
    /*QString baseFilter2 = QString("(src port 39082)").arg(interface2MAC).arg(interface1MAC);
    if ( filterInterfaceTwo != "")
        baseFilter2 = baseFilter2 + QString(" and (%1)").arg(filterInterfaceTwo);
        */

    setupWinPcapFilter(&pAdapterOne, baseFilter1);
//    setupWinPcapFilter(&pAdapterTwo, baseFilter2);
}

void NetworkEmulator::setSelectedInterfaces( int interface1, int interface2)
{
    //An
    AirpcapMacAddress MacAddress;
    double testtxrates = 0;
    int ratesend = 9;
    AirpcapChannelInfo 	ChannelInfo;

    selectedInterfaceIndex1 = interface1;
    selectedInterfaceIndex2 = interface2;

    QByteArray ba1 = interfaceDataList[interface1].interfaceName.toLatin1();
    QByteArray ba2 = interfaceDataList[interface2].interfaceName.toLatin1();

    // temporarily disable the online interface and enable offline file read
    //  const char *interfaceOne = ba1.data();
	const char *interfaceOne = "file://c:/temp/h264dump1.pcap";
    const char *interfaceTwo = ba2.data();

    char errbuf[PCAP_ERRBUF_SIZE];
    u_int32_t freq_chan = 11;

    if ( (pAdapterOne= pcap_open(interfaceOne,          	// name of the device
                                  PACKET_CAPTURE_SIZE,  	// portion of the packet to capture
                                                                // 65536 guarantees that the whole packet will be captured on all the link layers
                                  PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode
                                  100,  			// read timeout
                                  NULL,             		// authentication on the remote machine
                                  errbuf    			// error buffer
                                  ) ) == NULL)
    {
        qDebug() << "Unable to open the adapter." << interfaceDataList[interface1].interfaceName << " is not supported by WinPcap";
    }

    if ( (pAdapterTwo= pcap_open(interfaceTwo,          	// name of the device
                                  PACKET_CAPTURE_SIZE,  	// portion of the packet to capture
                                                                // 65536 guarantees that the whole packet will be captured on all the link layers
                                  PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode
                                  100,  			// read timeout
                                  NULL,             		// authentication on the remote machine
                                  errbuf    			// error buffer
                                  ) ) == NULL)
    {
        qDebug() << "Unable to open the adapter." << interfaceDataList[interface2].interfaceName << " is not supported by WinPcap";
    }

    //An
    // Returns the AirPcap handler associated with an adapter.
    // This handler can be used to change the wireless-related settings of the CACE Technologies AirPcap wireless capture adapters.
    airpcap_handle = (PAirpcapHandle)pcap_get_airpcap_handle(pAdapterTwo);

    if(airpcap_handle == NULL)
    {
         printf("Problem in opening Aipcap handler");
         pcap_close(pAdapterTwo);
         return;
    }

    ChannelInfo.Frequency = 2462;
    ChannelInfo.ExtChannel = 1;
    ChannelInfo.Flags = AIRPCAP_CIF_TX_ENABLED;

    if (!AirpcapSetDeviceChannelEx(airpcap_handle, ChannelInfo))
    {
        printf("Error in Setting the Channel Ext : %s", AirpcapGetLastError(airpcap_handle));
        fprintf(fpData, "\n13: Error in Setting the Channel Ext: %s", AirpcapGetLastError(airpcap_handle));
        return;
    }

     //An
     //
     // Set the link layer to 802.11 plus ppi headers
     //
     if(!AirpcapSetLinkType(airpcap_handle, AIRPCAP_LT_802_11_PLUS_PPI))
     {
         printf("Error setting the link layer: %s\n", AirpcapGetLastError(airpcap_handle));
         AirpcapClose(airpcap_handle);
         return;
     }
     //An
     // Get the MAC address
     //
     if (!AirpcapGetMacAddress(airpcap_handle, &MacAddress))
     {
         printf("Error retrieving the MAC address: %s\n", AirpcapGetLastError(airpcap_handle));
         fprintf(fpData, "\n15: Error retrieving the MAC address: %s\n", AirpcapGetLastError(airpcap_handle));
         return;
     }

     radio_header = (PPI_PACKET_HEADER*)TxPacket_tst;
     radio_header->PphLength = sizeof(PPI_PACKET_HEADER);
     radio_header->PphDlt = 105;
     radio_header->PfhType = PPI_PFHTYPE_80211NMACPHY;
     radio_header->PfhLength = PPI_PFHTYPE_80211NMACPHY_SIZE;
     radio_header->MCS = (UCHAR)(testtxrates);

     radio_header->NumStreams = 1;

     //Frame Control
     TxPacket_tst[radio_header->PphLength] = 8;
     TxPacket_tst[radio_header->PphLength+1] = 218;
     //Duration / ID
     TxPacket_tst[radio_header->PphLength+2] = 186;
     TxPacket_tst[radio_header->PphLength+3] = 113;

     //Dest Address - 00:80:48:6F:25:02
     TxPacket_tst[radio_header->PphLength+4] = 0;
     TxPacket_tst[radio_header->PphLength+5] = 128;
     TxPacket_tst[radio_header->PphLength+6] = 72;
     TxPacket_tst[radio_header->PphLength+7] = 111;
     TxPacket_tst[radio_header->PphLength+8] = 35;
     TxPacket_tst[radio_header->PphLength+9] = 05;

     //Source Address - 00:80:48:6F:23:05
     TxPacket_tst[radio_header->PphLength + 10] = MacAddress.Address[0];
     TxPacket_tst[radio_header->PphLength + 11] = MacAddress.Address[1];
     TxPacket_tst[radio_header->PphLength + 12] = MacAddress.Address[2];
     TxPacket_tst[radio_header->PphLength + 13] = MacAddress.Address[3];
     TxPacket_tst[radio_header->PphLength + 14] = MacAddress.Address[4];
     TxPacket_tst[radio_header->PphLength + 15] = MacAddress.Address[5];

     //BSS ID
     TxPacket_tst[radio_header->PphLength + 16] = MacAddress.Address[0];
     TxPacket_tst[radio_header->PphLength + 17] = MacAddress.Address[1];
     TxPacket_tst[radio_header->PphLength + 18] = MacAddress.Address[2];
     TxPacket_tst[radio_header->PphLength + 19] = MacAddress.Address[3];
     TxPacket_tst[radio_header->PphLength + 20] = MacAddress.Address[4];
     TxPacket_tst[radio_header->PphLength + 21] = MacAddress.Address[5];

     TxPacket_tst[radio_header->PphLength+24] = 106;	//6A	// TBD analyze these header fields
     TxPacket_tst[radio_header->PphLength+25] = 10;	//A
     TxPacket_tst[radio_header->PphLength+26] = 218;	//DA
     TxPacket_tst[radio_header->PphLength+27] = 154;	//9A
     TxPacket_tst[radio_header->PphLength+28] = ratesend;	//1
     TxPacket_tst[radio_header->PphLength+29] = 1;	//1
     TxPacket_tst[radio_header->PphLength+30] = 1;	//1

}

void NetworkEmulator::startBridge()
{
    readerThread1.readFromAdapter(1,pAdapterOne,buffer1);
    writerThread1.writeToAdapter(1,pAdapterTwo,buffer1);

 //   readerThread2.readFromAdapter(2,pAdapterTwo,buffer2);
//    writerThread2.writeToAdapter(2,pAdapterOne,buffer2);

 //   monitorThread.pollValues(&readerThread1,&readerThread2,1000);
}

int NetworkEmulator::setupWinPcapFilter(pcap_t** fp, QString macFilter)
{
    QByteArray ba = macFilter.toLatin1();
    const char *filter = ba.data();

    // Filtering variables
    bpf_u_int32 NetMask;
    struct bpf_program fcode;
    NetMask=0xffffff;

    qDebug() << "wpcap Filter is: " << filter ;

    if(pcap_compile(*fp, &fcode, filter, 1, NetMask) < 0)
    {
            qDebug() << "Error compiling filter: wrong syntax.";
    }

    //set the filter
    if(pcap_setfilter(*fp, &fcode)<0)
    {
        qDebug() << "Error setting the filter";
    }

    return 1;
}
