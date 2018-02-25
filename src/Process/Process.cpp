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

  for (auto& it : Modules) {
      it.second->exec(duplex);
    }

}
