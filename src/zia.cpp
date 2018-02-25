#include "zia.hh"

nz::zia::zia():
  _isStart(false),
  _process(_modulesLoader),
  _parser(_process, nullptr),
  _modulesLoader(_modules, _modulesPath, _conf),
  _dlLoaderNet("moduleNet", false)
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
  Log::inform("Server booting ...");
  loadModules();

  _isStart = true;
}
void nz::zia::stop()
{
  if (!_isStart)
    {
      Log::error("The server isn't started yet", "Zia Core", 1);
      return ;
    }
  Log::inform("Server halt ...");
  _modulesLoader.unloadAll();

  unloadNetwork();

  _conf.clear();
  _modules.clear();
  _modulesPath.clear();

  _isStart = false;
}

void nz::zia::reload()
{
  if (!_isStart)
    {
      Log::error("The server isn't started yet", "Zia Core", 1);
      return ;
    }
  Log::inform("The server's reloading, please wait...");
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
  Log::inform("Configuration loading ...");
  loadConf();

  Log::inform("Modules loading ...");
  try { _modulesLoader.loadAll(); }
  catch (...) {
	  Log::inform("Modules loading failed.");
  }
  Log::inform("Modules loaded successfully.");

  Log::inform("Network loading ...");
  try { loadNetwork(); }
  catch (...) {
	  Log::inform("Network loading failed.");
  }
  Log::inform("Network loaded successfully.");
}

void nz::zia::loadNetwork()
{
  std::string filename = _moduleNet;
#if defined (_WIN32) || defined (_WIN64)
  filename += ".dll";
#elif defined (__linux__) || defined (__APPLE__)
  filename += ".so";
#endif

  _net = nullptr;
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
  if (_net == nullptr)
    {
      nz::Log::error("Fail load net module", "Zia Core", 3);
      return ;
    }
  _parser.setNet(_net);
  _net->config(_conf);

  ::zia::api::Net::Callback funcCallback = std::bind(&nz::Parser::callbackRequestReceived, _parser, std::placeholders::_1,
						     std::placeholders::_2);
  _net->run(funcCallback);
  Log::inform("Network is run");
}

nz::ModuleLoader &nz::zia::getModulesLoader()
{
  return _modulesLoader;
}

void nz::zia::unloadNetwork()
{
  if (_net == nullptr)
    {
      nz::Log::error("No module net is load", "Zia Core", 4);
      if (!_moduleNetPath.empty())
	{
	  _dlLoaderNet.destroyLib(_moduleNetPath);
	}
      _moduleNet.clear();
    }
  else
    {
      _net->stop();
      _dlLoaderNet.destroyLib(_moduleNetPath);

      _net = nullptr;
      _moduleNet.clear();
      _moduleNetPath.clear();
    }
}

void nz::zia::setModuleNetwork(const std::string & moduleNet)
{
  unloadNetwork();
  _moduleNet = moduleNet;
  loadNetwork();
}

void nz::zia::reloadNetwork()
{
  if (_moduleNet.empty())
    {
      nz::Log::error("No module net is load", "Zia Core", 4);
      return ;
    }
  auto tmp = _moduleNet;
  unloadNetwork();
  _moduleNet = tmp;
  loadNetwork();
}

const nz::ModuleLoader& nz::zia::getModulesLoader() const
{
  return _modulesLoader;
}