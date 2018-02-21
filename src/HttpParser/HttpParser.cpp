//
// Made by Marco
//

#include "HttpParser.hh"

nz::HttpParser::HttpParser()
{
}

nz::HttpParser::~HttpParser()
{
}

zia::api::http::Method	nz::HttpParser::GetMethodFromString(const std::string & input)
{
	static const std::map<std::string, zia::api::http::Method> link =
	{
		{ "OPTIONS", zia::api::http::Method::options },
	{ "GET", zia::api::http::Method::get },
	{ "HEAD", zia::api::http::Method::head },
	{ "POST", zia::api::http::Method::post },
	{ "PUT", zia::api::http::Method::put },
	{ "DELETE", zia::api::http::Method::delete_ },
	{ "TRACE", zia::api::http::Method::trace },
	{ "CONNECT", zia::api::http::Method::connect }
	};

	for (auto it : link)
	{
		if (it.first == input)
			return it.second;
	}

	return zia::api::http::Method::unknown;
}

zia::api::http::Version	nz::HttpParser::GetVersionFromString(const std::string & input)
{
	static const std::map<std::string, zia::api::http::Version> link =
	{
		{ "HTTP/0.9", zia::api::http::Version::http_0_9 },
	{ "HTTP/1.0", zia::api::http::Version::http_1_0 },
	{ "HTTP/1.1", zia::api::http::Version::http_1_1 },
	{ "HTTP/2.0", zia::api::http::Version::http_2_0 }
	};

	for (auto it : link)
	{
		if (it.first == input)
			return it.second;
	}

	return zia::api::http::Version::unknown;
}

std::string		nz::HttpParser::GetStringFromVersion(const zia::api::http::Version & input)
{
	static const std::map<std::string, zia::api::http::Version> link =
	{
		{ "HTTP/0.9", zia::api::http::Version::http_0_9 },
	{ "HTTP/1.0", zia::api::http::Version::http_1_0 },
	{ "HTTP/1.1", zia::api::http::Version::http_1_1 },
	{ "HTTP/2.0", zia::api::http::Version::http_2_0 }
	};

	for (auto it : link)
	{
		if (it.second == input)
			return it.first;
	}
	throw nz::HttpParserException("Unknown Version");
	return "Unknown";
}

zia::api::HttpRequest nz::HttpParser::GetRequest(const std::vector<std::string> & input)
{
	size_t pos = 0;

	// Create a variable for manipulation
	std::vector<std::string> data = input;

	// Initialize the output
	zia::api::HttpRequest output;
	output.method = zia::api::http::Method::unknown;
	output.uri = "";
	output.version = zia::api::http::Version::unknown;
	output.headers = std::map<std::string, std::string>();
	output.body = transform::StringToRaw("");


	// Get the first line ==> contain the method, the uri and the http version
	if (data.size() > 0)
	{
		std::string fLine = data.at(0);
		data.erase(data.begin());

		std::vector<std::string> tokens;

		while ((pos = fLine.find(SP)) != std::string::npos)
		{
			tokens.push_back(fLine.substr(0, pos));
			fLine = fLine.substr(pos + std::strlen(SP));
		}
		tokens.push_back(fLine.substr(0, pos));

		// Get the method of the request
		if (tokens.size() > 0)
			output.method = GetMethodFromString(tokens.at(0));
		// Get the uri of the request
		if (tokens.size() > 1)
			output.uri = tokens.at(1);
		// Get the version of the request
		if (tokens.size() > 2)
			output.version = GetVersionFromString(tokens.at(2));
	}

	// Parse the header to find all the fields
	while (data.size() && data.at(0) != "")
	{
		// If the fields name doesn't contains ':' at the end, ignore it
		if ((pos = data.at(0).find(":")) != std::string::npos)
		{
			std::string key = data.at(0).substr(0, pos);
			data.at(0) = data.at(0).substr(pos + 1);
			std::string value = data.at(0);
			output.headers[transform::EpurStr(key)] = transform::EpurStr(value);
		}
		data.erase(data.begin());
	}

	// Find the body
	std::string body;
	while (data.size())
	{
		body += data.at(0) + CRLF;
		data.erase(data.begin());
	}
	output.body = transform::StringToRaw(transform::EpurStr(body));

	return output;
}

zia::api::HttpResponse	nz::HttpParser::GetResponse(const zia::api::Net::Raw & input)
{
	return GetResponse(transform::RawToString(input));
}

zia::api::HttpResponse	nz::HttpParser::GetResponse(const std::string & input)
{
	std::vector<std::string> row = transform::Split(transform::EpurStr(input, CRLF), CRLF);
	return GetResponse(row);
}

zia::api::Net::Raw		nz::HttpParser::ResponseToRaw(const zia ::api::HttpResponse &input)
{
	zia::api::Net::Raw	output;
	std::string			tmp;

	CheckResponseValidity(input);

	tmp += GetStringFromVersion(input.version) + SP;
	tmp += std::to_string(input.status) + SP;
	tmp += input.reason + CRLF;
	for (auto it : input.headers)
		tmp += it.first + ":" + SP + it.second + CRLF;
	tmp += CRLF;
	tmp += transform::RawToString(input.body);

	output = transform::StringToRaw(tmp);
	return output;
}

zia::api::HttpResponse nz::HttpParser::GetResponse(const std::vector<std::string> & input)
{
	size_t pos = 0;

	// Create a variable for manipulation
	std::vector<std::string> data = input;

	// Initialize the output
	zia::api::HttpResponse output;
	output.reason = "";
	output.status = zia::api::http::common_status::unknown;
	output.version = zia::api::http::Version::unknown;
	output.headers = std::map<std::string, std::string>();
	output.body = transform::StringToRaw("");

	// Get the first line ==> contain the http version, the status code and the reason
	if (data.size() > 0)
	{
		std::string fLine = data.at(0);
		data.erase(data.begin());

		std::vector<std::string> tokens;

		while ((pos = fLine.find(SP)) != std::string::npos)
		{
			tokens.push_back(fLine.substr(0, pos));
			fLine = fLine.substr(pos + std::strlen(SP));
		}
		tokens.push_back(fLine.substr(0, pos));

		// Get the version of the reponse
		if (tokens.size() > 0)
			output.version = GetVersionFromString(tokens.at(0));
		// Get the status of the response
		if (tokens.size() > 1)
			output.status = std::stoi(tokens.at(1));
		// Get the reason of the response
		if (tokens.size() > 2)
			output.reason = tokens.at(2);
	}

	// Parse the header to find all the fields
	while (data.size() && data.at(0) != "")
	{
		// If the fields name doesn't contains ':' at the end, ignore it
		if ((pos = data.at(0).find(":")) != std::string::npos)
		{
			std::string key = data.at(0).substr(0, pos);
			data.at(0) = data.at(0).substr(pos + 1);
			std::string value = data.at(0);
			output.headers[transform::EpurStr(key)] = transform::EpurStr(value);
		}
		data.erase(data.begin());
	}

	// Find the body
	std::string body;
	while (data.size())
	{
		body += data.at(0) + CRLF;
		data.erase(data.begin());
	}
	output.body = transform::StringToRaw(transform::EpurStr(body));

	return output;
}

zia::api::HttpDuplex	nz::HttpParser::Parse(const zia::api::Net::Raw & raw)
{
	zia::api::HttpDuplex back;
	std::string request = transform::RawToString(raw);
	std::vector<std::string> row = transform::Split(transform::EpurStr(request, CRLF), CRLF);

	back.raw_req = raw;
	back.req = GetRequest(row);

	CheckRequestValidity(back.req);

	return back;
}

void					nz::HttpParser::CheckRequestValidity(zia::api::HttpRequest data)
{
	// Check Method
	if (data.method == zia::api::http::Method::unknown)
		throw HttpParserException("Unknow Method");

	// Check Uri
	if (data.uri == "")
		throw HttpParserException("Empty uri");

	// Check Version
	if (data.version == zia::api::http::Version::unknown)
		throw HttpParserException("Unknow Version");

	// Check Headers

	// Check Body
	if (!data.body.empty())
	{
		if (data.method == zia::api::http::Method::head
			|| data.method == zia::api::http::Method::delete_
			|| data.method == zia::api::http::Method::trace)
			throw HttpParserException("This method doesn't require a body");
	}
	else
	{
		if (data.method == zia::api::http::Method::post
			|| data.method == zia::api::http::Method::put
			|| data.method == zia::api::http::Method::connect)
			throw HttpParserException("This method require a body");
	}
}

void					nz::HttpParser::CheckResponseValidity(zia::api::HttpResponse data)
{
	// Check Status
	if (data.status == zia::api::http::common_status::unknown)
		throw HttpParserException("Unknow Status");

	// Check Version
	if (data.version == zia::api::http::Version::unknown)
		throw HttpParserException("Unknow Version");

	// Check Headers

	// Check Body
}
