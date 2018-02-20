#include "../build/googletest-src/googletest/include/gtest/gtest.h"

#include "../src/HttpParser/HttpParser.hh"

TEST(HttpParser, RequestHeaderOnly) {
	std::cout << "------------------ TEST - GET - HEADER ONLY ------------------" << std::endl;
    nz::HttpParser  parser;
    
    zia::api::Net::Raw request = transform::StringToRaw("GET /index.html HTTP/1.1 " + std::string(CRLF) + "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) " + std::string(CRLF) + "Host: www.tutorialspoint.com" + std::string(CRLF) + "Accept-Language: en-us" + std::string(CRLF) + "Accept-Encoding: gzip, deflate" + std::string(CRLF) + "Connection: Keep-Alive" + std::string(CRLF) + std::string(CRLF));

    zia::api::HttpDuplex duplex = parser.Parse(request);

	EXPECT_EQ(duplex.req.method, zia::api::http::Method::get);
    ASSERT_STREQ(duplex.req.uri.c_str(), "/index.html");
	EXPECT_EQ(duplex.req.version, zia::api::http::Version::http_1_1);
	EXPECT_EQ(duplex.req.headers["Connection"], "Keep-Alive");
	EXPECT_EQ(transform::RawToString(duplex.req.body), "");
	std::cout << "------------------ END ------------------" << std::endl;
}

TEST(HttpParser, RequestHeaderAndBody) {
	std::cout << "------------------ TEST - GET - HEADER AND BODY ------------------" << std::endl;
	nz::HttpParser  parser;

	zia::api::Net::Raw request = transform::StringToRaw("GET /index.html HTTP/1.1 " + std::string(CRLF) + "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT) " + std::string(CRLF) + "Host: www.tutorialspoint.com" + std::string(CRLF) + "Accept-Language: en-us" + std::string(CRLF) + "Accept-Encoding: gzip, deflate" + std::string(CRLF) + "Connection: Keep-Alive" + std::string(CRLF) + std::string(CRLF) + "<html><p>Enguerrand a eu -42</p></html>");

	zia::api::HttpDuplex duplex = parser.Parse(request);

	EXPECT_EQ(duplex.req.method, zia::api::http::Method::get);
	ASSERT_STREQ(duplex.req.uri.c_str(), "/index.html");
	EXPECT_EQ(duplex.req.version, zia::api::http::Version::http_1_1);
	EXPECT_EQ(duplex.req.headers["Connection"], "Keep-Alive");
	EXPECT_EQ(transform::RawToString(duplex.req.body), "");
	std::cout << "------------------ END ------------------" << std::endl;
}
