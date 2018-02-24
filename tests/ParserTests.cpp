#include "gtest/gtest.h"

#include "../src/Parser/Parser.hh"
#include "../src/Parser/ParserJson.hh"
#include "../src/ModuleLoader/ModuleLoader.hh"
#include "../src/Process/Process.hh"

TEST(Parser, Parser) {
    std::vector<std::string> v;
    ::zia::api::Conf conf;
    nz::ModuleLoader    moduleLoader(v, v, conf);
    moduleLoader.loadAll();
    ASSERT_EQ(moduleLoader.getModules().size(), 0);

    nz::Process         process(moduleLoader);
    nz::Parser          parser(process, nullptr);

    ::zia::api::Net::Raw raw = transform::StringToRaw("GET /index.html HTTP/1.1 \nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \nHost: www.tutorialspoint.com\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive");
    ::zia::api::NetInfo netInfo;

    netInfo.sock = nullptr;
    ASSERT_EQ(netInfo.sock, nullptr);

    parser.setNet(nullptr);
    parser.callbackRequestReceived(raw, netInfo);
}

TEST(Parser, ParserJsonIncorrectFile) {
    std::string output;

    //Wrong file
    nz::ParserJson parseWrongJson("../conf/config.azdqsd");
    parseWrongJson.getConfig();
    testing::internal::CaptureStdout();
    parseWrongJson.dump();
    output = testing::internal::GetCapturedStdout();
    ASSERT_STREQ(output.c_str(), "Configuration file is empty\n");
}

TEST(Parser, ParserJsonCorrectFile) {
    std::string output;

    //Correct file
    nz::ParserJson parserJson("../tests/conf/config.json");
    parserJson.getConfig();
    testing::internal::CaptureStdout();
    parserJson.dump();
    output = testing::internal::GetCapturedStdout();
    ASSERT_STREQ(output.c_str(), "{\n    \"debug\": false,\n    \"log_level\": 2,\n    \"module_net\": \"network_with_ssh\",\n    \"modules\": [\n        \"cgibin\",\n        \"gzip\",\n        \"logger\"\n    ],\n    \"modules_path\": [\n        \".\",\n        \"modules\",\n        3\n    ],\n    \"port\": 80,\n    \"port_ssl\": 443\n}\n");
}