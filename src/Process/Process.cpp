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
  // Fake Module HTTP
  // FOR TEST
  std::string reponse;
  std::string content = "<h1>Hello World</h1><?php echo '<p>Bonjour le monde</p>'; ?>";

  duplex.resp.version = zia::api::http::Version::http_1_1;
  duplex.resp.status = 200;
  duplex.resp.reason = "OK";
  duplex.resp.headers["Content-Type"] = "text/html";
  duplex.resp.headers["Content-Length"] = std::to_string(content.length());
  for (char i : content) {
    duplex.resp.body.push_back(std::byte(i));
  }
// FOR TEST - END

  auto Modules = this->_moduleLoader.getModules();

  for (auto it : Modules) {
      it.second->exec(duplex);
    }

}
