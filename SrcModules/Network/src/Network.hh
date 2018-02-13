#ifndef CPP_ZIA_NETWORK_HH
#define CPP_ZIA_NETWORK_HH

#include <unordered_map>
#include <thread>
#include <memory>

#include "Errors.hpp"
#include "Log.hpp"
#include "api.h"

#include "Socket.hh"
#include "Buffer.hh"

namespace nzm {
  class Select;
  class Network : public zia::api::Net
  {
   private:
    std::unordered_map<short, std::shared_ptr<std::thread>>	_thListen;
   public:
    Network();

    virtual ~Network();

   public:
    virtual bool config(const zia::api::Conf &conf);

    virtual bool run(Callback cb);

    virtual bool send(zia::api::ImplSocket *sock, const Raw &resp);

    virtual bool stop();

   private:
    void runSelect(short port, zia::api::Net::Callback cb);

   public:
    std::vector<zia::api::ImplSocket *>				_sockets;
  };
}

#endif //CPP_ZIA_NETWORK_HH
