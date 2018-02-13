#include "Select.hh"

nzm::Select::Select(zia::api::Net::Callback cb):
	_callback(cb)
{
}

void nzm::Select::run()
{
  FD_ZERO(&this->_fdsRead);
  FD_ZERO(&this->_fdsWrite);

  for (auto &it : this->_tunnels) {
      FD_SET(it->getFd(), &this->_fdsRead);
      FD_SET(it->getFd(), &this->_fdsWrite);
    }
  for (auto &it : this->_listenTunnels) {
      FD_SET(it->getFd(), &this->_fdsRead);
      FD_SET(it->getFd(), &this->_fdsWrite);
    }
  if (select(this->getMaxFd() + 1, &this->_fdsRead, NULL, NULL, NULL) > 0) {
      for (auto &it : this->_listenTunnels) {
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
		  if (it->getBufferIn().hasHTTPRequest()) {
		      zia::api::NetInfo netInfo;
		      netInfo.sock = reinterpret_cast<zia::api::ImplSocket *>(it.get());
		      this->_callback(it->getBufferIn().getHttpRequest(), netInfo);
		    }
		}
	      catch (ModuleNetworkException &e) {
		  this->_tunnels.erase(std::find(this->_tunnels.begin(), this->_tunnels.end(), it));
		  continue ;
		}
	    }
	  if (FD_ISSET(it->getFd(), &this->_fdsWrite)) {
	      nz::Log::debug("SELECT DATA WRITE");
	      try {
		  it->checkWrite();
		}
	      catch (ModuleNetworkException &e) {
		  this->_tunnels.erase(std::find(this->_tunnels.begin(), this->_tunnels.end(), it));
		  continue ;
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
