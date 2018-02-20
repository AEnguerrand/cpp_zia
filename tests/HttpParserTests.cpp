#include "gtest/gtest.h"

#include "../src/HttpParser/HttpParser.hh"

TEST(HttpParser, Request) {
	std::cout << "------------------ TEST - GET - HEADER ONLY ------------------" << std::endl
    nz::HttpParser  parser;
    
    zia::api::Net::Raw request = transform::StringToRaw("GET /index.html HTTP/1.1 " + CRLF + "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) " + CRLF + "Host: www.tutorialspoint.com" + CRLF + "Accept-Language: en-us" + CRLF + "Accept-Encoding: gzip, deflate" + CRLF + "Connection: Keep-Alive" + CRLF + CRLF);

    zia::api::HttpDuplex duplex = parser.Parse(request);

	EXPECT_EQ(duplex.req.method, zia::api::http::Method::get);
    ASSERT_STREQ(duplex.req.uri.c_str(), "/index.html");
	EXPECT_EQ(duplex.req.version, zia::api::http::Version::http_1_1);
	ASSERT_STREQ(duplex.req.headers["Connection"], "Keep-Alive");
	ASSERT_STREQ(transform::RawToString(duplex.req.body), "");
	std::cout << "------------------ END ------------------" << std::endl
}

TEST(HttpParser, Request) {
	std::cout << "------------------ TEST - GET - HEADER AND BODY ------------------" << std::endl
	nz::HttpParser  parser;

	zia::api::Net::Raw request = transform::StringToRaw("GET /index.html HTTP/1.1 " + CRLF + "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) " + CRLF + "Host: www.tutorialspoint.com" + CRLF + "Accept-Language: en-us" + CRLF + "Accept-Encoding: gzip, deflate" + CRLF + "Connection: Keep-Alive" + CRLF + CRLF + "<html><p>Enguerrand a eu -42</p></html>");

	zia::api::HttpDuplex duplex = parser.Parse(request);

	EXPECT_EQ(duplex.req.method, zia::api::http::Method::get);
	ASSERT_STREQ(duplex.req.uri.c_str(), "/index.html");
	EXPECT_EQ(duplex.req.version, zia::api::http::Version::http_1_1);
	ASSERT_STREQ(duplex.req.headers["Connection"], "Keep-Alive");
	ASSERT_STREQ(transform::RawToString(duplex.req.body), "");
	std::cout << "------------------ END ------------------" << std::endl
}
