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
      Log::error("The server's already started, please consider stopping it before trying again", "Zia Core", 2);
      return ;
    }
  Log::inform("The server is booting...");
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
      Log::error("The server isn't started yet", "Zia Core", 1);
      return ;
    }
  Log::inform("Shuting down the server...");
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
      Log::error("The server isn't started yet", "Zia Core", 1);
      return ;
    }
  Log::inform("The server's rebooting, please wait...");
  stop();
  start();
}

void  nz::zia::loadConf()
{
  _conf = ParserJson("../conf/config.json").getConfig();

  // Get module_net (string) from config file
  try { _moduleNet = std::get<std::string>(_conf.at("module_net").v); }
  catch (...) {
    _moduleNet = "cpp_zia_module_network";
    nz::Log::warning("module_net not found or must be a string, the default network module has been set to '" + _moduleNet + "'", "Zia Core");
  }

  // Get modules (array) from config file
  try {
    ::zia::api::ConfArray modules = std::get<::zia::api::ConfArray>(_conf.at("modules").v);
    for (auto module : modules) {
      _modules.push_back(std::get<std::string>(module.v));
    }
  }
  catch (...) {
    _modules.push_back("cpp_zia_module_router");
    nz::Log::warning("modules not found or must be an array, the default module has been set to '" + _modules.back() + "'", "Zia Core");
  }

  // Get modules_path (array) from config file
  try {
    ::zia::api::ConfArray modulesPath = std::get<::zia::api::ConfArray>(_conf.at("modules_path").v);
    for (auto modulePath : modulesPath) {
      _modulesPath.push_back(std::get<std::string>(modulePath.v));
    }
  }
  catch (...) {
    _modulesPath.push_back("./Modules");
    nz::Log::warning("modules_path not found or must be an array, the default module path has been set to '" + _modulesPath.back() + "'", "Zia Core");
  }

  // Get module_net (string) from config file
  try { _debug = std::get<bool>(_conf.at("debug").v); }
  catch (...) {
    _debug = false;
    nz::Log::warning("debug not found or must be a boolean, debug mode set to 'false'", "Zia Core");
  }

  // Get module_net (string) from config file
  try { _logLevel = std::get<long long>(_conf.at("log_level").v); }
  catch (...) {
    _logLevel = 1;
    nz::Log::warning("log_level not found or must be a number, default log level set to '" + std::to_string(_logLevel) + "'", "Zia Core");
  }
}

void nz::zia::loadModules()
{
  Log::inform("Loading configuration...");
  loadConf();

  Log::inform("Loading modules...");
  _modulesLoader.loadAll();

  Log::inform("Loading network ...");
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

  for (auto& path : _modulesPath)
    {
      for (auto& p : std::experimental::filesystem::directory_iterator(path))
	{
	  if (std::experimental::filesystem::is_regular_file(p)
	      && std::experimental::filesystem::path(p).filename() == filename)
	    {
	      _moduleNetPath = std::experimental::filesystem::path(p).string();
	      _dlLoaderNet.addLib(_moduleNetPath);
	      _net = _dlLoaderNet.getInstance(_moduleNetPath);
	      _dlLoaderNet.dump();
	      break ;
	    }
	}
    }
  if (!_net) {
      nz::Log::error("Fail to load network module", "Zia Core", 3);
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