#ifndef CPP_ZIA_BUFFER_HH
#define CPP_ZIA_BUFFER_HH

#include <list>
#include <vector>

namespace nzm {
  class Buffer
  {
   private:

    std::list<char>	_buffer;
   public:
    Buffer();

    void push(char data);
    char pop();

    bool hasHTTPRequest();

    std::vector<char> getHttpRequest();
  };
}

#endif //CPP_ZIA_BUFFER_HH
