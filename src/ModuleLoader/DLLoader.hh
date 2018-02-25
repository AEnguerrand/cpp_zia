#ifndef CPP_ZIA_DLLOADER_HH
#define CPP_ZIA_DLLOADER_HH

#ifndef _WIN32

#include <string>
#include <memory>
#include <iomanip>
#include <iostream>
#include <dlfcn.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

namespace nz
{
  template<typename T>
  class DLLoader
  {
   private:
    std::vector<std::string> libs;
    std::unordered_map<std::string, void *> handlers;
    std::unordered_map<std::string, T *> instances;
    std::string name;
    bool noise;

   public:
    DLLoader(const std::string &name = "anonymous", int noise = false)
    {
      this->name = name;
      this->noise = noise;

      if (noise)
	{
	  std::cerr << "_> Construction of DLLoader (" << this->name << ")" << std::endl;
	}
    }

    void addLib(const std::string &path)
    {
      void *handler;

      if (std::find(this->libs.begin(), this->libs.end(), path) != this->libs.end())
	{
	  if (this->noise)
	    {
	      std::cerr << "_> \"" << path << "\" is already loaded in this context" << std::endl;
	    }
	  return;
	}

      if (this->noise)
	{
	  std::cerr << "_> Adding new lib in (" << this->name << ") [" << path << "]" << std::endl;
	}

      if ((handler = dlopen(path.c_str(), RTLD_LAZY)) == nullptr)
	{
	  std::cerr << dlerror() << std::endl;
	}
      else
	{
	  this->handlers[path] = handler;
	  this->instances[path] = nullptr;
	  this->libs.push_back(path);
	}
    }

    std::vector<std::string> getPaths() const
    { return (this->libs); }

    std::unordered_map<std::string, T *> getInstances() const
    { return this->instances; }

    T *resetLib(const std::string &ist)
    {

      std::unordered_map<std::string, T *> ists = this->instances;
      T *nIst = nullptr;

      if (noise)
	{
	  std::cerr << "_> " << ist << " is being reset..." << '\n';
	}

      this->deleteInstance(ist);
      nIst = this->getInstance(ist);

      if (noise)
	{
	  std::cerr << "<_ " << ((nIst == nullptr) ? "ERROR" : "SUCCESS") << '\n';
	}

      return nIst;
    }

    std::string getPathByInstance(T *ist) const
    {
      std::unordered_map<std::string, T *> ists = this->instances;
      std::string res = "";

      for (auto it = ists.begin(); it != ists.end(); it++)
	{
	  if (it->second == ist) res = it->first;
	}
      return res;
    }

    void dump() const
    {
      int paddingVarName, currSize, idx;

      if (this->noise)
	{
	  paddingVarName = idx = currSize = 0;

	  std::cerr << "_> Dumping libraries for the current context:" << std::endl;
	  std::cerr << "_> There is " << this->libs.size() << " library in (" << this->name << ")" << std::endl;
	  // Get Padding
	  for (auto it = this->libs.begin(); it != this->libs.end(); it++)
	    {
	      currSize = (*it).size();
	      if (currSize > paddingVarName) paddingVarName = currSize;
	    }
	  // Display table header
	  for (int i = 0; i < paddingVarName + 17; i++) std::cerr << "-";
	  std::cerr << "|" << std::endl << "|  " << std::setw(10) << "ID" << " | " << std::setw(paddingVarName) << " Path" << " |"
		    << std::endl;
	  // Display (header / content) separator
	  for (int i = 0; i < paddingVarName + 17; i++) std::cerr << "-";
	  std::cerr << "|" << std::endl;
	  // Display content
	  for (auto it = this->libs.begin(); it != this->libs.end(); it++)
	    {
	      std::cerr << "|  " << std::setw(10) << idx << " | " << std::setw(paddingVarName) << (*it) << " |" << std::endl;
	      idx += 1;
	    }
	  // Display footer
	  for (int i = 0; i < paddingVarName + 17; i++) std::cerr << "-";
	  std::cerr << "|" << std::endl;
	}
    }

    T *getInstance(const std::string &path)
    {
      void *handler;
      T *(*symbol)();

      // If there is already an instance of $path, then return it
      if (this->instances[path])
	{
	  return this->instances[path];
	}

      // Else, create one
      handler = this->handlers[path];

      if (this->noise)
	std::cerr << "_> Creating new instance of [" << path << "] in (" << this->name << ")..." << std::endl;

      if ((symbol = reinterpret_cast<T *(*)()>(dlsym(handler, "create"))) == nullptr)
	{
	  this->handlers.erase(path);
	  std::cerr << dlerror() << std::endl;
	  return nullptr;
	}

      this->instances[path] = symbol();

      if (this->noise)
	std::cerr << "_> Instance successfully created!" << std::endl;

      // And return it
      return this->instances[path];
    }

    void deleteInstance(const std::string &path)
    {
      void *handler;
      T *(*symbol)(T *);

      if (noise)
	{
	  std::cerr << "_> About to delete instance of [" << path << "]" << std::endl;
	}

      // If there is no handler for this path, then stop the current action
      if (this->handlers.find(path) == this->handlers.end())
	{
	  return;
	}
      else
	{
	  handler = this->handlers[path];
	}

      // If there is no instance for this path, then stop the current action
      if (this->instances[path] == nullptr)
	{
	  if (noise)
	    {
	      std::cerr << "_> No instance of [" << path << "] found, skipping..." << std::endl;
	    }

	  return;
	}

      if ((symbol = reinterpret_cast<T *(*)(T *)>(dlsym(handler, "DObject"))) == nullptr)
	{
	  this->instances.erase(this->instances.find(path));
	  return;
	}

      symbol(this->instances[path]);
      this->instances.erase(this->instances.find(path));

      if (noise)
	{
	  std::cerr << "_> instance of [" << path << "] successfully destroyed" << std::endl;
	}
    }

    void destroyLib(const std::string &path)
    {
      this->deleteInstance(path);
      this->libs.erase(std::find(this->libs.begin(), this->libs.end(), path));

      if (this->handlers.find(path) == this->handlers.end())
	{
	  return;
	}
      else
	{
	  if (dlclose(this->handlers[path]))
	    {
	      std::cerr << dlerror() << std::endl;
	    }
	}
    }

    ~DLLoader()
    {
      if (this->noise)
	{
	  std::cerr << "_> Destruction of DLLoader (" << this->name << ")" << std::endl;
	}

      for (auto it = this->libs.begin(); it != this->libs.end(); it++)
	{
	  this->deleteInstance((*it));

	  if (this->handlers.find((*it)) == this->handlers.end())
	    {
	      return;
	    }
	  else
	    {
	      if (dlclose(this->handlers[(*it)]))
		{
		  std::cerr << dlerror() << std::endl;
		}
	    }
	}

      if (this->noise)
	{
	  std::cerr << "_> DLLoader (" << this->name << ") successfully destroyed" << std::endl;
	}
    }
  };
}

#endif

#endif //CPP_ZIA_DLLOADER_HH
