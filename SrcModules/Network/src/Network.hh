#ifndef CPP_ZIA_NETWORK_HH
#define CPP_ZIA_NETWORK_HH

#include <unordered_map>
#include <thread>

#include "Errors.hpp"
#include "Log.hpp"
#include "api.h"

#include "Select.hh"
#include "Socket.hh"
#include "Buffer.hh"

namespace nzm {
  class Network : public zia::api::Net
  {
   protected:
    struct ServerTcp {
      short		_port;
      Select		_select;
    };

    std::unordered_map<short, std::thread>	_thListen;
   public:
    Network();

    virtual ~Network();

   public:
    virtual bool config(const zia::api::Conf &conf);

    virtual bool run(Callback cb);

    virtual bool send(zia::api::ImplSocket *sock, const Raw &resp);

    virtual bool stop();

   private:
    void runAccept(ServerTcp serverTcp);
  };
}

// Create instance

extern "C" zia::api::Net * create()
{
  return new(nzm::Network);
}

#endif //CPP_ZIA_NETWORK_HH
