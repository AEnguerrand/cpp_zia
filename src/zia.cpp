#include "zia.hh"

nz::zia::zia():
	_process(_modulesLoader),
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
  ::zia::api::Net *net = this->_dlLoaderNet.getInstance("./Modules/cpp_zia_module_network.so");
  if (net == nullptr) {
      nz::Log::error("Fail load net module", "Zia Core", 1);
    }
  // Config network
  ::zia::api::Conf confNetwork;
  net->config(confNetwork);
  // Run network
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
