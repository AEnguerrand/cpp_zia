#include "ModuleLoader.hh"

nz::ModuleLoader::ModuleLoader(std::vector<std::string> &modulesName, std::vector<std::string> &modulesPath,
			       const ::zia::api::Conf &conf) :
	_modulesName(modulesName),
	_modulesPath(modulesPath),
	_conf(conf),
	_dlLoader("modulesZia", true)
{
}

nz::ModuleLoader::~ModuleLoader()
{
}

void nz::ModuleLoader::loadAll()
{
  for (auto moduleName : _modulesName)
    {
      addModule(moduleName);
    }
}

void nz::ModuleLoader::addModule(const std::string &moduleName)
{
  std::string moduleFilename = convertToFilename(moduleName);
  for (auto path : _modulesPath)
    {
      for (auto &p : std::experimental::filesystem::directory_iterator(path))
	{
	  if (std::experimental::filesystem::is_regular_file(p)
	      && std::experimental::filesystem::path(p).filename() == moduleFilename)
	    {
	      _dlLoader.addLib(std::experimental::filesystem::path(p).string());
	      auto tmp = _dlLoader.getInstance(std::experimental::filesystem::path(p).string());
	      tmp->config(_conf);
	      _modules[moduleName] = std::experimental::filesystem::path(p).string();
	      _dlLoader.dump();
	      return;
	    }
	}
    }
}

bool nz::ModuleLoader::deleteModuleByName(const std::string &moduleName)
{
  try
    {
      _dlLoader.destroyLib(_modules.at(moduleName));
      _modules.erase(_modules.find(moduleName));
      _modulesName.erase(std::find(_modulesName.begin(), _modulesName.end(), moduleName));
    }
  catch (std::exception e)
    {
      std::cerr << "Error: " << e.what() << std::endl;
      return false;
    }
  return true;
}

bool nz::ModuleLoader::unloadAll()
{
  for (auto module : _dlLoader.getInstances())
    {
      _dlLoader.destroyLib(module.first);
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
  return _dlLoader.getInstances();
}

const std::string nz::ModuleLoader::convertToFilename(const std::string &moduleName) const
{
  std::string filename = moduleName;

#if defined (_WIN32) || defined (_WIN64)
  filename += ".dll";
#elif defined (__linux__) || defined (__APPLE__)
  filename += ".so";
#endif
  return filename;
}
