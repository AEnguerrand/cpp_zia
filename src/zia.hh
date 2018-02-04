#ifndef CPP_ZIA_ZIA_HH
#define CPP_ZIA_ZIA_HH

#include <map>

#include "api.h"

#include "Log/Log.hpp"

#include "ModuleLoader/ModuleLoader.hh"

namespace nz {
  class zia
  {
   private:
    bool 				_isStart;

    ModuleLoader	_modulesLoader;

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
