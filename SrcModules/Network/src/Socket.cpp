#include "Socket.hh"

nzm::Socket::Socket():
	_isInit(false),
	_fd(-1)
{
	mp::__initSocket();
}

nzm::Socket::~Socket()
{
  this->_isInit = false;
  mp::__shutdown(this->getFd());
  mp::__closeSocket(this->getFd());
}

int nzm::Socket::getFd() const
{
  return this->_fd;
}

bool nzm::Socket::isInit() const
{
  return this->_isInit;
}

int nzm::Socket::initServer(short port)
{
  struct sockaddr_in 	sin;

  if (this->_isInit)
    throw ModuleNetworkException("Socket already init");

  if ((this->_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    throw ModuleNetworkException("Socket init error");

  this->_isInit = true;

  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  auto i = mp::__cast<MP_TYPE>(1);
  if (setsockopt(this->_fd, SOL_SOCKET, MP_REUSE_PORT_FLAG, &i, sizeof(int)) < 0)
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

  for (auto i = 0; i < len ; i++) {
      this->_bufferIn.push(buf[i]);
    }
  return len;
}

int nzm::Socket::write(zia::api::Net::Raw raw)
{
	std::string data;
	for (auto & i : raw)
		data.push_back(static_cast<char>(i));
	int len = send(this->getFd(), data.data(), raw.size(), 0);
	if (len <= 0)
		throw ModuleNetworkException("Socket is close");
  return 0;
}

void nzm::Socket::checkWrite()
{
  if (this->_bufferOut.hasHTTPResponse()) {
      this->write(this->_bufferOut.getHttpResponse());
    }
}

const nzm::Buffer &nzm::Socket::getBufferIn() const
{
  return this->_bufferIn;
}

const nzm::Buffer &nzm::Socket::getBufferOut() const
{
  return this->_bufferOut;
}

nzm::Buffer &nzm::Socket::getBufferIn()
{
  return this->_bufferIn;
}

nzm::Buffer &nzm::Socket::getBufferOut()
{
  return this->_bufferOut;
}

void nzm::Socket::fillNetinfo(zia::api::NetInfo & netInfo) const
{
  netInfo.port = 0;
  netInfo.ip.i = 0;
  netInfo.ip.str = "127.0.0.1";
  netInfo.start = std::chrono::steady_clock::now();
  netInfo.time = std::chrono::system_clock::now();
  netInfo.sock = nullptr;
}
