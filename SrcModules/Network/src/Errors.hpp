#ifndef CPP_ZIA_ERRORS_HPP
#define CPP_ZIA_ERRORS_HPP

#include <string>

class ModuleNetworkException : public std::exception
{
 protected:
  std::string _msg;
 public:

  explicit ModuleNetworkException(const char* message): _msg(message) {}
  explicit ModuleNetworkException (const std::string& message): _msg(message) {}

  virtual ~ModuleNetworkException () throw () {}

  virtual const char* what() const throw () { return std::string("ModuleNetworkException: " + this->_msg).c_str(); }
};

#endif //CPP_ZIA_ERRORS_HPP
