#ifndef CPP_ZIA_ZIA_HH
#define CPP_ZIA_ZIA_HH

#include <map>

#include "api.h"

#include "Log/Log.hpp"

namespace nz {
  class zia
  {
   private:
    bool 				_isStart;

    std::map<unsigned int, ::zia::api::Module &>	_modules;

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
