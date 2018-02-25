#ifndef CPP_ZIA_CONSOLE_HH
#define CPP_ZIA_CONSOLE_HH

#include <iostream>
#include <iomanip>

#include "../zia.hh"
#include "Log.hpp"

namespace nz {
  class Console
  {
   public:
    zia &	_zia;

   public:
    Console(zia & zia);

    virtual ~Console();

    void	run();
    void	runCmd(std::string &);
  };
}

#endif //CPP_ZIA_CONSOLE_HH
