#include "Parser.hh"

nz::Parser::Parser(Process & process, ::zia::api::Net *net):
	_process(process),
	_net(net)
{}

void nz::Parser::callbackRequestReceived(::zia::api::Net::Raw cRaw, ::zia::api::NetInfo cNetInfo)
{
  std::future<void> processStart(std::async([&](::zia::api::Net::Raw raw, ::zia::api::NetInfo netInfo) {
    // Async process HTTP
    // Parser
    zia::api::HttpDuplex httpDuplex = this->_httpParser.Parse(raw);

    httpDuplex.info = netInfo;

    this->_process.startProcess(httpDuplex);
    // Order to send parser ZIA HTTP and at end network module
    // Todo: Tranform httpDuplex to raw
    this->_net->send(netInfo.sock, httpDuplex.raw_resp);
  }, cRaw, cNetInfo));

  processStart.get();
}

void nz::Parser::setNet(zia::api::Net *net)
{
  this->_net = net;
}
