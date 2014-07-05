#include "networkinterface.h"
#include "buffer.h"
#include "CollectedStatistics.h"

#include <QDebug>
#include <QNetworkInterface>

extern "C" PAirpcapHandle pcap_get_airpcap_handle(pcap_t *p);

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
    QString baseFilter1 = QString("(not ether src %1) and (not ether dst %2)").arg(interface1MAC).arg(interface2MAC);
    if ( filterInterfaceOne != "")
        baseFilter1 = baseFilter1 + QString(" and (%1)").arg(filterInterfaceOne);

    QString baseFilter2 = QString("(not ether src %1) and (not ether dst %2)").arg(interface2MAC).arg(interface1MAC);
    if ( filterInterfaceTwo != "")
        baseFilter2 = baseFilter2 + QString(" and (%1)").arg(filterInterfaceTwo);

    setupWinPcapFilter(&pAdapterOne, baseFilter1);
    setupWinPcapFilter(&pAdapterTwo, baseFilter2);
}

void NetworkEmulator::setSelectedInterfaces( int interface1, int interface2)
{
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
    if(!AirpcapSetDeviceChannel(airpcap_handle, freq_chan))
    {
         printf("Error in Setting the Channel : %s", AirpcapGetLastError(airpcap_handle));
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
}

void NetworkEmulator::startBridge()
{
    readerThread1.readFromAdapter(1,pAdapterOne,buffer1);
    writerThread1.writeToAdapter(1,pAdapterTwo,buffer1);

    readerThread2.readFromAdapter(2,pAdapterTwo,buffer2);
    writerThread2.writeToAdapter(2,pAdapterOne,buffer2);

    monitorThread.pollValues(&readerThread1,&readerThread2,1000);
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
