#ifndef CPP_ZIA_ZIA_HH
#define CPP_ZIA_ZIA_HH

#include <map>
#include <chrono>
#include <thread>
#include <exception>

#include "api.h"

#include "Log.hpp"

#include "ModuleLoader/ModuleLoader.hh"
#include "Process/Process.hh"
#include "Parser/Parser.hh"
#include "Parser/ParserJson.hh"

namespace nz {
  class zia
  {
   private:
    bool 			_isStart;
    Process			_process;
    Parser			_parser;

    // Configuration
    ::zia::api::Conf		_conf;
    std::string 		_moduleNet;
    std::vector<std::string>  	_modules;
    std::vector<std::string>  	_modulesPath;

    bool 			_debug;
    long long 			_logLevel;

    // Modules
    ModuleLoader		_modulesLoader;
    // Network
    DLLoader<::zia::api::Net>	_dlLoaderNet;
    std::string 		_moduleNetPath;
    ::zia::api::Net 		*_net;
   public:
    zia();
    virtual ~zia();

   public:
    void 	start();
    void 	stop();
    void 	reload();

   private:
    void  	loadConf();
    void	loadModules();

    void 	loadNetwork();
  };
}

#endif //CPP_ZIA_ZIA_HH
