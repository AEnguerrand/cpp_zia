#include "ModuleLoader.hh"

nz::ModuleLoader::ModuleLoader()
{

}

nz::ModuleLoader::~ModuleLoader()
{

}

void nz::ModuleLoader::loadAll()
{
  for(auto& p: std::experimental::filesystem::directory_iterator("./Modules"))
    std::cout << p << '\n';
}

void nz::ModuleLoader::unloadAll()
{

}
