#include "ModuleLoader.hh"

nz::ModuleLoader::ModuleLoader():
	dlLoader("modulesZia", true)
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
	  this->dlLoader.addLib(std::experimental::filesystem::path(p));
	}
    }
  this->dlLoader.dump();
}

void nz::ModuleLoader::unloadAll()
{
  for (auto lib : this->dlLoader.getPaths()) {
      this->dlLoader.resetLib(lib);
    }
}