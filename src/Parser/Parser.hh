#ifndef CPP_ZIA_PARSER_HH
#define CPP_ZIA_PARSER_HH

#include "api.h"
#include "Log.hpp"
#include "../Process/Process.hh"

namespace nz {
  class Parser
  {
   private:
    Process 		&_process;
    ::zia::api::Net 	*_net;
   public:
    Parser(Process & process, ::zia::api::Net *net);

    void callbackRequestReceived(::zia::api::Net::Raw raw, ::zia::api::NetInfo netInfo);

    void setNet(zia::api::Net *net);
  };
}

#endif //CPP_ZIA_PARSER_HH
