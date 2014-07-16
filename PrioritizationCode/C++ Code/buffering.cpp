#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include <errno.h>
#include "classes/NALParser.h"

#define PORT 39082
#define PKT_SIZE 65536
#define FILENAME "buffer.dump"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cout << "Usage: buffering <seconds>" << endl;
		exit(0);
	}
	int serv_sockfd, num_bytes, pkts_recv = 0, ticks = -1;
	int seconds = atoi(argv[1]);
	char packet[PKT_SIZE];
	//Some timers
	time_t start, end;
	double timedif;
	int print_now = 0;
	
	//The file
	ofstream buffer;
	
	//Storage for sender's address
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	
	//NAL unit parser
	NALParser parser;

	//Start a UDP server -- open up a port
	//Open a socket
	serv_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(serv_sockfd <= 0)
	{
		perror("socket");
		exit(1);
	}

	//Bind to port on host side
	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons((unsigned short) PORT);

	int bound = bind(serv_sockfd, (struct sockaddr *) &address, sizeof(address));
	if(bound < 0)
	{
		perror("bind");
		exit(1);
	}

	//Set non-blocking
	//int nonBlocking = 1;
	//if(fcntl(serv_sockfd, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
	//{
		//perror("fcntl");
		//exit(1);
	//}
	
	//Open the file
	buffer.open(FILENAME, ios::binary);
	if(!buffer.is_open())
	{
		perror("open");
	}
	while(ticks < seconds)
	{
		memset(packet, 0, PKT_SIZE);
		//Get next packet
		if((num_bytes = recvfrom(serv_sockfd, packet, PKT_SIZE, 0,
			(struct sockaddr *)&their_addr, &addr_len)) == -1)
		{
			if(errno != EAGAIN)
			{
				perror("recvfrom");
				exit(1);
			}		
		}
		
		if(errno != EAGAIN)
		{
			pkts_recv++;
			//printf("Packet length: %d\n", num_bytes);
			//int i;
			//for(i = 0; packet[i] == 0; i++) ;
			
			//Byte 4: 00000001 (0x1)
			//First packet byte 5: 01100111 (0x67)
			//First frame byte 5: 01100101 (0x65)
			//Else: 01000001 (0x41)
			//printf("Byte %d: 0x%x\t Byte %d: 0x%x\t Byte %d: 0x%x\t Size: %d\n", 1, packet[0] & 0xff, 
			//			2, packet[1] & 0xff, 3, packet[2] & 0xff, num_bytes);
			
			char *dat = packet;			
			PacketPriority type = parser.prioritizePacket((uint8_t *) dat, num_bytes);
			cout << type << endl;
			//if(type != NAL_TYPE_UNSPEC)
			//	cout << "===========NEW FRAME===========" << endl;

		//	cout << hex << (int) packet[0] << endl;
			
			//if(packet[4] != 0x67 && packet[4] != 0x65)
			//{
				//dat++;
				//num_bytes--;
			//}
			//Write to file
			buffer.write(dat, num_bytes);
			
		}
	}
	
	close(serv_sockfd);
	buffer.close();
	return 0;
	
}
