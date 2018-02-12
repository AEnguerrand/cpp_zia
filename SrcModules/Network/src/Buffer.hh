#ifndef CPP_ZIA_BUFFER_HH
#define CPP_ZIA_BUFFER_HH

#include <list>
#include <vector>

#include "api.h"

namespace nzm {
  class Buffer
  {
   private:

    std::list<char>	_buffer;
   public:
    Buffer();

    void push(char data);
    char pop();

    bool hasHTTPRequest() const;
    zia::api::Net::Raw getHttpRequest();

    bool hasHTTPResponse() const;
    zia::api::Net::Raw getHttpResponse();
  };
}

#endif //CPP_ZIA_BUFFER_HH
