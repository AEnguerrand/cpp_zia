#ifndef CPP_ZIA_ROUTER_HH
#define CPP_ZIA_ROUTER_HH

#include <experimental/filesystem>

#include "api.h"

namespace nzm {
  class Router
  {
   public:
    Router();
    virtual ~Router();

    void execRouting(zia::api::HttpDuplex & httpDuplex);

   private:
    bool isDirectory(std::string & uri) const;

   private:
    void displayBrowsing(zia::api::HttpDuplex & httpDuplex);
  };
}

#endif //CPP_ZIA_ROUTER_HH
