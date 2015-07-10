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
#include <netinet/tcp.h>


namespace net {

socketconnect::socketconnect(const std::string& ip, int port){

    
    struct sockaddr_in serv_addr; 

    if((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
    } 

    memset(&serv_addr, 0, sizeof(serv_addr)); 

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

void socketconnect::setCallback(void (*callback)(socketconnect* sock, message* msg)){
	_callback = callback;
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
    
    int res = 0;
    while (true) {
	res = readMessage();
     if (res==2 || res <0) break;	
    }    

    return 0;

}

int32_t swap_int32( int32_t val )
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | ((val >> 16) & 0xFFFF);
}

int socketconnect::readMessage(){

    int n = 0;

    int type = 0;
    n = receive(_sockfd, &type, 4);
    if (n<0) return -1;
    type = swap_int32(type);	
    printf("Reading type %d\n",type);

    int size = 0;
    n = receive(_sockfd, &size, 4);
    if (n<0) return -1;	
    size = swap_int32(size);
    printf("Reading payload size %d\n",size);	

    char recvBuff[size+1];
    memset(recvBuff, 0, sizeof(recvBuff));
    n = receive(_sockfd, (void*) recvBuff, size);
    if (n<0) return -1;
    
    printf("Payload:%s\n",recvBuff);

    message* msg = new message();
    msg->_type = type;
    msg->_payload = std::string(recvBuff);		

    if (_callback) {
       _callback(this,msg);
    }

    delete msg;

    return type;

}

int socketconnect::sendMessage(message* msg){

    int type = swap_int32(msg->_type);
    int rc = write(_sockfd, &type, 4);
    if (rc < 0) return -1;

    int size = swap_int32(msg->_payload.size());
    rc = write(_sockfd, &size, 4); 	
	
    int flag = 1; 
    setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
    rc = send(_sockfd, msg->_payload.c_str() , msg->_payload.size(), 0); 
    flag = 0; 
    setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
    printf("Writing payload size %d\n",rc);		

}

int socketconnect::receive( int fd, void* buffer, int size)
{
    void* ret = buffer;
    int left = size;
    int rc;
    while (left) {
        rc = read(fd, ret , left);
        if (rc < 0) return -1;
	   ret = (void*) ((long long int) ret + rc);
        left -= rc;
    }
    return 0;
}


}
