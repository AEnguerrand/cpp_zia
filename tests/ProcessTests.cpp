#include "gtest/gtest.h"

#include "../src/HttpParser/HttpParser.hh"
#include "../src/ModuleLoader/ModuleLoader.hh"
#include "../src/Process/Process.hh"

TEST(Process, StartProcess) {
    std::vector<std::string> names = {"cpp_zia_module_router", "cpp_zia_module_php"};
    std::vector<std::string> paths = {"./Modules"};
    ::zia::api::Conf conf;

    nz::ModuleLoader            moduleLoader(names, paths, conf);
    moduleLoader.loadAll();
    ASSERT_EQ(moduleLoader.getModules().size(), 2);

    nz::Process         process(moduleLoader);
    zia::api::Net::Raw  request = transform::StringToRaw("GET /index.html HTTP/1.1 \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive");
    nz::HttpParser      parser;

    zia::api::HttpDuplex duplex = parser.Parse(request);

    process.startProcess(duplex);
}