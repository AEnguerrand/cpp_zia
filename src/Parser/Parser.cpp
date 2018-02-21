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

    // Order to send parser ZIA HTTP and at end network module
    this->_process.startProcess(httpDuplex);

    // Convert http response into raw
    httpDuplex.raw_resp = this->_httpParser.ResponseToRaw(httpDuplex.resp);

    // Send network
    this->_net->send(netInfo.sock, httpDuplex.raw_resp);
  }, cRaw, cNetInfo));

  // TODO: List for queue of async (because now wait end of async, is useless now)
  processStart.get();
}

void nz::Parser::setNet(zia::api::Net *net)
{
  this->_net = net;
}
