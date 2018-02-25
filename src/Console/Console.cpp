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
				{{"modules", "list"}, [&](auto&){ displayModulesList(); }},
				{{"network", "reload"}, [&](auto&){ _zia.reloadNetwork(); }},
			}
		},
		{
			3,
			{
				{{"modules", "add", "<arg>"}, [&](auto& args){ _zia.getModulesLoader().addModule(args.at(2)); }},
				{{"modules", "remove", "<arg>"}, [&](auto& args){ _zia.getModulesLoader().deleteModuleByName(args.at(2)); }},
				{{"network", "set", "<arg>"}, [&](auto& args){ _zia.setModuleNetwork(args.at(2)); }}
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

	bool found = false;
	for (auto& commandInfos : commands)
	{
		bool same = true;
		std::vector<std::string> cmd = commandInfos.first;
		for (uint32_t i = 0; i < size; ++i)
		{
			if ((cmd.at(i).front() != '<' && cmd.at(i).back() != '>') && args.at(i) != cmd.at(i))
				same = false;
		}
		if (same)
		{
			commandInfos.second(args);
			found = true;
		}
	}
	if (!found)
		invalidCommand();
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
			nz::Log::print("\t" + join(commandInfos.first, " "));
	}
}

void nz::Console::displayModulesList()
{
		auto Modules = _zia.getModulesLoader().getModules();
	  nz::Log::print("List of loaded modules");
	  for (auto module : Modules)
			nz::Log::print(module.first);
}

std::string nz::Console::join(std::vector<std::string> v, std::string j) {
	std::string result = "";
	int count = 0;
	int size = v.size();
	for (auto it = v.begin(); it != v.end(); it++) {
		result += *it;
		if (count < size - 1)
		result += j;
		count++;
	}
	return result;
}
