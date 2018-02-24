#include <iostream>
#include "Module.hpp"

using namespace nzm;

#if defined(_MSC_VER)

extern "C"
{
	__declspec(dllexport) Module *create()
	{
		return new Module();
	}
}

#elif defined(__GNUC__) || defined(__GNUG__)

extern "C"
{
	Module *create()
	{
		return new Module();
	}
}

#endif

Module::Module() {}

Module::~Module() {}

bool Module::config(const zia::api::Conf& conf){
    return true;
}

bool Module::exec(zia::api::HttpDuplex& http){
    std::cout << "PHP exec" << std::endl;
    return true;
}
