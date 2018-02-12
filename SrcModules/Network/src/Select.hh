#ifndef CPP_ZIA_SELECT_HH
#define CPP_ZIA_SELECT_HH

#include <vector>
#include <functional>
#include <memory>

#ifdef _WIN32
#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/select.h>
#include <arpa/inet.h>
#endif

#include "Errors.hpp"
#include "Log.hpp"

#include "Socket.hh"

namespace nzm {
  class Select
  {
   private:
    fd_set _fdsRead;

   private:
    std::vector<std::shared_ptr<Socket>>	_tunnels;
    std::vector<std::shared_ptr<Socket>>	_listenTunnels;

   public:
    Select();

    void run();

    void addListenTunnels(std::shared_ptr<Socket> socket);

    void addTunnel(std::shared_ptr<Socket>  socket);
    void removeTunnel(std::shared_ptr<Socket> socket);

   private:
    int getMaxFd();
  };
}

#endif //CPP_ZIA_SELECT_HH
