#ifndef CPP_ZIA_MODULELOADER_HH
#define CPP_ZIA_MODULELOADER_HH

#include "unordered_map"
#include <fstream>
#include <iostream>
#include <experimental/filesystem>

#include "api.h"
#include "DLLoader.hh"

namespace nz {
  class ModuleLoader
  {
   private:
    DLLoader<zia::api::Module>	dlLoader;

   public:
    ModuleLoader();
    virtual ~ModuleLoader();

    void loadAll();
    void unloadAll();
  };
}


#endif //CPP_ZIA_MODULELOADER_HH
