#include "Parser.hh"

nz::Parser::Parser()
{}

void nz::Parser::callbackRequestReceived(::zia::api::Net::Raw raw, ::zia::api::NetInfo netInfo)
{
  // Async process HTTP
  nz::Log::debug("CALLBACK READ TRIGGER");
}
