#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "Module.hpp"

using namespace nzm;

bool PHPerror;
std::string cgiContent;
bool isPhp;
std::string body = "";

extern "C" Module* create(){
    return new Module();
}

Module::Module() {}

Module::~Module() {}

bool Module::config(const zia::api::Conf& conf){
    return true;
}

bool Module::exec(zia::api::HttpDuplex& http){
    return true;
}