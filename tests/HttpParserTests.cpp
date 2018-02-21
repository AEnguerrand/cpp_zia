#include "gtest/gtest.h"

#include "../src/HttpParser/HttpParser.hh"

TEST(HttpParser, Request) {
    nz::HttpParser  parser;
    
    zia::api::Net::Raw request = transform::StringToRaw("GET /index.html HTTP/1.1 \nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \nHost: www.tutorialspoint.com\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive");

    zia::api::HttpDuplex duplex = parser.Parse(request);

    EXPECT_EQ(duplex.req.method, zia::api::http::Method::get);
    ASSERT_STREQ(duplex.req.uri.c_str(), "/index.html");
}