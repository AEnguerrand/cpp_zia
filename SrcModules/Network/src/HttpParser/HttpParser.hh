//
// Made by Marco
//

#ifndef HTTP_PARSER_HPP_
# define HTTP_PARSER_HPP_

#include <iostream>
#include <cstring>
#include <vector>
#include "HttpParserException.hpp"
#include "Transform.hpp"
#include "http.h"

#define CRLF "\r\n"
#define SP " "

namespace nz
{
	class HttpParser
	{
	public:
		HttpParser();
		virtual ~HttpParser();

	private:
		zia::api::HttpRequest	GetRequest(const std::vector<std::string> & input);
		zia::api::HttpResponse	GetResponse(const std::vector<std::string> & input);

	private:
		zia::api::http::Method	GetMethodFromString(const std::string & input);
		zia::api::http::Version GetVersionFromString(const std::string & input);
		std::string				GetStringFromVersion(const zia::api::http::Version & input);
		void					CheckValidity(zia::api::HttpRequest);

	public:
		zia::api::HttpDuplex	Parse(const zia::api::Net::Raw & request);
		zia::api::HttpResponse	GetResponse(const std::string & input);
		zia::api::HttpResponse	GetResponse(const zia::api::Net::Raw & input);
		zia::api::Net::Raw		ResponseToRaw(const zia::api::HttpResponse &input);
	};
}

#endif //HTTP_PARSER_HPP_