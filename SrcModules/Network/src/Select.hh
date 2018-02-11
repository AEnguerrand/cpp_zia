#ifndef CPP_ZIA_SELECT_HH
#define CPP_ZIA_SELECT_HH

#include <vector>
#include <functional>

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
    std::vector<Socket>	_tunnels;
    std::vector<Socket>	_listenTunnels;

   public:
    Select();

    void run();

    void addListentTunnels(Socket socket);

    void addTunnel(Socket socket);
    void removeTunnel(Socket socket);

   private:
    int getMaxFd();
  };
}

#endif //CPP_ZIA_SELECT_HH
