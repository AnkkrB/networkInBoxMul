#include "networkinterface.h"
#include "buffer.h"
#include "CollectedStatistics.h"

#include <QDebug>
#include <QNetworkInterface>

#include "PpiHeader.h"

int flag;
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
    flag = 0;
    lookupAdapterInterfaces();
    buffer1 = new Buffer(BUFFER_SIZE);
    buffer2 = new Buffer(BUFFER_SIZE);
    connect(&monitorThread,SIGNAL(statisticsCollected(CollectedStatistics*)),this,SLOT(receiveStatistics(CollectedStatistics*)));

    connect(&readerThread1, SIGNAL(readEnds()), this, SLOT(readerThreadExits()));
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
   // pcap_close(pAdapterTwo);

//    writerThread1.stop();
    flag = 1;
//    writerThread2.stop();
   // fclose(fpData);
}

void NetworkEmulator::receiveStatistics(CollectedStatistics* stats)
{
    emit statisticsCollected(stats);
}

void NetworkEmulator::readerThreadExits()
{
    emit readEnds();
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

static int setFilterOnce = 0;
void NetworkEmulator::setMacFilters(QString filterInterfaceOne, QString filterInterfaceTwo )
{
    if (setFilterOnce == 0) {
        interfaceDataList[selectedInterfaceIndex1].hardwareAddress = "00:80:48:69:34:17";
        interfaceDataList[selectedInterfaceIndex2].hardwareAddress = "00:80:48:6f:23:24";
        //interfaceDataList[selectedInterfaceIndex2].hardwareAddress = "00:80:48:6f:23:05";
        QString interface1MAC = interfaceDataList[selectedInterfaceIndex1].hardwareAddress;
        QString interface2MAC = interfaceDataList[selectedInterfaceIndex2].hardwareAddress;

        QString baseFilter1 = QString("(wlan src %1)").arg(interface1MAC);

#ifdef ONLINE
        setupWinPcapFilter(&pAdapterOne, baseFilter1);
#endif
        setFilterOnce = 1;
    }
}
char *interfaceOneFileOut;
extern const char *interfaceOne;
void NetworkEmulator::setSelectedInterfaces( int interface1, int interface2)
{
    //An
    AirpcapMacAddress MacAddress;
    double testtxrates = 9;
    int ratesend = 9;
    AirpcapChannelInfo 	ChannelInfo;
    selectedInterfaceIndex1 = interface1;
    selectedInterfaceIndex2 = interface2;

    QByteArray ba1 = interfaceDataList[interface1].interfaceName.toLatin1();
    QByteArray ba2 = interfaceDataList[interface2].interfaceName.toLatin1();

#ifdef ONLINE
    const char *interfaceOne = ba1.data();
#else
    interfaceOneFileOut = (char*)malloc(sizeof(char)*(5+strlen(interfaceOne) ));
    interfaceOneFileOut = strcpy(interfaceOneFileOut, interfaceOne+7);
    interfaceOneFileOut = strcat(interfaceOneFileOut, ".264\0");

#endif
    const char *interfaceTwo = ba2.data();

    char errbuf[PCAP_ERRBUF_SIZE];

    if ( (pAdapterOne= pcap_open((const char*)interfaceOne,  // name of the device
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
    ChannelInfo.Frequency = 5540;
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
}

void NetworkEmulator::startBridge()
{
    readerThread1.readFromAdapter(1,pAdapterOne,buffer1);
    writerThread1.writeToAdapter(1,pAdapterTwo,buffer1);
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
