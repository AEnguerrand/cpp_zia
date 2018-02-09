#ifndef CPP_ZIA_ZIA_HH
#define CPP_ZIA_ZIA_HH

#include <map>

#include "api.h"

#include "Log.hpp"

#include "ModuleLoader/ModuleLoader.hh"
#include "Process/Process.hh"
#include "Parser/Parser.hh"

namespace nz {
  class zia
  {
   private:
    bool 				_isStart;

    ModuleLoader	_modulesLoader;
    Process		_process;

    // Module Net
    DLLoader<::zia::api::Net>		_dlLoaderNet;
   public:
    zia();
    virtual ~zia();

    void start();
    void stop();
    void reload();

   private:
    void loadAll();
  };
}

#endif //CPP_ZIA_ZIA_HH
