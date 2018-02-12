#ifndef CPP_ZIA_BUFFER_HH
#define CPP_ZIA_BUFFER_HH

#include <list>

namespace nzm {
  class Buffer
  {
   private:
    unsigned int	_size;
    unsigned int	_head;
    unsigned int 	_tail;

    std::list<char>	_buffer;
   public:
    Buffer();

    void push(char data);
    char pop();
  };
}

#endif //CPP_ZIA_BUFFER_HH
