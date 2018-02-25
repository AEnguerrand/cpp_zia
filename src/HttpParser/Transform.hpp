//
// Made by Marco
//

#ifndef TRANSFORM_HPP_
# define TRANSFORM_HPP_

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "net.h"

namespace transform
{
	static zia::api::Net::Raw StringToRaw(std::string input)
	{
		zia::api::Net::Raw output;
		std::transform(input.begin(), input.end(), std::back_inserter(output),
			[](unsigned char c) -> std::byte { return static_cast<std::byte>(c); });
		return output;
	}

	static std::string RawToString(zia::api::Net::Raw input)
	{
		std::string output;
		std::transform(input.begin(), input.end(), std::back_inserter(output),
			[](std::byte c) -> unsigned char { return static_cast<unsigned char>(c); });
		return output;
	}

	static std::vector<std::string>	Split(const std::string & input, const std::string & delimiter)
	{
		std::vector<std::string> output;
		std::string tmp = input;
		std::string token;
		size_t pos = 0;

		while ((pos = tmp.find(delimiter)) != std::string::npos)
		{
			token = tmp.substr(0, pos);
			output.push_back(token);
			tmp.erase(0, pos + delimiter.length());
		}
		output.push_back(tmp);
		return output;
	}

	static std::string	EpurStr(const std::string & input)
	{
		std::string tmp;
		std::string output;
		std::istringstream iss(input);

		if (iss >> tmp)
			output += tmp;
		while (iss >> tmp)
			output += ' ' + tmp;
		return output;
	}

	static std::string	EpurStr(const std::string & input, const std::string & save)
	{
		auto data = Split(input, save);
		std::string output;

		for (auto it : data)
			output += EpurStr(it) + save;
		return output;
	}

	static std::string	VectorToString(const std::vector<std::string> & input)
	{
		std::string output;

		for (auto& it : input)
			output += it + ' ';

		return output.substr(0, output.size() - 1);
	}
}

#endif // TRANSFORM_HPP_