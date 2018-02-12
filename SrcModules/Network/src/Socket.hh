#ifndef CPP_ZIA_SOCKET_HH
#define CPP_ZIA_SOCKET_HH

#ifdef _WIN32
#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#endif

#include <vector>
#include <unistd.h>
#include <functional>

#include "api.h"
#include "Errors.hpp"
#include "Log.hpp"
#include "Buffer.hh"

namespace nzm {
  class Socket
  {
   private:
    bool 	_isInit;
    int 	_fd;
    bool 	_isServer;

    Buffer	_bufferIn;
    Buffer	_bufferOut;

   public:
    Socket();
    virtual ~Socket();

    int getFd() const;

    bool isServer() const;


    int initServer(short port);
    int initClient(Socket & socketServer);

    int read();
    int write(zia::api::Net::Raw raw);

    void checkWrite();

    const Buffer &getBufferIn() const;
    Buffer &getBufferIn();

    const Buffer &getBufferOut() const;
    Buffer &getBufferOut();

    bool operator==(const Socket & rhs) const;
  };
}

#endif //CPP_ZIA_SOCKET_HH
