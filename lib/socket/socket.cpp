#include <lib/socket/socket.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

namespace net {

socketconnect::socketconnect(const std::string& ip, int port){

    
    struct sockaddr_in serv_addr; 

    if((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 

    if(inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
    } 

    if( connect(_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
    } 

    start();

}

pthread_t& socketconnect::start(){
	
	pthread_create(&_thread, NULL, &socketconnect::staticMainPathThread, this);
	return _thread;

}

void* socketconnect::staticMainPathThread(void *arg) {
    return reinterpret_cast<socketconnect*>(arg)->mainPathThread();
}

void* socketconnect::mainPathThread(){

    printf("\n Socket thread started ...\n");
    int n = 0;
    char recvBuff[1024];
    memset(recvBuff, '0',sizeof(recvBuff));

    while ( (n = read(_sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    } 

    if(n < 0)
    {
        printf("\n Read error \n");
    } 

    return 0;

}

}
