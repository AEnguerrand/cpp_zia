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
    try {
	httpDuplex.raw_resp = this->_httpParser.ResponseToRaw(httpDuplex.resp, httpDuplex.req.method);
      }
    catch (nz::HttpParserException e) {
	this->sendErrorServer(httpDuplex);
	httpDuplex.raw_resp = this->_httpParser.ResponseToRaw(httpDuplex.resp, httpDuplex.req.method);
      }

    // Send network
    if (netInfo.sock) {
      this->_net->send(netInfo.sock, httpDuplex.raw_resp);
    }
  }, cRaw, cNetInfo));

  // TODO: List for queue of async (because now wait end of async, is useless now)
  processStart.get();
}

void nz::Parser::setNet(zia::api::Net *net)
{
  this->_net = net;
}

void nz::Parser::sendErrorServer(zia::api::HttpDuplex &httpDuplex) const
{
  // Settings base
  httpDuplex.resp.version = zia::api::http::Version::http_1_1;
  httpDuplex.resp.status = zia::api::http::common_status::internal_server_error;
  httpDuplex.resp.reason = "Internal Server Error";

  // Body
  std::string content =
	  "<html>"
		  "<head>"
		  "<title>Index of " + httpDuplex.req.uri +  "</title>"
		  "</head>"
		  "<body>"
		  "<h1>Not found " + httpDuplex.req.uri + "</h1>";

  content += "Internal Error (not response is generate)";

  content += "</body></html>";


  for (char i : content) {
      httpDuplex.resp.body.push_back(std::byte(i));
    }

  // Headers values
  httpDuplex.resp.headers["Content-Type"] = "text/html";
  httpDuplex.resp.headers["Content-Length"] = std::to_string(httpDuplex.resp.body.size());
}
