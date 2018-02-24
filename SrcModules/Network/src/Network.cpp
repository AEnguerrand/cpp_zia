#include "Network.hh"
#include "Select.hh"

// Create instance

#if defined(_MSC_VER)

extern "C"
{
	__declspec(dllexport) zia::api::Net * create()
	{
		return new(nzm::Network);
	}
}

#elif defined(__GNUC__) || defined(__GNUG__)

extern "C"
{
	zia::api::Net * create()
	{
		return new(nzm::Network);
	}
}

#endif


bool nzm::Network::config(const zia::api::Conf &conf)
{
  nz::Log::debug("CONFIG NETWORK LOADING");
  return false;
}

bool nzm::Network::run(zia::api::Net::Callback cb)
{
  auto funcRunSelect = std::bind(&nzm::Network::runSelect, this, std::placeholders::_1, std::placeholders::_2);

  this->_select = std::make_shared<std::thread>(funcRunSelect, 7000, cb);

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
  while (true) {
      select.run();
    }
}
