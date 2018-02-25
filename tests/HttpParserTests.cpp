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
    zia::api::Net::Raw responseRaw = parser.ResponseToRaw(response);
    std::string responseStr = transform::RawToString(responseRaw);

    ASSERT_STREQ(responseStr.c_str(), "HTTP/1.1 200 OK\r\nCache - Control: no - cache, no - store, must - revalidate, max - age = 0, proxy - revalidate, no - transform, private\r\nConnection: close\r\nContent - Type: text / xml\r\nDate: Wed, 13 May 2009 16 : 26 : 47 GMT\r\nETag: \"4A0AF4C7-08E1-37C7F492\"\r\nExpires: Tue, 12 May 2009 16 : 26 : 47 GMT\r\nLast - Modified: Thu, 14 May 2009 16 : 26 : 47 GMT\r\nP3P: policyref = \"/w3c/p3p.xml\", CP = \"NOI DSP COR NID PSA OUR IND COM NAV STA\"\r\nPragma: no - cache\r\nVary: *\r\nX - C: ms - 3.7.2\r\nxserver: www79\r\n\r\n< ? xml version = \"1.0\" encoding = \"UTF-8\" ? > <status>SUCCESS< / status>");
}

TEST(HttpParser, HttpParserBadMethod) {
    nz::HttpParser parser;

    //Bad method
    zia::api::Net::Raw request = transform::StringToRaw("GET_ \r\nUser-Agent Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive");
    try
    {
        parser.Parse(request);
        ASSERT_TRUE(false);
        
    }
    catch (nz::HttpParserException e)
    {
        ASSERT_TRUE(true);
    }
}

TEST(HttpParser, HttpParserEmptyURI) {
    nz::HttpParser parser;
    
    // Empty URI
    zia::api::Net::Raw request = transform::StringToRaw("GET \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive");
    try
    {
        parser.Parse(request);
        ASSERT_TRUE(false);
        
    }
    catch (nz::HttpParserException e)
    {
        ASSERT_TRUE(true);
    }
}

TEST(HttpParser, HttpParserBadVersion) {
    nz::HttpParser parser;

    // Bad version
    zia::api::Net::Raw request = transform::StringToRaw("GET /index.html HTTP/100.1 \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive");
    try
    {
        parser.Parse(request);
        ASSERT_TRUE(false);
    }
    catch (nz::HttpParserException e)
    {
        ASSERT_TRUE(true);
    }
}

TEST(HttpParser, HttpParserBodyShouldBeEmpty) {
    nz::HttpParser parser;

    // Body should be empty
    zia::api::Net::Raw request = transform::StringToRaw("TRACE /index.html HTTP/1.1 \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n<html></html>");
    try
    {
        parser.Parse(request);
        ASSERT_TRUE(false);
        
    }
    catch (nz::HttpParserException e)
    {
        ASSERT_TRUE(true);
    }
}

TEST(HttpParser, HttpParserBodyShouldNotBeEmpty) {
    nz::HttpParser parser;

    // Body should not be empty
    zia::api::Net::Raw request = transform::StringToRaw("CONNECT /index.html HTTP/1.1 \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive");
    try
    {
        parser.Parse(request);
        ASSERT_TRUE(false);
        
    }
    catch (nz::HttpParserException e)
    {
        ASSERT_TRUE(true);
    }
}

TEST(HttpParser, HttpParserException) {
    nz::HttpParserException e("Test construction for the HttpParserException class");

    ASSERT_STREQ(e.what(), "Test construction for the HttpParserException class");
}

TEST(HttpParser, Transform) {
    std::vector<std::string> a = {"One", "Two", "Three", "Four", "Five"};

    std::string joinedStr = transform::VectorToString(a);
    ASSERT_STREQ(joinedStr.c_str(), "One Two Three Four Five");
}