#include "gtest/gtest.h"

#include "../src/HttpParser/HttpParser.hh"
#include "../src/HttpParser/HttpParserException.hpp"

TEST(HttpParser, Request) {
    nz::HttpParser  parser;
    
    zia::api::Net::Raw request = transform::StringToRaw("GET /index.html HTTP/1.1 \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive");
    zia::api::Net::Raw responseSample = transform::StringToRaw("HTTP/1.1 200 OK\r\n\
        Date: Wed, 13 May 2009 16 : 26 : 47 GMT\r\n\
        X - C : ms - 3.7.2\r\n\
        Expires : Tue, 12 May 2009 16 : 26 : 47 GMT\r\n\
        Last - Modified : Thu, 14 May 2009 16 : 26 : 47 GMT\r\n\
        Cache - Control : no - cache, no - store, must - revalidate, max - age = 0, proxy - revalidate, no - transform, private\r\n\
        Pragma : no - cache\r\n\
        ETag : \"4A0AF4C7-08E1-37C7F492\"\r\n\
        Vary : *\r\n\
        P3P : policyref = \"/w3c/p3p.xml\", CP = \"NOI DSP COR NID PSA OUR IND COM NAV STA\"\r\n\
        xserver : www79\r\n\
        Connection : close\r\n\
        Content - Type : text / xml\r\n\r\n\
        < ? xml version = \"1.0\" encoding = \"UTF-8\" ? >\r\n\
        <status>SUCCESS< / status>\r\n");

    zia::api::HttpDuplex duplex = parser.Parse(request);

    EXPECT_EQ(duplex.req.method, zia::api::http::Method::get);
    ASSERT_STREQ(duplex.req.uri.c_str(), "/index.html");

    zia::api::HttpResponse response = parser.GetResponse(responseSample);
    parser.ResponseToRaw(response);
}

TEST(HttpParser, HttpParser) {
    nz::HttpParser parser;

    //Bad method
    zia::api::Net::Raw request = transform::StringToRaw("GET_ \r\nUser-Agent Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive");
    try
    {
        parser.Parse(request);
        
    }
    catch (nz::HttpParserException e)
    {

    }

    // Empty URI
    zia::api::Net::Raw request2 = transform::StringToRaw("GET \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive");
    try
    {
        parser.Parse(request2);
        
    }
    catch (nz::HttpParserException e)
    {

    }

    // Bad version
    zia::api::Net::Raw request3 = transform::StringToRaw("GET /index.html HTTP/100.1 \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive");
    try
    {
        parser.Parse(request3);
        
    }
    catch (nz::HttpParserException e)
    {

    }

    // Body should be empty
    zia::api::Net::Raw request4 = transform::StringToRaw("TRACE /index.html HTTP/1.1 \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n<html></html>");
    try
    {
        parser.Parse(request4);
        
    }
    catch (nz::HttpParserException e)
    {

    }

    // Body should not be empty
    zia::api::Net::Raw request5 = transform::StringToRaw("CONNECT /index.html HTTP/1.1 \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive");
    try
    {
        parser.Parse(request5);
        
    }
    catch (nz::HttpParserException e)
    {

    }
}

TEST(HttpParser, HttpParserException) {
    nz::HttpParserException e("Test construction for the HttpParserException class");

    std::cout << e.what() << std::endl;
}

TEST(HttpParser, Transform) {
    std::vector<std::string> a = {"One", "Two", "Three", "Four", "Five"};

    std::string joinedStr = transform::VectorToString(a);
    std::cout << joinedStr << std::endl;
}