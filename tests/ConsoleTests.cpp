#include "gtest/gtest.h"

#include "../src/Console/Console.hh"

TEST(Console, ConsoleHelp) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("help");

    testing::internal::CaptureStdout();
    console.runCmd(cmd);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_STREQ(output.c_str(), "List of command in Zia:\n"
	    "start : Start Zia\n"
	    "stop : Stop Zia\n"
	    "reload : Reload Zia\n"
	    "modules add <module name> : Add module to Zia\n"
	    "modules remove <module name> : Remove module to Zia\n"
	    "modules list : List module of Zia\n"
	    "network set <module name> : Set module for network and reload network of Zia\n"
	    "network reload : Reload network of Zia\n"
	    "---------------------------------\n");
}

TEST(Console, ConsoleStart) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("start");

    // console.runCmd(cmd); // exception thrown ?
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

TEST(Console, ConsoleModulesAdd) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("modules add a_module");

    console.runCmd(cmd);
}

TEST(Console, ConsoleModulesRemove) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("modules remove a_module");

    console.runCmd(cmd);
}

TEST(Console, ConsoleModulesList) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("modules list");

    testing::internal::CaptureStdout();
    console.runCmd(cmd);
    std::string output = testing::internal::GetCapturedStdout();
    //Why no modules ?
    ASSERT_STREQ(output.c_str(), "List of modules load:\n---------------------------------\n");
}

TEST(Console, ConsoleWrongCmd) {
    nz::zia Zia;
    nz::Console console(Zia);
    std::string cmd("modules list a_module");

    testing::internal::CaptureStdout();
    console.runCmd(cmd);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_STREQ(output.c_str(), "Command is invalid / not define in Zia\n");
}