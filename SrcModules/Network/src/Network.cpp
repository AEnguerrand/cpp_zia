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

nzm::Network::Network()
{
  this->_isRun = false;
  this->_stop.store(false);
  nz::Log::inform("[Module Network]: Start");
}

nzm::Network::~Network()
{
  this->stop();
  nz::Log::inform("[Module Network]: Stop");
}

bool nzm::Network::config(const zia::api::Conf &conf)
{
  try { this->_port = std::get<long long>(conf.at("port").v); }
  catch (...) {
      nz::Log::warning("port not found or must be a number, default port set to '80'", "Module Network");
      this->_port = 80;
    }
  return true;
}

bool nzm::Network::run(zia::api::Net::Callback cb)
{
  if (this->_isRun)
    {
      std::cerr << "Network is already run" << std::endl;
      return false;
    }

  auto funcRunSelect = std::bind(&nzm::Network::runSelect, this, std::placeholders::_1, std::placeholders::_2);

  this->_isRun = true;

  this->_select = std::make_shared<std::thread>(funcRunSelect, this->_port, cb);

  return false;
}

bool nzm::Network::send(zia::api::ImplSocket *sock, const zia::api::Net::Raw &resp)
{
  auto socket = reinterpret_cast<Socket *>(sock);
  socket->getBufferOut().pushRaw(resp);
  return true;
}

bool nzm::Network::stop()
{
  if (!this->_isRun)
    {
      std::cerr << "Network is not run" << std::endl;
    }
  this->_stop.store(true);
  this->_select->join();
  this->_isRun = false;
  return true;
}

void nzm::Network::runSelect(short port, zia::api::Net::Callback cb)
{
  std::shared_ptr<Socket> socketServer = std::make_shared<Socket>();
  Select select(cb, *this);

  socketServer->initServer(port);
  select.addListenTunnels(socketServer);
  while (!this->_stop.load()) {
      select.run();
    }
}
