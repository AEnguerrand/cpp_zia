#include "Network.hh"

bool nzm::Network::config(const zia::api::Conf &conf)
{
  nz::Log::debug("CONFIG NETWORK LOADING");
  return false;
}

bool nzm::Network::run(zia::api::Net::Callback cb)
{
  auto funcRunSelect = std::bind(&nzm::Network::runSelect, this, std::placeholders::_1);

  this->_thListen.insert(std::pair<short, std::shared_ptr<std::thread>>(7000, std::make_shared<std::thread>(funcRunSelect, 7000)));

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
{
  nz::Log::inform("[Module Network]: Run");
}

nzm::Network::~Network()
{
  nz::Log::inform("[Module Network]: Stop");
}

void nzm::Network::runSelect(short port)
{
  nz::Log::debug("AFTER CTOR");
  auto socketServer = std::make_shared<Socket>();
  Select select;

  socketServer->initServer(port);
  select.addListenTunnels(socketServer);
  while (1) {
      select.run();
    }
}
