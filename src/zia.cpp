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
}

void nz::zia::stop()
{
  Log::inform("Server halt ...");
}

void nz::zia::reload()
{
  Log::inform("Server reload ...");
}
