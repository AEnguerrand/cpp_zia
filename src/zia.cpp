#include "zia.hh"

nz::zia::zia():
  _process(_modulesLoader),
  _parser(_process, nullptr),
  _modulesLoader(_modules, _modulesPath, this->_conf),
  _dlLoaderNet("moduleNet", true)
{
}

nz::zia::~zia()
{
  this->stop();
}

void nz::zia::start()
{
  Log::inform("Server booting ...");
  this->loadModules();

  // Run network
  ::zia::api::Net::Callback funcCallback = std::bind(&nz::Parser::callbackRequestReceived, this->_parser, std::placeholders::_1,
						     std::placeholders::_2);
  this->_net->run(funcCallback);

  // Todo: Remove it by run of console
  // for (std::string line; std::getline(std::cin, line);) {
  //     std::cout << line << std::endl;
  //   }
  std::chrono::seconds d(60);
  while (true) {
    std::this_thread::sleep_for(d);
  }
}
void nz::zia::stop()
{
  Log::inform("Server halt ...");
  this->_modulesLoader.unloadAll();
}

void nz::zia::reload()
{
  Log::inform("Server reload ...");
  this->_modulesLoader.unloadAll();
  this->_modulesLoader.loadAll();
}

void  nz::zia::loadConf()
{
  ParserJson        parser("../conf/config.json");
  this->_conf = parser.getConfig();

  // Get port (number) from config file
  /*try { this->_port = std::get<long long>(config["port"].v); }
  catch (std::bad_variant_access&) {
    nz::Log::warning("port not found or must be a number, default port set to '80'", "Zia Core", 100);
    this->_port = 80;
  }*/

  // Get port_ssl (number) from config file
  /*try { this->_portSsl = std::get<long long>(config["port_ssl"].v); }
  catch (std::bad_variant_access&) {
    nz::Log::warning("port_ssl not found or must be a number, default ssl port set to '443'", "Zia Core", 100);
    this->_port = 443;
  }*/

  // Get module_net (string) from config file
  try { this->_moduleNet = std::get<std::string>(this->_conf["module_net"].v); }
  catch (std::bad_variant_access&) {
    nz::Log::warning("module_net not found or must be a string, default module network set to 'cpp_zia_module_network'", "Zia Core", 100);
    this->_moduleNet = "cpp_zia_module_network";
  }

  // Get modules (array) from config file
  try {
    ::zia::api::ConfArray modules = std::get<::zia::api::ConfArray>(this->_conf["modules"].v);
    for (auto module : modules) {
      this->_modules.push_back(std::get<std::string>(module.v));
    }
  }
  catch (std::bad_variant_access&) {
    nz::Log::warning("modules not found or must be an array, default modules set to 'cpp_zia_module_router'", "Zia Core", 100);
    this->_modules.push_back("cpp_zia_module_router");
  }

  // Get modules_path (array) from config file
  try {
    ::zia::api::ConfArray modulesPath = std::get<::zia::api::ConfArray>(this->_conf["modules_path"].v);
    for (auto modulePath : modulesPath) {
      this->_modulesPath.push_back(std::get<std::string>(modulePath.v));
    }
  }
  catch (std::bad_variant_access&) {
    nz::Log::warning("modules_path not found or must be an array, default modules paths set to './Modules'", "Zia Core", 100);
    this->_modulesPath.push_back("./Modules");
  }

  // Get module_net (string) from config file
  try { this->_debug = std::get<bool>(this->_conf["debug"].v); }
  catch (std::bad_variant_access&) {
    nz::Log::warning("debug not found or must be a boolean, default debug mode set to 'false'", "Zia Core", 100);
    this->_debug = false;
  }

  // Get module_net (string) from config file
  try { this->_logLevel = std::get<long long>(this->_conf["log_level"].v); }
  catch (std::bad_variant_access&) {
    nz::Log::warning("log_level not found or must be a number, default log level set to '1'", "Zia Core", 100);
    this->_logLevel = 1;
  }
}

void nz::zia::loadModules()
{
  nz::Log::inform("Configuration loading ...");
  this->loadConf();
  nz::Log::inform("Modules loading ...");
  this->_modulesLoader.loadAll();

  nz::Log::inform("Network loading ...");
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
  if (this->_net == nullptr) {
      nz::Log::error("Fail load net module", "Zia Core", 1);
    }
  this->_parser.setNet(this->_net);
  this->_net->config(this->_conf);
}
