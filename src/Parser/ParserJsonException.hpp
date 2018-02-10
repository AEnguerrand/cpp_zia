#ifndef CPP_ZIA_PARSERJSONEXCEPTION_HH
#define CPP_ZIA_PARSERJSONEXCEPTION_HH

namespace nz {
  class ParserJsonException : public std::exception
  {
    protected:
      std::string _msg;
    
    public:
      explicit  ParserJsonException(const char* msg): _msg(msg) {}
      explicit  ParserJsonException(const std::string& msg): _msg(msg) {}
      virtual   ~ParserJsonException(void) throw() {}
              
      virtual const char* what() const throw() { return this->_msg.c_str(); }
  };
}

#endif //CPP_ZIA_PARSERJSONEXCEPTION_HH