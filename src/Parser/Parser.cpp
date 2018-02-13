#include "Parser.hh"

nz::Parser::Parser(Process & process, ::zia::api::Net *net):
	_process(process),
	_net(net)
{}

void nz::Parser::callbackRequestReceived(::zia::api::Net::Raw raw, ::zia::api::NetInfo netInfo)
{
  // Async process HTTP
  nz::Log::debug("CALLBACK READ TRIGGER");
  // Parser
  zia::api::HttpDuplex httpDuplex;

  this->_process.startProcess(httpDuplex);
  // Order to send parser ZIA HTTP and at end network module
  this->_net->send(netInfo.sock, httpDuplex.raw_resp);
}

void nz::Parser::setNet(zia::api::Net *net)
{
  this->_net = net;
}
