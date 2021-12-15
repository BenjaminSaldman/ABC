/*
	TCP/IP client
*/ 


#include <stdio.h>

#if defined _WIN32

// link with Ws2_32.lib
#pragma comment(lib,"Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>

#else
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <time.h>
#include <errno.h> 
#include <string.h>
#endif

#define SERVER_PORT 5060
#define SERVER_IP_ADDRESS "10.0.2.15"
#define BUFFER_SIZE 65536
int main()
{
    char buff[BUFFER_SIZE]={0};

for(int i=0;i<5;i++)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    //char buff[BUFFER_SIZE]={0};
    if(sock == -1)
    {
        printf("Could not create socket : %d",errno);
    }
    // "sockaddr_in" is the "derived" from sockaddr structure
    // used for IPv4 communication. For IPv6, use sockaddr_in6
    //
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
	int rval = inet_pton(AF_INET, (const char*)SERVER_IP_ADDRESS, &serverAddress.sin_addr);
	if (rval <= 0)
	{
		printf("inet_pton() failed");
		return -1;
	}
     // Make a connection to the server with socket SendingSocket.

     if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1)
     {
	   printf("connect() failed with error code : %d" ,errno);
     }
     printf("connected to server\n");
     // Sends some data to server
     FILE *fp;
     fp=fopen("1gb.txt","r");
     if(fp==NULL)
     {
         printf("Could'nt read the FILE!\n");
         return -1;
     }
    int len = 0;
    int bytesSent = 0;
    int bytes_sent = 0;
    int numOfSeg = 0;
    while(len=fread(buff,1,sizeof(buff),fp)>0)
    {
        bytesSent=send(sock,buff,len,0);
        if (-1 == bytesSent)
        {
            printf("send() failed with error code : %d" ,errno);
        }
        else if (0 == bytesSent)
        {
 	        printf("peer has closed the TCP connection prior to send().\n");
        }
        else if(len>bytesSent)
        {
            printf("sent only %d bytes from the required %d.\n", len, bytesSent);
        }
        else{
            	printf("message was successfully sent .\n");
        }
        bytes_sent +=bytesSent;
        numOfSeg++;
    }
        printf("message number %d was successfully sent, with %d segments .\n",i+1,numOfSeg);
        fclose(fp);
        close(sock);

    }
    printf("Changing TCP algorihtm to reno \n");
    for(int i=0;i<5;i++)
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in serverAddress;
        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(SERVER_PORT);
	    int rval = inet_pton(AF_INET, (const char*)SERVER_IP_ADDRESS, &serverAddress.sin_addr);
        if(sock=socket(AF_INET,SOCK_STREAM,0)<0){
            printf("Could'nt create new socket\n");
            return -1;
        }
	    if (rval <= 0)
	    {
		    printf("inet_pton() failed");
		    return -1;
	    }
        char buf[BUFFER_SIZE];
        socklen_t len;
        sock = socket(AF_INET, SOCK_STREAM, 0);
 	    if (sock == -1) {
 		perror("socket");
        return -1;
        } 
        len = sizeof(buf); 
        if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) { 
        perror("getsockopt");
        return -1;
        } 
        printf("Current: %s\n", buf); 
        strcpy(buf, "reno"); 
        len = strlen(buf);
        if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) {
        perror("setsockopt"); 
        return -1;
        }
        len = sizeof(buf); 
        if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) {
        perror("getsockopt"); 
        return -1; 
        }
        printf("Changed algorithm\n");
        if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1)
        {
	     printf("connect() failed with error code : %d" ,errno);
        }
        printf("connected to server\n");
        FILE *fp;
        fp=fopen("1gb.txt","r");
        if(fp==NULL)
        {
         printf("Could'nt read the FILE!\n");
         return -1;
        }
        int length = 0;
        int bytesSent = 0;
        int bytes_sent = 0;
        int numOfSeg = 0;
        while(length=fread(buff,1,sizeof(buff),fp)>0)
        {
            bytesSent=send(sock,buff,length,0);
            if (-1 == bytesSent)
            {
                printf("send() failed with error code : %d" ,errno);
            }
            else if (0 == bytesSent)
            {
 	            printf("peer has closed the TCP connection prior to send().\n");
            }
            else if(length>bytesSent)
            {
                printf("sent only %d bytes from the required %d.\n", length, bytesSent);
            }
            else{
            	printf("message was successfully sent .\n");
            }
        bytes_sent +=bytesSent;
        numOfSeg++;
        }
        printf("message number %d was successfully sent, with %d segments .\n",i+1,numOfSeg);
        fclose(fp);
        close(sock);
    }
    return 0; 

}