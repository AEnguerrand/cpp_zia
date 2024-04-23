#ifndef CPP_ZIA_ROUTER_HH
#define CPP_ZIA_ROUTER_HH

#include <filesystem>
#include <iostream>
#include <fstream>
#include <chrono>

#include "api.h"

namespace nzm
{
  class Router
  {
  public:
    Router();
    virtual ~Router();

    void execRouting(zia::api::HttpDuplex &httpDuplex);

  private:
    bool uriFound(std::string uri) const;
    bool isDirectory(std::string uri) const;
    std::string getTypeFile(std::string uri) const;

  private:
    void display404(zia::api::HttpDuplex &httpDuplex);
    void displayBrowsing(zia::api::HttpDuplex &httpDuplex);
    void displayFile(zia::api::HttpDuplex &httpDuplex);
  };
}

#endif // CPP_ZIA_ROUTER_HH
