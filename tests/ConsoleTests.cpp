#include "gtest/gtest.h"

#include "../src/Console/Console.hh"

TEST(Console, ConsoleHelp)
{
  nz::zia Zia;
  nz::Console console(Zia);
  std::string cmd("help");

  testing::internal::CaptureStdout();
  console.runCmd(cmd);
  std::string output = testing::internal::GetCapturedStdout();
  ASSERT_STREQ(output.c_str(), "List of available commands:\n\tmodules add <module name>\n\tmodules remove <module name>\n\tnetwork set <module name>\n\tmodules list\n\tnetwork reload\n\thelp\n\tstart\n\tstop\n\treload\n");
}

TEST(Console, ConsoleStart) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("start");

    console.runCmd(cmd);
}

TEST(Console, ConsoleStop) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("stop");

    console.runCmd(cmd);
}

TEST(Console, ConsoleReload) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("reload");

    console.runCmd(cmd);
}

TEST(Console, ConsoleNetworkReloadNoModulesNetLoaded) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("network reload");

    console.runCmd(cmd);
}

TEST(Console, ConsoleModulesAdd) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("modules add cpp_zia_module_php");

    console.runCmd(cmd);

    // testing::internal::CaptureStdout();
    // cmd = "modules list";
    // console.runCmd(cmd);
    // std::string output = testing::internal::GetCapturedStdout();
    // ASSERT_STREQ(output.c_str(), "List of loaded modules \n");
}

TEST(Console, ConsoleModulesRemove) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("modules remove cpp_zia_module_php");

    console.runCmd(cmd);

    // testing::internal::CaptureStdout();
    // cmd = "modules list";
    // console.runCmd(cmd);
    // std::string output = testing::internal::GetCapturedStdout();
    // ASSERT_STREQ(output.c_str(), "List of loaded modules \n");
}

TEST(Console, ConsoleNetworkSet) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("network set cpp_zia_module_network");

    // console.runCmd(cmd);
}

TEST(Console, ConsoleModulesListEmpty) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("modules list");

    testing::internal::CaptureStdout();
    console.runCmd(cmd);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_STREQ(output.c_str(), "List of loaded modules\n");
}

TEST(Console, ConsoleModulesListLoaded) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("start");

    console.runCmd(cmd);
    cmd = "modules list";
    testing::internal::CaptureStdout();
    console.runCmd(cmd);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_STREQ(output.c_str(), "List of loaded modules\n./Modules/cpp_zia_module_router.so\n");
}

TEST(Console, ConsoleBadCmdName) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("yes42man");

    testing::internal::CaptureStdout();
    console.runCmd(cmd);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_STREQ(output.c_str(), "Invalid command\n");
}

TEST(Console, ConsoleBadCmdNbArgs) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("yes42man yes42man yes42man yes42man yes42man yes42man yes42man");

    testing::internal::CaptureStdout();
    console.runCmd(cmd);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_STREQ(output.c_str(), "Invalid command\n");
}