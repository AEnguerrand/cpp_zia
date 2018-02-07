#ifndef CPP_ZIA_NETWORK_HH
#define CPP_ZIA_NETWORK_HH

#include "api.h"

namespace nzm {
  class Network : public zia::api::Net
  {
   public:
    Network();

   private:
    virtual bool config(const zia::api::Conf &conf);

    virtual bool run(Callback cb);

    virtual bool send(zia::api::ImplSocket *sock, const Raw &resp);

    virtual bool stop();
  };
}

// Create instance

extern "C" zia::api::Net * create()
{
  return new(nzm::Network);
}

#endif //CPP_ZIA_NETWORK_HH
