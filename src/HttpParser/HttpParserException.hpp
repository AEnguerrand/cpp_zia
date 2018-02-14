//
// Made by Marco
//

#ifndef CPP_ZIA_HTTP_PARSER_EXCEPTION_HH
# define CPP_ZIA_HTTP_PARSER_EXCEPTION_HH

#include <iostream>

namespace nz {
	class HttpParserException : public std::exception
	{
	protected:
		std::string _msg;

	public:
		explicit  HttpParserException(const std::string& msg) : _msg(msg) {}
		virtual   ~HttpParserException(void) throw() {}

		virtual const char* what() const throw() { return this->_msg.c_str(); }
	};
}

#endif //CPP_ZIA_HTTP_PARSER_EXCEPTION_HH
