#include "gtest/gtest.h"

#include "../src/ModuleLoader/ModuleLoader.hh"

TEST(ModuleLoader, ModuleLoader) {
    std::vector<std::string> v;
    nz::ModuleLoader            moduleLoader(v, v);
    nz::ModuleLoader const      moduleLoaderConst(v, v);

    moduleLoader.loadAll();
    moduleLoader.unloadAll();
    moduleLoader.addModule("A_New_Module");
    moduleLoader.deleteModuleByName("A_New_Module");

    nz::DLLoader<zia::api::Module> const& dlLoaderConst = moduleLoaderConst.getDlLoader();
    nz::DLLoader<zia::api::Module> & dlLoader = moduleLoader.getDlLoader();
}