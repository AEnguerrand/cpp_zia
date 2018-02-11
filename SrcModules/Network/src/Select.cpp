#include "Select.hh"

nzm::Select::Select()
{

}

void nzm::Select::run()
{
  FD_ZERO(&this->_fdsRead);

  for (auto it : this->_tunnels) {
      FD_SET(it.getFd(), &this->_fdsRead);
    }

  if (select(this->getMaxFd() + 1, &this->_fdsRead, NULL, NULL, NULL) > 0) {
      // something is readable
      for (auto it : this->_listenTunnels) {
	  if (FD_ISSET(it.getFd(), &this->_fdsRead)) {
	      // it's the listener
	      this->addTunnel(it);
	    }
	}
      for (auto it : this->_tunnels) {
	  if (FD_ISSET(it.getFd(), &this->_fdsRead)) {
	      // handle data on this connection
	      nz::Log::debug("Data read");
	    }
	}
    }
}

void nzm::Select::addTunnel(Socket socket)
{
  this->_tunnels.push_back(socket);
}

void nzm::Select::removeTunnel(Socket socket)
{
  this->_tunnels.erase(std::find(this->_tunnels.begin(), this->_tunnels.end(), socket));
}

void nzm::Select::addListentTunnels(nzm::Socket socket)
{
  this->_listenTunnels.push_back(socket);
}

int nzm::Select::getMaxFd()
{
  int maxFd = 0;
  for (auto it : this->_tunnels) {
      if (maxFd < it.getFd())
	maxFd = it.getFd();
    }
  for (auto it : this->_listenTunnels) {
      if (maxFd < it.getFd())
	maxFd = it.getFd();
    }
  return maxFd;
}
