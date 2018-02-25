#ifndef CPP_ZIA_CONSOLE_HH
#define CPP_ZIA_CONSOLE_HH

#include <iostream>
#include <iomanip>

#include "../zia.hh"
#include "../HttpParser/Transform.hpp"
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
    void displayHelp();
    void dispatchCommand(const std::vector<std::string>&);
    void invalidCommand();
    void displayModulesList();
  };
}

#endif //CPP_ZIA_CONSOLE_HH
