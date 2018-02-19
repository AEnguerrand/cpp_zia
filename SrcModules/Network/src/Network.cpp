#include "Network.hh"
#include "Select.hh"

// Create instance

extern "C" zia::api::Net * create()
{
  return new(nzm::Network);
}

bool nzm::Network::config(const zia::api::Conf &conf)
{
  nz::Log::debug("CONFIG NETWORK LOADING");
  return false;
}

bool nzm::Network::run(zia::api::Net::Callback cb)
{
  //Todo: Check what thread is too too long
  auto funcRunSelect = std::bind(&nzm::Network::runSelect, this, std::placeholders::_1, std::placeholders::_2);
  //this->_thListen.insert(std::pair<short, std::shared_ptr<std::thread>>(7000, std::make_shared<std::thread>(funcRunSelect, 7000, cb)));

  /*std::future<void> selectStart(std::async(std::launch::async, [&](zia::api::Net::Callback cb) {
    this->runSelect(7000, cb);
  }, cb));

  selectStart.get();*/

  this->_select = std::make_shared<std::thread>(funcRunSelect, 7000, cb);

  //this->runSelect(7000, cb);
  // Todo: Not exit now
  return false;
}

bool nzm::Network::send(zia::api::ImplSocket *sock, const zia::api::Net::Raw &resp)
{
  // Todo: use buffer for write
  auto socket = reinterpret_cast<Socket *>(sock);
  socket->getBufferOut().pushRaw(resp);
  return true;
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

void nzm::Network::runSelect(short port, zia::api::Net::Callback cb)
{
  std::shared_ptr<Socket> socketServer = std::make_shared<Socket>();
  Select select(cb, *this);

  socketServer->initServer(port);
  select.addListenTunnels(socketServer);
  while (1) {
      select.run();
    }
}
