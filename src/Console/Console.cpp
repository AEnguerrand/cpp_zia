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
  std::vector<std::string> args;
  std::istringstream iss(cmd);
  for(std::string s; iss >> s; )
    args.push_back(s);

  try
    {
      if (args.size() == 1 && args.at(0) == "help")
	{
	  std::cout << "List of command in Zia:" << std::endl;
	  std::cout << "start : Start Zia" << std::endl;
	  std::cout << "stop : Stop Zia" << std::endl;
	  std::cout << "reload : Reload Zia" << std::endl;
	  std::cout << "modules add <module name> : Add module to Zia" << std::endl;
	  std::cout << "modules remove <module name> : Remove module to Zia" << std::endl;
	  std::cout << "modules list : List module of Zia" << std::endl;
	  std::cout << "network set <module name> : Set module for network and reload network of Zia" << std::endl;
	  std::cout << "network reload : Reload network of Zia" << std::endl;
	  std::cout << "---------------------------------" << std::endl;
	}
      else if (args.size() == 1 && args.at(0) == "start")
	{
	  this->_zia.start();
	}
      else if (args.size() == 1 && args.at(0) == "stop")
	{
	  this->_zia.stop();
	}
      else if (args.size() == 1 && args.at(0) == "reload")
	{
	  this->_zia.reload();
	}
      else if (args.size() == 3 && args.at(0) == "modules" && args.at(1) == "add")
	{
	  this->_zia.getModulesLoader().addModule(args.at(2));
	}
      else if (args.size() == 3 && args.at(0) == "modules" && args.at(1) == "remove")
	{
	  this->_zia.getModulesLoader().deleteModuleByName(args.at(2));
	}
      else if (args.size() == 2 && args.at(0) == "modules" && args.at(1) == "list")
	{
	  auto Modules = this->_zia.getModulesLoader().getModules();
	  std::cout << "List of modules load:" << std::endl;
	  for (auto module : Modules)
	    {
	      std::cout << module.first << std::endl;
	    }
	  std::cout << "---------------------------------" << std::endl;
	}
      else if (args.size() == 3 && args.at(0) == "network" && args.at(1) == "set")
	{
	  this->_zia.setModuleNetwork(args.at(2));
	}
      else if (args.size() == 2 && args.at(0) == "network" && args.at(1) == "reload")
	{
	  this->_zia.reloadNetwork();
	}
      else
	{
	  std::cout << "Command is invalid / not define in Zia" << std::endl;
	}
    }
  catch (...)
    {
      Log::error("Console send cmd", "Zia Console", 101);
    }
}


