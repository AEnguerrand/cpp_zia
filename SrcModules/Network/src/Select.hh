#ifndef CPP_ZIA_SELECT_HH
#define CPP_ZIA_SELECT_HH

#include <unordered_map>
#include <functional>

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
  class Select
  {
   protected:
    struct Tunnel {
      bool isServer;
      int fd;
      std::function<void(unsigned int fd)>	_readFunc;
      std::function<void(unsigned int fd)>	_writeFunc;
    };
   private:
    std::unordered_map<unsigned int, Tunnel>	_tunnels;

   public:
    Select();

    void run();
    void addTunnel(int fd, std::function<void(unsigned int fd)> readFunc, std::function<void(unsigned int fd)> writeFunc, bool isServer = false);
    void removeTunnel(int fd);
  };
}

#endif //CPP_ZIA_SELECT_HH
