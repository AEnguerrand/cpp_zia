#include "Console.hh"

nz::Console::Console(nz::zia & zia):
	_zia(zia)
{
}

nz::Console::~Console()
{
}

void nz::Console::run()
{
  Log::inform("Console is avaible");
  for (std::string line; std::getline(std::cin, line);)
    {
      this->runCmd(line);
    }
}

void nz::Console::runCmd(std::string & cmd)
{
  if (cmd == "start")
    {
      this->_zia.start();
    }
  else if (cmd == "stop")
    {
      this->_zia.stop();
    }
  else if (cmd == "reload")
    {
      this->_zia.reload();
    }
}


