#ifndef MODULE_ROUTER_HPP_
# define MODULE_ROUTER_HPP_

#include "api.h"
#include "Router.hh"

namespace nzm
{
  class Module : public zia::api::Module
  {
   private:
    Router	_router;
   public:
    Module();

    ~Module();

    /**
    * Called after contruct and when config changed.
    * \return true on success, otherwise false.
    */
    bool config(const zia::api::Conf &conf);

    /**
    * Called on HTTP request.
    * \return true on success, otherwise false.
    */
    bool exec(zia::api::HttpDuplex &http);
  };
}

#endif