#include <iostream>
#include "Module.hpp"

using namespace nzm;

extern "C" Module *create()
{
  return new Module();
}

Module::Module()
{}

Module::~Module()
{}

bool Module::config(const zia::api::Conf &conf)
{
  return true;
}

bool Module::exec(zia::api::HttpDuplex &http)
{
  this->_router.execRouting(http);
  return true;
}
