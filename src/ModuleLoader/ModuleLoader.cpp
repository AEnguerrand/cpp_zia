#include "ModuleLoader.hh"

nz::ModuleLoader::ModuleLoader(std::vector<std::string>& modulesName, std::vector<std::string>& modulesPath):
  _modulesName(modulesName),
  _modulesPath(modulesPath),
	_dlLoader("modulesZia", true) {}

nz::ModuleLoader::~ModuleLoader() {}

void nz::ModuleLoader::loadAll()
{
  for (auto moduleName : _modulesName){
    #if defined (_WIN32) || defined (_WIN64)
      moduleName += ".dll";
    #elif defined (__linux__) || defined (__APPLE__)
      moduleName += ".so";
    #endif
    addModule(moduleName);
  }
}

void nz::ModuleLoader::addModule(const std::string& moduleName)
{
  for (auto path : _modulesPath)
  {
    for (auto& p : std::experimental::filesystem::directory_iterator(path))
    {
      if (std::experimental::filesystem::is_regular_file(p)
        && std::experimental::filesystem::path(p).filename() == moduleName)
      {
        this->_dlLoader.addLib(std::experimental::filesystem::path(p).string());
        this->_dlLoader.getInstance(std::experimental::filesystem::path(p).string());
        this->_modules[moduleName] = std::experimental::filesystem::path(p).string();
        this->_dlLoader.dump();
        return;
      }
    }
  }
}

bool nz::ModuleLoader::deleteModuleByName(const std::string& moduleName)
{
  try
  {
    this->_dlLoader.resetLib(this->_modules.at(moduleName));
    this->_modules.erase(this->_modules.find(moduleName));
    this->_modulesName.erase(std::find(this->_modulesName.begin(), this->_modulesName.end(), moduleName));
  }
  catch (std::exception e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return true;
  }
  return false;
}

bool nz::ModuleLoader::unloadAll()
{
  while (!_modulesName.empty()){
    if (deleteModuleByName(this->_modulesName.at(0)))
      return true;
  }
  return false;
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
