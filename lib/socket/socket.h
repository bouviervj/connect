#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string>

namespace net {

class socketconnect {

public:
  socketconnect(const std::string& ip, int port);

private:

  pthread_t& start(); 	  
  static void* staticMainPathThread(void *arg);
  void* mainPathThread();

  int _sockfd; // Socket file descriptor
  pthread_t _thread;

};

}

#endif


