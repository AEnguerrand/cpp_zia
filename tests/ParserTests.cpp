#include "gtest/gtest.h"

#include "../src/Parser/Parser.hh"
#include "../src/Parser/ParserJson.hh"
#include "../src/Parser/ParserJsonException.hpp"
#include "../src/ModuleLoader/ModuleLoader.hh"
#include "../src/Process/Process.hh"

TEST(Parser, Parser) {
    nz::ModuleLoader    moduleLoader;
    moduleLoader.loadAll();

    nz::Process         process(moduleLoader);
    nz::Parser          parser(process, nullptr);

    ::zia::api::Net::Raw raw = transform::StringToRaw("GET /index.html HTTP/1.1 \nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \nHost: www.tutorialspoint.com\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive");
    ::zia::api::NetInfo netInfo;

    netInfo.sock = nullptr;

    parser.callbackRequestReceived(raw, netInfo);
}

TEST(Parser, ParserJson) {
    //Wrong file
    try
    {
        nz::ParserJson parserJson("../conf/config.azdqsd");
    }
    catch (nz::ParserJsonException e)
    {

    }

    //Correct file
    nz::ParserJson parserJson("../tests/conf/config.json");

    parserJson.getConfig();
    parserJson.dump();
}

TEST(Parser, ParserJsonException) {
    nz::ParserJsonException e("Test construction for the ParserJsonException class");

    std::cout << e.what() << std::endl;
}