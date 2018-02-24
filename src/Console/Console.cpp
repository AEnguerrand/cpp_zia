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
      std::cout << line << std::endl;
    }
}


