#include "gtest/gtest.h"

#include "../src/zia.hh"

TEST(Zia, ZiaNotYetStarted) {
    nz::zia Zia;
    std::string output;

    // Zia not yet started, trying to reload
    testing::internal::CaptureStderr();
    Zia.reload();
    output = testing::internal::GetCapturedStderr();
    ASSERT_STREQ(output.c_str(), "[ERROR 1] - Zia Core: The server isn't started yet\n");
    // Zia not yet started, trying to stop
    testing::internal::CaptureStderr();
    Zia.stop();
    output = testing::internal::GetCapturedStderr();
    ASSERT_STREQ(output.c_str(), "[ERROR 1] - Zia Core: The server isn't started yet\n");
}

TEST(Zia, ZiaAlreadyStarted) {
    nz::zia Zia;

    Zia.start();
    testing::internal::CaptureStderr();
    Zia.start();
    std::string output = testing::internal::GetCapturedStderr();
    ASSERT_STREQ(output.c_str(), "[ERROR 2] - Zia Core: The server's already started, please consider stopping it before trying again\n");
}

TEST(Zia, ZiaBadNetworkModule) {
    // nz::zia Zia;

    // Zia.start();
    // testing::internal::CaptureStderr();
    // Zia.setModuleNetwork("moduleNet");
    // std::string output = testing::internal::GetCapturedStderr();
    // ASSERT_STREQ(output.c_str(), "_> About to delete instance of [./Modules/cpp_zia_module_network.so]\n./Modules/cpp_zia_module_network.so: undefined symbol: DObject\n[ERROR 3] - Zia Core: Fail load net module\n");
}

TEST(Zia, ZiaNormalBehaviour) {
    nz::zia Zia;
    std::string output;

    Zia.start();
    Zia.reload();

    nz::ModuleLoader const& moduleLoader = Zia.getModulesLoader();

    Zia.reloadNetwork();
}