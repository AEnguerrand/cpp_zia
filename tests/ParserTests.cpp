#include "gtest/gtest.h"

#include "../src/Parser/Parser.hh"
#include "../src/Parser/ParserJson.hh"
#include "../src/ModuleLoader/ModuleLoader.hh"
#include "../src/Process/Process.hh"

TEST(Parser, Parser) {
    std::vector<std::string> v;
    nz::ModuleLoader    moduleLoader(v, v);
    moduleLoader.loadAll();

    nz::Process         process(moduleLoader);
    nz::Parser          parser(process, nullptr);

    ::zia::api::Net::Raw raw = transform::StringToRaw("GET /index.html HTTP/1.1 \nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \nHost: www.tutorialspoint.com\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive");
    ::zia::api::NetInfo netInfo;

    netInfo.sock = nullptr;

    // Fix incoming..
    // parser.callbackRequestReceived(raw, netInfo);
}

TEST(Parser, ParserJson) {
    //Wrong file
    nz::ParserJson parseWrongJson("../conf/config.azdqsd");
    parseWrongJson.getConfig();
    parseWrongJson.dump();

    //Correct file
    nz::ParserJson parserJson("../tests/conf/config.json");
    parserJson.getConfig();
    parserJson.dump();
}