#ifndef CPP_ZIA_PROCESS_HH
#define CPP_ZIA_PROCESS_HH

#include "api.h"
#include "../ModuleLoader/ModuleLoader.hh"

namespace nz {
  class Process
  {
   private:
    ModuleLoader	&_moduleLoader;
   public:
    Process(ModuleLoader &moduleLoader);

    void startProcess(zia::api::HttpDuplex duplex);
  };
}

#endif //CPP_ZIA_PROCESS_HH
