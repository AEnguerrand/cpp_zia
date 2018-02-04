#include "zia.hh"

nz::zia::zia()
{
}

nz::zia::~zia()
{
  this->stop();
}

void nz::zia::start()
{
  Log::inform("Server booting ...");
  this->_modulesLoader.loadAll();
}

void nz::zia::stop()
{
  Log::inform("Server halt ...");
  this->_modulesLoader.unloadAll();
}

void nz::zia::reload()
{
  Log::inform("Server reload ...");
  this->_modulesLoader.unloadAll();
  this->_modulesLoader.loadAll();
}
