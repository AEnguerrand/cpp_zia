#include "Socket.hh"

nzm::Socket::Socket() : _isInit(false), _fd(-1) { mp::__initSocket(); }

nzm::Socket::~Socket() {
  _isInit = false;
  mp::__shutdown(getFd());
  mp::__closeSocket(getFd());
}

int nzm::Socket::getFd() const { return _fd; }

bool nzm::Socket::isInit() const { return _isInit; }

int nzm::Socket::initServer(short port) {
  struct sockaddr_in sin;

  if (_isInit)
    throw ModuleNetworkException("Socket already init");

  if ((_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    throw ModuleNetworkException("Socket init error");

  _isInit = true;

  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  auto i = mp::__cast<MP_TYPE>(1);
  if (setsockopt(_fd, SOL_SOCKET, MP_REUSE_PORT_FLAG, &i, sizeof(int)) < 0)
    throw ModuleNetworkException("Socket fail set options");
  if (bind(_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0 ||
      listen(_fd, 100) < 0)
    throw ModuleNetworkException("Fail bind and listen");
  _isServer = true;
  return _fd;
}

int nzm::Socket::initClient(Socket &socketServer) {
  struct sockaddr_in client_sin;
  socklen_t client_sin_len;

  if (_isInit)
    throw ModuleNetworkException("Socket already init");

  client_sin_len = sizeof(client_sin);
  if ((_fd = accept(socketServer.getFd(), (struct sockaddr *)&client_sin,
                    &client_sin_len)) < 0)
    throw ModuleNetworkException("Fail accept");

  _isInit = true;
  _isServer = false;
  return _fd;
}

bool nzm::Socket::isServer() const { return _isServer; }

bool nzm::Socket::operator==(const nzm::Socket &rhs) const {
  return getFd() == rhs.getFd();
}

int nzm::Socket::read() {
  char buf[1000];
  int len = recv(getFd(), buf, sizeof(buf), 0);
  if (len <= 0) {
    throw ModuleNetworkException("Socket is close");
  }

  for (auto i = 0; i < len; i++) {
    _bufferIn.push(buf[i]);
  }
  return len;
}

int nzm::Socket::write(zia::api::Net::Raw raw) {
  std::string data;
  for (auto &i : raw)
    data.push_back(static_cast<char>(i));
  int len = send(getFd(), data.data(), raw.size(), 0);
  if (len <= 0)
    throw ModuleNetworkException("Socket is close");
  return 0;
}

void nzm::Socket::checkWrite() {
  if (_bufferOut.hasHTTPResponse()) {
    write(_bufferOut.getHttpResponse());
  }
}

const nzm::Buffer &nzm::Socket::getBufferIn() const { return _bufferIn; }

const nzm::Buffer &nzm::Socket::getBufferOut() const { return _bufferOut; }

nzm::Buffer &nzm::Socket::getBufferIn() { return _bufferIn; }

nzm::Buffer &nzm::Socket::getBufferOut() { return _bufferOut; }

void nzm::Socket::fillNetinfo(zia::api::NetInfo &netInfo) const {
  netInfo.port = 0;
  netInfo.ip.i = 0;
  netInfo.ip.str = "127.0.0.1";
  netInfo.start = std::chrono::steady_clock::now();
  netInfo.time = std::chrono::system_clock::now();
  netInfo.sock = nullptr;
}
