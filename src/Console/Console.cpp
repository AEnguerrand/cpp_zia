#include "Console.hh"

typedef std::vector<std::pair<std::vector<std::string>, std::function<void(const std::vector<std::string>&)>>> commands_infos_t;
typedef std::unordered_map<uint32_t, commands_infos_t> cli_router_t;

static cli_router_t commands_sg;

nz::Console::Console(nz::zia & zia):
	_zia(zia)
{
	commands_sg = 
	{
		{
			1, 
			{
				{{"help"}, [&](auto&){ displayHelp(); }},
				{{"start"}, [&](auto&){ _zia.start(); }},
				{{"stop"}, [&](auto&){ _zia.stop(); }},
				{{"reload"}, [&](auto&){ _zia.reload(); }}
			}
		},
		{
			2, 
			{
				{{"modules", "list"}, [&](auto&){ displayModulesList(); }}
			}
		},
		{
			3, 
			{
				{{"modules", "add", "<arg>"}, [&](auto& args){ _zia.getModulesLoader().addModule(args.at(2)); }},
				{{"modules", "remove", "<arg>"}, [&](auto& args){ _zia.getModulesLoader().deleteModuleByName(args.at(2)); }}
			}
		},
	};
}

nz::Console::~Console()
{
}

void nz::Console::run()
{
  Log::inform("Console is available");
  for (std::string line; std::getline(std::cin, line);)
      runCmd(line);
}

void nz::Console::runCmd(std::string& cmd)
{
  std::vector<std::string> args;
  std::istringstream iss(cmd);
  for (std::string s; iss >> s; )
    args.push_back(s);

	try
	{
		dispatchCommand(args);
	}
	catch (...)
	{
		Log::error("Error while computing cli command", "Zia Console", 101);
	}
}

void nz::Console::dispatchCommand(const std::vector<std::string>& args)
{
	uint32_t size = args.size();

	if (commands_sg.find(size) == commands_sg.end())
	{
		if (size) invalidCommand();
		return;
	}
	
	commands_infos_t& commands = commands_sg[size];

	for (auto& commandInfos : commands)
	{
		bool same = true;
		for (uint32_t i = 0; i < size; ++i)
		{
			if (commandInfos.first.at(i) != "<arg>" && args.at(i) != commandInfos.first.at(i))
				same = false;
		}
		if (same)
			commandInfos.second(args);
	}
}

void nz::Console::invalidCommand()
{
	Log::print("Invalid command");
}

void nz::Console::displayHelp()
{
	nz::Log::print("List of available commands:");
	for (auto& cps : commands_sg)
	{
		for (auto& commandInfos : cps.second)
			nz::Log::print("\t" + transform::Join(commandInfos.first, " "));
	}
}

void nz::Console::displayModulesList()
{
		auto Modules = _zia.getModulesLoader().getModules();
	  nz::Log::print("List of loaded modules");
	  for (auto module : Modules)
			nz::Log::print(module.first);
}

/*void nz::Console::runCmd(std::string & cmd)
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
	  std::cout << "---------------------------------" << std::endl;
	}
      else if (args.size() == 1 && args.at(0) == "start")
	{
	  _zia.start();
	}
      else if (args.size() == 1 && args.at(0) == "stop")
	{
	  _zia.stop();
	}
      else if (args.size() == 1 && args.at(0) == "reload")
	{
	  _zia.reload();
	}
      else if (args.size() == 3 && args.at(0) == "modules" && args.at(1) == "add")
	{
	  _zia.getModulesLoader().addModule(args.at(2));
	}
      else if (args.size() == 3 && args.at(0) == "modules" && args.at(1) == "remove")
	{
	  _zia.getModulesLoader().deleteModuleByName(args.at(2));
	}
      else if (args.size() == 2 && args.at(0) == "modules" && args.at(1) == "list")
	{
	  auto Modules = _zia.getModulesLoader().getModules();
	  std::cout << "List of modules load:" << std::endl;
	  for (auto module : Modules)
	    {
	      std::cout << module.first << std::endl;
	    }
	  std::cout << "---------------------------------" << std::endl;
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
}*/


