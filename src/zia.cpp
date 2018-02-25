#include "zia.hh"

nz::zia::zia():
  _isStart(false),
  _process(_modulesLoader),
  _parser(_process, nullptr),
  _modulesLoader(_modules, _modulesPath, _conf),
  _dlLoaderNet("moduleNet", true)
{
}

nz::zia::~zia()
{
  stop();
}

void nz::zia::start()
{
  if (_isStart)
    {
      Log::error("Server is already start, please stop it before", "Zia Core", 2);
      return ;
    }
  Log::inform("Server booting ...");
  loadModules();
  // Run network
  ::zia::api::Net::Callback funcCallback = std::bind(&nz::Parser::callbackRequestReceived, _parser, std::placeholders::_1,
						     std::placeholders::_2);
  Log::inform("Server is run");
  _net->run(funcCallback);

  _isStart = true;
}
void nz::zia::stop()
{
  if (!_isStart)
    {
      Log::error("Server is not start", "Zia Core", 1);
      return ;
    }
  Log::inform("Server halt ...");
  _modulesLoader.unloadAll();

  _net->stop();
  _dlLoaderNet.destroyLib(_moduleNetPath);

  _conf.clear();
  _modules.clear();
  _modulesPath.clear();
  _net = nullptr;
  _moduleNet.clear();
  _moduleNetPath.clear();

  _isStart = false;
}

void nz::zia::reload()
{
  if (!_isStart)
    {
      Log::error("Server is not start", "Zia Core", 1);
      return ;
    }
  Log::inform("Server reload ...");
  stop();
  start();
}

void  nz::zia::loadConf()
{
  ParserJson        parser("../conf/config.json");
  _conf = parser.getConfig();

  // Get module_net (string) from config file
  try { _moduleNet = std::get<std::string>(_conf.at("module_net").v); }
  catch (...) {
    nz::Log::warning("module_net not found or must be a string, default module network set to 'cpp_zia_module_network'", "Zia Core");
    _moduleNet = "cpp_zia_module_network";
  }

  // Get modules (array) from config file
  try {
    ::zia::api::ConfArray modules = std::get<::zia::api::ConfArray>(_conf.at("modules").v);
    for (auto module : modules) {
      _modules.push_back(std::get<std::string>(module.v));
    }
  }
  catch (...) {
    nz::Log::warning("modules not found or must be an array, default modules set to 'cpp_zia_module_router'", "Zia Core");
    _modules.push_back("cpp_zia_module_router");
  }

  // Get modules_path (array) from config file
  try {
    ::zia::api::ConfArray modulesPath = std::get<::zia::api::ConfArray>(_conf.at("modules_path").v);
    for (auto modulePath : modulesPath) {
      _modulesPath.push_back(std::get<std::string>(modulePath.v));
    }
  }
  catch (...) {
    nz::Log::warning("modules_path not found or must be an array, default modules paths set to './Modules'", "Zia Core");
    _modulesPath.push_back("./Modules");
  }

  // Get module_net (string) from config file
  try { _debug = std::get<bool>(_conf.at("debug").v); }
  catch (...) {
    nz::Log::warning("debug not found or must be a boolean, default debug mode set to 'false'", "Zia Core");
    _debug = false;
  }

  // Get module_net (string) from config file
  try { _logLevel = std::get<long long>(_conf.at("log_level").v); }
  catch (...) {
    nz::Log::warning("log_level not found or must be a number, default log level set to '1'", "Zia Core");
    _logLevel = 1;
  }
}

void nz::zia::loadModules()
{
  Log::inform("Configuration loading ...");
  loadConf();
  Log::inform("Modules loading ...");
  _modulesLoader.loadAll();

  Log::inform("Network loading ...");
  loadNetwork();
}

void nz::zia::loadNetwork()
{
  std::string filename = _moduleNet;
#if defined (_WIN32) || defined (_WIN64)
  filename += ".dll";
#elif defined (__linux__) || defined (__APPLE__)
  filename += ".so";
#endif

  for (auto path : _modulesPath)
    {
      for (auto& p : std::experimental::filesystem::directory_iterator(path))
	{
	  if (std::experimental::filesystem::is_regular_file(p)
	      && std::experimental::filesystem::path(p).filename() == filename)
	    {
	      _dlLoaderNet.addLib(std::experimental::filesystem::path(p).string());
	      _net = _dlLoaderNet.getInstance(std::experimental::filesystem::path(p).string());
	      _moduleNetPath = std::experimental::filesystem::path(p).string();
	      _dlLoaderNet.dump();
	      break ;
	    }
	}
    }
  if (_net == nullptr) {
      nz::Log::error("Fail load net module", "Zia Core", 3);
    }
  _parser.setNet(_net);
  _net->config(_conf);
}

nz::ModuleLoader &nz::zia::getModulesLoader()
{
  return _modulesLoader;
}

const nz::ModuleLoader& nz::zia::getModulesLoader() const
{
  return _modulesLoader;
}