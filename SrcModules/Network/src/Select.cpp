#include "Select.hh"
#include "Network.hh"

nzm::Select::Select(zia::api::Net::Callback cb, Network &network) :
	_callback(cb),
	_network(network)
{
}

void nzm::Select::run()
{
  FD_ZERO(&this->_fdsRead);
  FD_ZERO(&this->_fdsWrite);

  for (auto &it : this->_tunnels)
    {
      FD_SET(it->getFd(), &this->_fdsRead);
      FD_SET(it->getFd(), &this->_fdsWrite);
    }
  for (auto &it : this->_listenTunnels)
    {
      FD_SET(it->getFd(), &this->_fdsRead);
      FD_SET(it->getFd(), &this->_fdsWrite);
    }
  if (select(this->getMaxFd() + 1, &this->_fdsRead, &this->_fdsWrite, NULL, NULL) > 0)
    {
      for (auto &it : this->_tunnels)
	{
	  if (FD_ISSET(it->getFd(), &this->_fdsRead))
	    {
	      try
		{
		  it->read();
		  if (it->getBufferIn().hasHTTPRequest())
		    {
		      zia::api::NetInfo netInfo;
		      netInfo.sock = reinterpret_cast<zia::api::ImplSocket *>(it.get());
		      this->_callback(it->getBufferIn().getHttpRequest(), netInfo);
		    }
		}
	      catch (ModuleNetworkException &e)
		{
		  this->removeTunnel(it);
		  break;
		}
	    }
	  else if (FD_ISSET(it->getFd(), &this->_fdsWrite))
	    {
	      try
		{
		  it->checkWrite();
		}
	      catch (ModuleNetworkException &e)
		{
		  this->removeTunnel(it);
		  break;
		}
	    }
	}
      for (auto &it : this->_listenTunnels)
	{
	  if (FD_ISSET(it->getFd(), &this->_fdsRead))
	    {
	      this->addTunnel(it);
	    }
	}
      //Todo: Remove and fix error "server disconnect user"
      //this->printTunnels();
    }
}

void nzm::Select::addTunnel(std::shared_ptr<Socket> socket)
{
  std::shared_ptr<Socket> socketAccept = std::make_shared<Socket>();

  socketAccept->initClient(*socket.get());
  this->_tunnels.push_back(std::move(socketAccept));

}

void nzm::Select::removeTunnel(std::shared_ptr<Socket> socket)
{
  for (unsigned int i = 0; i < this->_tunnels.size(); i++)
    {
      if (this->_tunnels.at(i) == socket)
	{
	  FD_CLR(this->_tunnels.at(i)->getFd(), &this->_fdsRead);
	  FD_CLR(this->_tunnels.at(i)->getFd(), &this->_fdsWrite);
	  this->_tunnels.erase(this->_tunnels.begin() + i);
	  break;
	}
    }
}

void nzm::Select::addListenTunnels(std::shared_ptr<Socket> socket)
{
  this->_listenTunnels.push_back(socket);
}

int nzm::Select::getMaxFd()
{
  int maxFd = 0;
  for (auto &it : this->_tunnels)
    {
      if (maxFd < it->getFd())
	maxFd = it->getFd();
    }
  for (auto &it : this->_listenTunnels)
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
  for (auto &i : this->_tunnels)
    {
      std::cerr << std::setw(4) << i->getFd() << std::endl;
    }
  std::cerr << "[---------------------------]" << std::endl;
}
