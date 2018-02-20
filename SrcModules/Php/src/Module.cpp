#include <iostream>
#include "Module.hpp"

using namespace nzm;

extern "C" zia::api::Module* Module::create(){
    return new Module();
}

Module::Module() {}

Module::~Module() {}

bool Module::config(const zia::api::Conf& conf){
    return true;
}

bool Module::exec(zia::api::HttpDuplex& http){
    std::cout << "PHP HERE" << std::endl;
    return true;
}
