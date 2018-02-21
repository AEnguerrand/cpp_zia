#include "gtest/gtest.h"

#include "../src/ModuleLoader/ModuleLoader.hh"

TEST(ModuleLoader, ModuleLoader) {
    nz::ModuleLoader            moduleLoader;
    nz::ModuleLoader const      moduleLoaderConst;

    moduleLoader.loadAll();
    moduleLoader.unloadAll();

    nz::DLLoader<zia::api::Module> const& dlLoaderConst = moduleLoaderConst.getDlLoader();
    nz::DLLoader<zia::api::Module> & dlLoader = moduleLoader.getDlLoader();
}