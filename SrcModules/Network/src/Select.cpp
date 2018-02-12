#include "Select.hh"

nzm::Select::Select()
{

}

void nzm::Select::run()
{
  FD_ZERO(&this->_fdsRead);

  for (auto &it : this->_tunnels) {
      FD_SET(it->getFd(), &this->_fdsRead);
    }
  for (auto &it : this->_listenTunnels) {
      FD_SET(it->getFd(), &this->_fdsRead);
    }
  if (select(this->getMaxFd() + 1, &this->_fdsRead, NULL, NULL, NULL) > 0) {
      for (auto it : this->_listenTunnels) {
	  if (FD_ISSET(it->getFd(), &this->_fdsRead)) {
	      nz::Log::debug("SELECT ADD");
	      this->addTunnel(it);
	    }
	}
      for (auto &it : this->_tunnels) {
	  if (FD_ISSET(it->getFd(), &this->_fdsRead)) {
	      nz::Log::debug("SELECT DATA READ");
	      try {
		  it->read();
		}
	      catch (ModuleNetworkException &e) {
		  this->_tunnels.erase(std::find(this->_tunnels.begin(), this->_tunnels.end(), it));
		}
	    }
	}
    }
}

void nzm::Select::addTunnel(std::shared_ptr<Socket> socket)
{
  auto 	socketAccept = std::make_shared<Socket>();

  socketAccept->initClient(*socket);
  this->_tunnels.push_back(socketAccept);
}

void nzm::Select::removeTunnel(std::shared_ptr<Socket> socket)
{
  this->_tunnels.erase(std::find(this->_tunnels.begin(), this->_tunnels.end(), socket));
}

void nzm::Select::addListenTunnels(std::shared_ptr<Socket> socket)
{
  this->_listenTunnels.push_back(socket);
}

int nzm::Select::getMaxFd()
{
  int maxFd = 0;
  for (auto &it : this->_tunnels) {
      if (maxFd < it->getFd())
	maxFd = it->getFd();
    }
  for (auto &it : this->_listenTunnels) {
      if (maxFd < it->getFd())
	maxFd = it->getFd();
    }
  return maxFd;
}
