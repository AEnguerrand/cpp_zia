#include "zia.hh"

nz::zia::zia():
  _process(_modulesLoader),
  _parser(_process, nullptr),
  _modulesLoader(_modules, _modulesPath, this->_conf),
  _dlLoaderNet("moduleNet", true)
{
  this->_isStart = false;
}

nz::zia::~zia()
{
  this->stop();
}

void nz::zia::start()
{
  if (this->_isStart)
    {
      Log::error("Server is already start, please stop it before", "Zia Core", 2);
      return ;
    }
  Log::inform("Server booting ...");
  this->loadModules();

  this->_isStart = true;
}
void nz::zia::stop()
{
  if (!this->_isStart)
    {
      Log::error("Server is not start", "Zia Core", 1);
      return ;
    }
  Log::inform("Server halt ...");
  this->_modulesLoader.unloadAll();

  this->unloadNetwork();

  this->_conf.clear();
  this->_modules.clear();
  this->_modulesPath.clear();

  this->_isStart = false;
}

void nz::zia::reload()
{
  if (!this->_isStart)
    {
      Log::error("Server is not start", "Zia Core", 1);
      return ;
    }
  Log::inform("Server reload ...");
  this->stop();
  this->start();
}

void  nz::zia::loadConf()
{
  ParserJson        parser("../conf/config.json");
  this->_conf = parser.getConfig();

  // Get module_net (string) from config file
  try { this->_moduleNet = std::get<std::string>(this->_conf.at("module_net").v); }
  catch (...) {
    nz::Log::warning("module_net not found or must be a string, default module network set to 'cpp_zia_module_network'", "Zia Core");
    this->_moduleNet = "cpp_zia_module_network";
  }

  // Get modules (array) from config file
  try {
    ::zia::api::ConfArray modules = std::get<::zia::api::ConfArray>(this->_conf.at("modules").v);
    for (auto module : modules) {
      this->_modules.push_back(std::get<std::string>(module.v));
    }
  }
  catch (...) {
    nz::Log::warning("modules not found or must be an array, default modules set to 'cpp_zia_module_router'", "Zia Core");
    this->_modules.push_back("cpp_zia_module_router");
  }

  // Get modules_path (array) from config file
  try {
    ::zia::api::ConfArray modulesPath = std::get<::zia::api::ConfArray>(this->_conf.at("modules_path").v);
    for (auto modulePath : modulesPath) {
      this->_modulesPath.push_back(std::get<std::string>(modulePath.v));
    }
  }
  catch (...) {
    nz::Log::warning("modules_path not found or must be an array, default modules paths set to './Modules'", "Zia Core");
    this->_modulesPath.push_back("./Modules");
  }

  // Get module_net (string) from config file
  try { this->_debug = std::get<bool>(this->_conf.at("debug").v); }
  catch (...) {
    nz::Log::warning("debug not found or must be a boolean, default debug mode set to 'false'", "Zia Core");
    this->_debug = false;
  }

  // Get module_net (string) from config file
  try { this->_logLevel = std::get<long long>(this->_conf.at("log_level").v); }
  catch (...) {
    nz::Log::warning("log_level not found or must be a number, default log level set to '1'", "Zia Core");
    this->_logLevel = 1;
  }
}

void nz::zia::loadModules()
{
  Log::inform("Configuration loading ...");
  this->loadConf();
  Log::inform("Modules loading ...");
  this->_modulesLoader.loadAll();

  Log::inform("Network loading ...");
  this->loadNetwork();
}

void nz::zia::loadNetwork()
{
  std::string filename = this->_moduleNet;
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
	      this->_dlLoaderNet.addLib(std::experimental::filesystem::path(p).string());
	      this->_net = this->_dlLoaderNet.getInstance(std::experimental::filesystem::path(p).string());
	      this->_moduleNetPath = std::experimental::filesystem::path(p).string();
	      this->_dlLoaderNet.dump();
	      break ;
	    }
	}
    }
  if (this->_net == nullptr)
    {
      nz::Log::error("Fail load net module", "Zia Core", 3);
      return ;
    }
  this->_parser.setNet(this->_net);
  this->_net->config(this->_conf);

  ::zia::api::Net::Callback funcCallback = std::bind(&nz::Parser::callbackRequestReceived, this->_parser, std::placeholders::_1,
						     std::placeholders::_2);
  this->_net->run(funcCallback);
  Log::inform("Network is run");
}

nz::ModuleLoader &nz::zia::getModulesLoader()
{
  return this->_modulesLoader;
}

void nz::zia::unloadNetwork()
{
  if (this->_net == nullptr)
    {
      nz::Log::error("No module net is load", "Zia Core", 4);
      if (!this->_moduleNetPath.empty())
	{
	  this->_dlLoaderNet.destroyLib(this->_moduleNetPath);
	}
      this->_moduleNet.clear();
    }
  else
    {
      this->_net->stop();
      this->_dlLoaderNet.destroyLib(this->_moduleNetPath);

      this->_net = nullptr;
      this->_moduleNet.clear();
      this->_moduleNetPath.clear();
    }
}

void nz::zia::setModuleNetwork(std::string & moduleNet)
{
  this->unloadNetwork();
  this->_moduleNet = moduleNet;
  this->loadNetwork();
}

void nz::zia::reloadNetwork()
{
  auto tmp = this->_moduleNet;
  this->unloadNetwork();
  this->_moduleNet = tmp;
  this->loadNetwork();
}
