#ifndef CPP_ZIA_NETWORK_HH
#define CPP_ZIA_NETWORK_HH

#include <unordered_map>
#include <thread>
#include <memory>
#include <future>

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
    std::shared_ptr<std::thread>	_select;
    long long				_port;

    bool				_isRun;
    std::atomic<bool>			_stop;
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
  };
}

#endif //CPP_ZIA_NETWORK_HH
