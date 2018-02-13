#include "ModuleLoader.hh"

nz::ModuleLoader::ModuleLoader():
	_dlLoader("modulesZia", true)
{

}

nz::ModuleLoader::~ModuleLoader()
{

}

void nz::ModuleLoader::loadAll()
{
  for (auto& p: std::experimental::filesystem::directory_iterator("./Modules"))
    {
      if (std::experimental::filesystem::is_regular_file(p) && std::experimental::filesystem::path(p).extension() == ".so")
	{
	  if (std::experimental::filesystem::path(p).filename() != "cpp_zia_module_network.so") {
	      this->_dlLoader.addLib(std::experimental::filesystem::path(p));
	    }
	}
    }
  this->_dlLoader.dump();
}

void nz::ModuleLoader::unloadAll()
{
  for (auto lib : this->_dlLoader.getPaths()) {
      this->_dlLoader.resetLib(lib);
    }
}

const nz::DLLoader<zia::api::Module> &nz::ModuleLoader::getDlLoader() const
{
  return _dlLoader;
}

nz::DLLoader<zia::api::Module> &nz::ModuleLoader::getDlLoader()
{
  return _dlLoader;
}

std::unordered_map<std::string, ::zia::api::Module *> nz::ModuleLoader::getModules()
{
  return this->_dlLoader.getInstances();
}
