#ifndef CPP_ZIA_MODULELOADER_HH
#define CPP_ZIA_MODULELOADER_HH

#include "unordered_map"
#include <fstream>
#include <iostream>
#include <experimental/filesystem>


#include "api.h"
#include "Log.hpp"

#ifndef _WIN32

#include "DLLoader.hh"

#else
#include "DLLoader_Win.hh"
#endif

namespace nz
{
  class ModuleLoader
  {
   private:
    DLLoader<::zia::api::Module>		 	_dlLoader;
    std::unordered_map<std::string, std::string> 	_modules;
    std::vector<std::string> &				_modulesName;
    std::vector<std::string> &				_modulesPath;

    const ::zia::api::Conf &				_conf;

   public:
    ModuleLoader(std::vector<std::string> &modulesName, std::vector<std::string> &modulesPath, const ::zia::api::Conf &conf);

    virtual ~ModuleLoader();

    void loadAll();

    bool unloadAll();

    const DLLoader<zia::api::Module> &getDlLoader() const;

    DLLoader<zia::api::Module> &getDlLoader();

    std::unordered_map<std::string, ::zia::api::Module *> getModules();

   public:
    void addModule(const std::string &moduleName);

    bool deleteModuleByName(const std::string &moduleName);

   private:
    const std::string convertToFilename(const std::string &moduleName) const;
   };
}


#endif //CPP_ZIA_MODULELOADER_HH
