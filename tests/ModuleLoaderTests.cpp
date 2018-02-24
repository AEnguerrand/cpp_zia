#include "gtest/gtest.h"

#include "../src/ModuleLoader/ModuleLoader.hh"
#include "../src/ModuleLoader/DLLoader.hh"

TEST(ModuleLoader, ModuleLoader) {
    std::vector<std::string> names = {"cpp_zia_module_router", "cpp_zia_module_php", "cpp_zia_module_router"};
    std::vector<std::string> paths = {"./Modules"};
    ::zia::api::Conf conf;
    nz::ModuleLoader            moduleLoader(names, paths, conf);
    nz::ModuleLoader const      moduleLoaderConst(names, paths, conf);

    moduleLoader.loadAll();
    moduleLoader.unloadAll();

    //Wrong module
    moduleLoader.addModule("A_New_Module");
    moduleLoader.deleteModuleByName("A_New_Module");

    nz::DLLoader<zia::api::Module> const& dlLoaderConst = moduleLoaderConst.getDlLoader();
    nz::DLLoader<zia::api::Module> & dlLoader = moduleLoader.getDlLoader();
}