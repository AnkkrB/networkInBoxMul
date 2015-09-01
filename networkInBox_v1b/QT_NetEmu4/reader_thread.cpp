#include "reader_thread.h"
#include "buffer.h"
#include "PerformanceTimers.h"
#include "networkinterface.h"

#include <cmath>
#include <QDebug>

FILE *fpDataV;
FILE *fpSeqNo;
u_char *pktWifi_data[MAX_PKTS_STORED];
extern u_char TxPacket_tst[PPI802_HEADER_SIZE];
static int counter = 0;
extern char *interfaceOneFileOut;

ReaderThread::ReaderThread(QObject *parent) :
    QThread(parent)
{
    interfaceNumber = 0;
    lossrate = 0;
    packetCount = 0;
    pAdapter = NULL;
    quit = true;
    packetsSent = 0;
    packetsDropped = 0;
}

void ReaderThread::stop()
{
    if ( isRunning() )
    {
        // end the current thread loop
        quit = true;
        wait();
    }
     fclose(fpDataV);
     fclose(fpSeqNo);
}

void ReaderThread::setLossRate(int lossrate_)
{
    mutexLossRate.lock();
    lossrate = lossrate_;
    mutexLossRate.unlock();
}

void ReaderThread::readFromAdapter(int interfaceNumber_,
                                   pcap_t* pAdapter_,
                                   Buffer* buffer_)
{
    if ( isRunning() )
    {
        // end the current thread loop
        quit = true;
        wait();
    }

    interfaceNumber = interfaceNumber_;
    pAdapter = pAdapter_;
    buffer = buffer_;
    quit = false;
    resetTotalBytesRead();
    packetsSent = 0;
    packetsDropped = 0;

    start();
}

unsigned long ReaderThread::getTotalPacketsSent()
{
    return packetsSent;
}

unsigned long ReaderThread::getTotalPacketsDropped()
{
    return packetsDropped;
}

unsigned long ReaderThread::getBufferSize()
{
    return buffer->getSize();
}

unsigned long ReaderThread::getTotalBytesRead()
{
    mutexBytesRead.lock();
    unsigned long temp = totalBytes;
    mutexBytesRead.unlock();
    return temp;
}

void ReaderThread::resetTotalBytesRead()
{
    mutexBytesRead.lock();
    totalBytes = 0;
    mutexBytesRead.unlock();
}

void ReaderThread::increaseTotalBytesRead(int value)
{
    mutexBytesRead.lock();
    totalBytes += value;
    mutexBytesRead.unlock();
}

bool ReaderThread::isThisPacketToBeDropped()
{
    mutexLossRate.lock();
    int currentLossRate = lossrate;
    mutexLossRate.unlock();

    bool result;

    if ( currentLossRate == 0)
        result = false;
    {
        double packetsToSend = 100.0/currentLossRate;

        if ( currentLossRate > 50 )
            packetsToSend = ceil(1/(packetsToSend-1));

        if ( currentLossRate > 50)
        {
            if ( packetCount < packetsToSend )
                result = true;
            else
            {
                packetCount = 0;
                result = false;
            }
        }
        else
        {
            if ( packetCount < packetsToSend )
                result = false;
            else
            {
                packetCount = 0;
                result = true;
            }
        }
    }

    packetCount++;

    if ( result )
    {
        packetsDropped++;
        //qDebug() << "Packet dropped from interface " << interfaceNumber ;
    }
    else
    {
        packetsSent++;

       // qDebug() << "Packet sent from interface " << interfaceNumber ;
    }

    return result;
}

void PrintPackets(const u_char *pkt_data, u_int BufferSize)
{
    const u_char *Buf;
    u_int Off = 0;
    u_int TLen, TLen1;
    const u_char *pChar;

    char temp1;
    char temp2;
    static int seqCounter = 0;
    int seqNo = 0;

    Buf = pkt_data;
    Off=0;

    if(Off < BufferSize)
    {
        seqNo = pkt_data[106] + seqCounter;
        fprintf(fpSeqNo, "\n%d:   %d:   MCS:    %d", seqNo, pkt_data[107], pkt_data[45]);
        if (pkt_data[106] == 255)
            seqCounter = seqNo+1;

        Off += 42;
        pChar =(const u_char*)(Buf + Off);

        while(Off < BufferSize) {
            fprintf(fpDataV, "%c", pkt_data[Off]);
            Off++;
        }
    }
}

void ReaderThread::run()
{
    qDebug()  << "Thread ID: " <<  QThread::currentThreadId() << " Starting to receive traffic on interface " << interfaceNumber;
    struct pcap_pkthdr *header;
    const u_char *pkt_data;
    int res;

    if (fpDataV == 0)
        fpDataV = fopen(interfaceOneFileOut, "wb");
    if(fpSeqNo == 0)
        fpSeqNo = fopen("SeqNo.txt","wb");

    while(!quit && (res = pcap_next_ex( pAdapter , &header, &pkt_data)) >= 0)
    {
#ifdef ONLINE
        if (header->caplen>0){
            PrintPackets(&pkt_data[0], header->caplen);
        }else{
            int temp=0;
            temp=-1;
            continue;
        }
#else
        pktWifi_data[counter] = (u_char*)malloc(sizeof(u_char)*(header->caplen));
        memcpy(&pktWifi_data[counter][0], pkt_data, sizeof(u_char)*(header->caplen));
        PrintPackets(&pktWifi_data[counter][0], header->caplen);
#endif

        if(res == 0)
            continue; // Timeout elapsed

        increaseTotalBytesRead( header->caplen );

        if (  !isThisPacketToBeDropped() )
#ifdef ONLINE		
            buffer->addPacket(pkt_data,header->caplen);
#else
            buffer->addPacket(pktWifi_data[counter],(header->caplen+PPI802_HEADER_SIZE));
        counter = (counter + 1) % MAX_PKTS_STORED;
#endif		

    }
    qDebug() << "Reader thread "<< interfaceNumber <<  " exited";
    emit readEnds();
}


