#ifndef BUFFER_H
#define BUFFER_H

#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <windows.h>

#define BUFFER_SIZE 5000
#define Test_tx_len 30
#define PPI802_HEADER_SIZE 90 // sizeof(PPI_PACKET_HEADER) + 802.11n header
#define MAX_PKTS_STORED 5000

class Buffer
{
public:
    Buffer(long size);
    void addPacket(const uchar* packet, int size);
    int removePacket(const uchar** packet);
    int getSize();
    void reset();

private:
    QQueue<const u_char*> FIFO;
    QQueue<int> sizeFIFO;
    QMutex mutexFIFO;

    long bufferSize;

    bool quit;

    QWaitCondition bufferEmpty;
    QMutex wMutex;

};
extern u_char *pktWifi_data[MAX_PKTS_STORED];

#endif // BUFFER_H
