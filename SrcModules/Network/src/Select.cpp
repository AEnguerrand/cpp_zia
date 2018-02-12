#include "Select.hh"

nzm::Select::Select()
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
		  nz::Log::debug("START TEST WRITE");
		  // FOR TEST
		  std::string reponse;
		  std::string content = "<h1>Hello Word</h1>";

		  reponse = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(content.length()) +  "\r\nContent-Type: text/html\r\n\r\n" + content;
		  std::vector<char> raw (reponse.begin(), reponse.end());
		  it->write(raw);
		}
	      catch (ModuleNetworkException &e) {
		  this->_tunnels.erase(std::find(this->_tunnels.begin(), this->_tunnels.end(), it));
		}
	    }
	  if (FD_ISSET(it->getFd(), &this->_fdsWrite)) {
	      nz::Log::debug("SELECT DATA WRITE");
	      try {

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
