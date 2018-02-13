#include "Process.hh"

nz::Process::Process(nz::ModuleLoader &moduleLoader):
	_moduleLoader(moduleLoader)
{

}

/**
 * Function is call in Async (if paser HTTP is not call in Async)
 * @param duplex
 */
void nz::Process::startProcess(zia::api::HttpDuplex & duplex)
{
  auto Modules = this->_moduleLoader.getModules();

  for (auto it : Modules) {
      it.second->exec(duplex);
    }

  // Fake Module HTTP
  // FOR TEST
  std::string reponse;
  std::string content = "<h1>Hello World</h1>";

  reponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(content.length()) +  "\r\n\r\n" + content;

  for (char i : reponse) {
      duplex.raw_resp.push_back(std::byte(i));
    }
  // FOR TEST - END
}
