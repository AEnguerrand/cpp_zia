#ifndef CPP_ZIA_MODULELOADER_HH
#define CPP_ZIA_MODULELOADER_HH

#include "unordered_map"
#include <fstream>
#include <iostream>
#include <experimental/filesystem>

#include "api.h"
#include "DLLoader.hh"

// TODO: Order modules of defined in config went is getModules() is call

namespace nz {
  class ModuleLoader
  {
   private:
    DLLoader<::zia::api::Module>	_dlLoader;

   public:
    ModuleLoader();
    virtual ~ModuleLoader();

    void loadAll();
    void unloadAll();

    const DLLoader<zia::api::Module> &getDlLoader() const;
    DLLoader<zia::api::Module> &getDlLoader();

    std::unordered_map<std::string, ::zia::api::Module *> getModules();
  };
}


#endif //CPP_ZIA_MODULELOADER_HH
