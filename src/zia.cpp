#include "zia.hh"

nz::zia::zia():
	_process(_modulesLoader),
	_parser(_process, nullptr),
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
  // Load server config
  // Load module
  this->_modulesLoader.loadAll();
  // Load config of each module
  // Load network
  this->_dlLoaderNet.addLib("./Modules/cpp_zia_module_network.so");
  this->_dlLoaderNet.dump();
  this->_net = this->_dlLoaderNet.getInstance("./Modules/cpp_zia_module_network.so");
  if (this->_net == nullptr) {
      nz::Log::error("Fail load net module", "Zia Core", 1);
    }
  this->_parser.setNet(this->_net);
  // Config network
  ::zia::api::Conf confNetwork;
  this->_net->config(confNetwork);
  // Run network
  ::zia::api::Net::Callback funcCallback = std::bind(&nz::Parser::callbackRequestReceived, this->_parser, std::placeholders::_1,
						     std::placeholders::_2);
  this->_net->run(funcCallback);

  while (1);
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
