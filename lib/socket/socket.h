#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string>

namespace net {

struct message {
	int _type;
	std::string _payload;	
};

class socketconnect {

public:
  socketconnect(const std::string& ip, int port);

  void setCallback(void (*callback)(socketconnect* sock, message* msg));	

  int sendMessage(message* msg);

private:

  pthread_t& start(); 	  
  static void* staticMainPathThread(void *arg);
  void* mainPathThread();
  int readMessage();
  int receive( int fd, void* buffer, int size);

  int _sockfd; // Socket file descriptor
  pthread_t _thread;

  void (*_callback)(socketconnect* sock, message* msg); 

};

}

#endif


