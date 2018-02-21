#ifndef CPP_ZIA_ZIA_HH
#define CPP_ZIA_ZIA_HH

#include <map>
#include <chrono>
#include <thread>

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
    bool 				_isStart;

    ModuleLoader	_modulesLoader;
    Process		_process;
    Parser		_parser;

    long long _port;
    long long _portSsl;
    std::string _moduleNet;
    std::vector<std::string>  _modules;
    std::vector<std::string>  _modulesPath;
    bool  _debug;
    long long _logLevel;


    // Module Net
    DLLoader<::zia::api::Net>		_dlLoaderNet;
    ::zia::api::Net 			*_net;
   public:
    zia();
    virtual ~zia();

    void start();
    void stop();
    void reload();

   private:
    void loadAll();
    void  loadConf();
  };
}

#endif //CPP_ZIA_ZIA_HH
