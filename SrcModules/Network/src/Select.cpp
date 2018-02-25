#include "Select.hh"
#include "Network.hh"

nzm::Select::Select(zia::api::Net::Callback cb, Network &network) :
	_callback(cb),
	_network(network)
{
}

void nzm::Select::run()
{
  FD_ZERO(&_fdsRead);
  FD_ZERO(&_fdsWrite);

  for (auto &it : _tunnels)
    {
      FD_SET(it->getFd(), &_fdsRead);
      FD_SET(it->getFd(), &_fdsWrite);
    }
  for (auto &it : _listenTunnels)
    {
      FD_SET(it->getFd(), &_fdsRead);
      FD_SET(it->getFd(), &_fdsWrite);
    }
  struct timeval tv;

  tv.tv_sec = 5;
  tv.tv_usec = 0;

  if (select(getMaxFd() + 1, &_fdsRead, &_fdsWrite, NULL, &tv) > 0)
    {
      for (auto &it : _tunnels)
	{
	  if (FD_ISSET(it->getFd(), &_fdsRead))
	    {
	      try
		{
		  it->read();
		  if (it->getBufferIn().hasHTTPRequest())
		    {
		      zia::api::NetInfo netInfo;
		      it->fillNetinfo(netInfo);
		      netInfo.sock = reinterpret_cast<zia::api::ImplSocket *>(it.get());
		      _callback(it->getBufferIn().getHttpRequest(), netInfo);
		    }
		}
	      catch (ModuleNetworkException e)
		{
		  removeTunnel(it);
		  break;
		}
	    }
	  else if (FD_ISSET(it->getFd(), &_fdsWrite))
	    {
	      try
		{
		  it->checkWrite();
		}
	      catch (ModuleNetworkException e)
		{
		  removeTunnel(it);
		  break;
		}
	    }
	}
      for (auto &it : _listenTunnels)
	{
	  if (FD_ISSET(it->getFd(), &_fdsRead))
	    {
	      addTunnel(it);
	    }
	}
    }
}

void nzm::Select::addTunnel(std::shared_ptr<Socket> socket)
{
  std::shared_ptr<Socket> socketAccept = std::make_shared<Socket>();

  socketAccept->initClient(*socket.get());
  _tunnels.push_back(std::move(socketAccept));

}

void nzm::Select::removeTunnel(std::shared_ptr<Socket> socket)
{
  for (unsigned int i = 0; i < _tunnels.size(); i++)
    {
      if (_tunnels.at(i) == socket)
	{
	  FD_CLR(_tunnels.at(i)->getFd(), &_fdsRead);
	  FD_CLR(_tunnels.at(i)->getFd(), &_fdsWrite);
	  _tunnels.erase(_tunnels.begin() + i);
	  break;
	}
    }
}

void nzm::Select::addListenTunnels(std::shared_ptr<Socket> socket)
{
  _listenTunnels.push_back(socket);
}

int nzm::Select::getMaxFd()
{
  int maxFd = 0;
  for (auto &it : _tunnels)
    {
      if (maxFd < it->getFd())
	maxFd = it->getFd();
    }
  for (auto &it : _listenTunnels)
    {
      if (maxFd < it->getFd())
	maxFd = it->getFd();
    }
  return maxFd;
}

void nzm::Select::printTunnels()
{
  std::cerr << "Tunnels list" << std::endl;
  std::cerr << "[---------------------------]" << std::endl;
  for (auto &i : _tunnels)
    {
      std::cerr << std::setw(4) << i->getFd() << std::endl;
    }
  std::cerr << "[---------------------------]" << std::endl;
}
