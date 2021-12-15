/*
    TCP/IP-server
*/

#include<stdio.h>

#if defined _WIN32
#include<winsock2.h>   //winsock2 should be before windows
#pragma comment(lib,"ws2_32.lib")
#else
// Linux and other UNIXes
#include <sys/socket.h>
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h>
#include <string.h> 
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#endif
#define SERVER_PORT 5060  //The port that the server listens
#define SIZE 65536
static time_t total_time = 0;
static double ave_time = 0;
static int count = 0;
int main()
{      
    // Open the listening (server) socket
    int listeningSocket = -1;  
    if((listeningSocket = socket(AF_INET , SOCK_STREAM , 0 )) == -1)
    {
        printf("Could not create listening socket : %d" ,errno);
    }
	// Reuse the address if the server socket on was closed
	// and remains for 45 seconds in TIME-WAIT state till the final removal.
	//
    int enableReuse = 1;
    if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(int)) < 0)
    {
         printf("setsockopt() failed with error code : %d" ,errno);
    }
    // "sockaddr_in" is the "derived" from sockaddr structure
    // used for IPv4 communication. For IPv6, use sockaddr_in6
    //
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(SERVER_PORT);  //network order
    // Bind the socket to the port with any IP at this port
    if (bind(listeningSocket, (struct sockaddr *)&serverAddress , sizeof(serverAddress)) == -1)
    {
        printf("Bind failed with error code : %d", errno);
	// TODO: close the socket
        return -1;
    }
    printf("Bind() success\n");
    // Make the socket listening; actually mother of all client sockets.
    if (listen(listeningSocket, 500) == -1) //500 is a Maximum size of queue connection requests
									//number of concurrent connections 
    {
	printf("listen() failed with error code : %d",errno);
	// TODO: close the socket
     return -1;
    }
    //Accept and incoming connection
    printf("Waiting for incoming TCP-connections...\n");

    struct sockaddr_in clientAddress;  //
    socklen_t clientAddressLen = sizeof(clientAddress);
    char bufcc[256];
	socklen_t len = sizeof(bufcc);
	if (getsockopt(listeningSocket, IPPROTO_TCP, TCP_CONGESTION, bufcc, &len) != 0) 
	{ 
		perror("getsockopt");
		return -1;
	}
    char reciver[SIZE];
    int bytesReceived=0;
    for(int i=0;i<5;i++)
    {
    	memset(&clientAddress, 0, sizeof(clientAddress));
        clientAddressLen = sizeof(clientAddress);
        int clientSocket = accept(listeningSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
    	if (clientSocket == -1)
    	{
           printf("listen failed with error code : %d",errno);
	   // TODO: close the sockets
           return -1;
    	}
      printf("A new client connection accepted\n");
        clock_t begin = clock();
		time_t beg = time(0);
        int total_recived=0;
        while( (bytesReceived = recv(clientSocket, reciver, sizeof(reciver), 0)) > 0 ) 
		{
	        total_recived += bytesReceived;
	    }
        time_t en = time(0);
		clock_t end = clock();
        double time_for_call = (double)(end - begin) / CLOCKS_PER_SEC;
		count++;
		total_time += en-beg;
		ave_time += time_for_call;
	    printf("Received %d bytes in %f seconds \t", total_recived, time_for_call);
		printf("time in about seconds: %ld \n",en-beg);
	    if (bytesReceived<0)
	        perror("Receiving");
		close(clientSocket);
	}
    printf("total avarage time that took for the first algorithm: %f\n", ave_time/count);
	printf("total time the aglorithm ran: %ld seconds\n", total_time);
    printf("Changing algorithm to reno\n");
    strcpy(reciver, "reno");
    len=sizeof(reciver);
    if (setsockopt(listeningSocket, IPPROTO_TCP, TCP_CONGESTION, reciver, len) != 0) 
		{
			perror("setsockopt"); 
			return -1;
		}
		
	if (getsockopt(listeningSocket, IPPROTO_TCP, TCP_CONGESTION, reciver, &len) != 0) 
		{
			perror("getsockopt"); 
			return -1; 
		}
    printf("Successfully changed CC to %s\n",reciver);
    bytesReceived = 0;
	total_time = 0;
	ave_time = 0;
	count = 0;
     for(int i=0;i<5;i++)
    {
    	memset(&clientAddress, 0, sizeof(clientAddress));
        clientAddressLen = sizeof(clientAddress);
        int clientSocket = accept(listeningSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
    	if (clientSocket == -1)
    	{
           printf("listen failed with error code : %d",errno);
	   // TODO: close the sockets
           return -1;
    	}
      printf("A new client connection accepted\n");
        clock_t begin = clock();
		time_t beg = time(0);
        int total_recived=0;
        while( (bytesReceived = recv(clientSocket, reciver, sizeof(reciver), 0)) > 0 ) 
		{
	        total_recived += bytesReceived;
	    }
        time_t en = time(0);
		clock_t end = clock();
        double time_for_call = (double)(end - begin) / CLOCKS_PER_SEC;
		count++;
		total_time += en-beg;
		ave_time += time_for_call;
	    printf("Received %d bytes in %f seconds \t", total_recived, time_for_call);
		printf("time in about seconds: %ld \n",en-beg);
	    if (bytesReceived<0)
	        perror("Receiving");
		close(clientSocket);
	}
    printf("total avarage time that took for the reno algorithm: %f\n", ave_time/count);
	printf("total time the aglorithm ran: %ld seconds\n", total_time);
    return 0;
}