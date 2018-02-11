#ifndef CPP_ZIA_SOCKET_HH
#define CPP_ZIA_SOCKET_HH

#ifdef _WIN32
#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#endif

#include "Errors.hpp"
#include "Log.hpp"

namespace nzm {
  class Socket
  {
   private:
    bool 	_isInit;
    int 	_fd;
    bool 	_isServer;

   public:
    Socket();

    int getFd() const;

    bool is_isServer() const;


    int initServer(short port);
    int initClient(Socket socketServer);

    bool operator==(const Socket & rhs) const;
  };
}

#endif //CPP_ZIA_SOCKET_HH
