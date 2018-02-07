#include "Network.hh"

bool nzm::Network::config(const zia::api::Conf &conf)
{
  return false;
}

bool nzm::Network::run(zia::api::Net::Callback cb)
{
  return false;
}

bool nzm::Network::send(zia::api::ImplSocket *sock, const zia::api::Net::Raw &resp)
{
  return false;
}

bool nzm::Network::stop()
{
  return false;
}

nzm::Network::Network()
{}
