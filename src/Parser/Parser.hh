#ifndef CPP_ZIA_PARSER_HH
#define CPP_ZIA_PARSER_HH

#include "api.h"
#include "Log.hpp"
#include "../Process/Process.hh"

namespace nz {
  class Parser
  {
   private:
    Process &_process;
   public:
    Parser(Process & process);

    void callbackRequestReceived(::zia::api::Net::Raw raw, ::zia::api::NetInfo netInfo);
  };
}

#endif //CPP_ZIA_PARSER_HH
