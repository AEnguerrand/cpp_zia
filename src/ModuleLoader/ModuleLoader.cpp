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
  std::for_each(_modulesName.begin(), _modulesName.end(), [&](std::string& name){ addModule(name);  });
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
        _modules[moduleName] = std::experimental::filesystem::path(p).string(); 
	      _dlLoader.addLib(_modules[moduleName]);
	      auto tmp = _dlLoader.getInstance(_modules[moduleName]);
	      tmp->config(_conf);
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
      nz::Log::error(e.what(), "MODULE LOADER", 7);
      return false;
    }
  return true;
}

bool nz::ModuleLoader::unloadAll()
{
  std::for_each(
    _dlLoader.getInstances().begin(),
    _dlLoader.getInstances().end(),
    [&](auto module) { _dlLoader.destroyLib(module.first); }
  );
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
