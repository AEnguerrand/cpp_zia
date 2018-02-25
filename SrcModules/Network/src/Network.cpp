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
  _isRun = false;
  _stop.store(false);
  nz::Log::inform("[Module Network]: Start");
}

nzm::Network::~Network()
{
  stop();
  nz::Log::inform("[Network Module]: Stop");
}

bool nzm::Network::config(const zia::api::Conf &conf)
{
  try { _port = std::get<long long>(conf.at("port").v); }
  catch (...) {
      nz::Log::warning("port not found or must be a number, the default port has been set to '7000'", "Network Module");
      _port = 7000;
    }
  return true;
}

bool nzm::Network::run(zia::api::Net::Callback cb)
{
  if (_isRun)
    {
      std::cerr << "Network is already running" << std::endl;
      return false;
    }

  auto funcRunSelect = std::bind(&nzm::Network::runSelect, this, std::placeholders::_1, std::placeholders::_2);

  _isRun = true;

  _select = std::make_shared<std::thread>(funcRunSelect, _port, cb);

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
  if (!_isRun)
    {
      std::cerr << "Network is not run" << std::endl;
    }
  _stop.store(true);
  _select->join();
  _isRun = false;
  return true;
}

void nzm::Network::runSelect(short port, zia::api::Net::Callback cb)
{
  std::shared_ptr<Socket> socketServer = std::make_shared<Socket>();
  Select select(cb, *this);

  socketServer->initServer(port);
  select.addListenTunnels(socketServer);
  while (!_stop.load()) {
      select.run();
    }
}
