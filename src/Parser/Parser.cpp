#include "Parser.hh"

nz::Parser::Parser(Process & process):
	_process(process)
{}

void nz::Parser::callbackRequestReceived(::zia::api::Net::Raw raw, ::zia::api::NetInfo netInfo)
{
  // Async process HTTP
  nz::Log::debug("CALLBACK READ TRIGGER");
  // Parser
  zia::api::HttpDuplex httpDuplex;
  
  this->_process.startProcess(httpDuplex);
}
