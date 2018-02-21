#ifndef CPP_ZIA_SOCKET_HH
#define CPP_ZIA_SOCKET_HH


#include <vector>
#include <functional>

#include "CrossPlateformFunctions.hpp"
#include "Errors.hpp"
#include "Buffer.hh"
#include "Log.hpp"
#include "api.h"

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

    bool isInit() const;


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
