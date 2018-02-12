#include "Socket.hh"

nzm::Socket::Socket():
	_isInit(false),
	_fd(-1)
{
  nz::Log::debug("Socket CTOR");
}

nzm::Socket::~Socket()
{
  shutdown(this->getFd(), SHUT_RDWR);
  close(this->getFd());
  nz::Log::debug("Socket DTOR");
}

int nzm::Socket::getFd() const
{
  return this->_fd;
}

int nzm::Socket::initServer(short port)
{
  struct sockaddr_in 	sin;
  int 			i;

  if (this->_isInit)
    throw ModuleNetworkException("Socket already init");

  if ((this->_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    throw ModuleNetworkException("Socket init error");

  this->_isInit = true;

  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  i = 1;
  if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEPORT, &i, sizeof(int)) < 0)
    throw ModuleNetworkException("Socket fail set options");
  if (bind(this->_fd, (struct sockaddr*)&sin, sizeof(sin)) < 0 || listen(this->_fd, 100) < 0)
    throw ModuleNetworkException("Fail bind and listen");
  this->_isServer = true;
  return this->_fd;
}

int nzm::Socket::initClient(Socket & socketServer)
{
  struct sockaddr_in 	client_sin;
  socklen_t 		client_sin_len;

  if (this->_isInit)
    throw ModuleNetworkException("Socket already init");

  client_sin_len = sizeof(client_sin);
  if ((this->_fd = accept(socketServer.getFd(), (struct sockaddr *)&client_sin,
		   &client_sin_len)) < 0)
    throw ModuleNetworkException("Fail accept");

  this->_isInit = true;
  this->_isServer = false;
  return this->_fd;
}

bool nzm::Socket::isServer() const
{
  return _isServer;
}

bool nzm::Socket::operator==(const nzm::Socket &rhs) const
{
  return this->getFd() == rhs.getFd();
}

int nzm::Socket::read()
{
  char buf[1000];
  int len = recv(this->getFd(), buf, sizeof(buf), 0);
  if (len <= 0) {
      throw ModuleNetworkException("Socket is close");
    }
  std::cout << "READ: " << buf << std::endl;
  return len;
}

int nzm::Socket::write(std::vector<char> raw)
{
  return 0;
}
