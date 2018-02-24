#include "Network.hh"
#include "Select.hh"

// Create instance

extern "C" zia::api::Net * create()
{
  return new(nzm::Network);
}

bool nzm::Network::config(const zia::api::Conf &conf)
{
  try { this->_port = std::get<long long>(conf.at("port").v); }
  catch (std::exception&) {
      nz::Log::warning("port not found or must be a number, default port set to '80'", "Module Network", 100);
      this->_port = 80;
    }
  return true;
}

bool nzm::Network::run(zia::api::Net::Callback cb)
{
  auto funcRunSelect = std::bind(&nzm::Network::runSelect, this, std::placeholders::_1, std::placeholders::_2);

  this->_select = std::make_shared<std::thread>(funcRunSelect, this->_port, cb);

  return false;
}

bool nzm::Network::send(zia::api::ImplSocket *sock, const zia::api::Net::Raw &resp)
{
  // TODO: use buffer for write
  auto socket = reinterpret_cast<Socket *>(sock);
  socket->getBufferOut().pushRaw(resp);
  return true;
}

bool nzm::Network::stop()
{
  this->_stop.store(true);
  return true;
}

nzm::Network::Network()
{
  this->_stop.store(false);
  nz::Log::inform("[Module Network]: Start");
}

nzm::Network::~Network()
{
  this->_stop.store(true);
  nz::Log::inform("[Module Network]: Stop");
}

void nzm::Network::runSelect(short port, zia::api::Net::Callback cb)
{
  std::shared_ptr<Socket> socketServer = std::make_shared<Socket>();
  Select select(cb, *this);

  std::cout << "PORT: " << port << std::endl;

  socketServer->initServer(port);
  select.addListenTunnels(socketServer);
  while (!this->_stop.load()) {
      select.run();
    }
}
