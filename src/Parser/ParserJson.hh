#ifndef CPP_ZIA_PARSERJSON_HH
#define CPP_ZIA_PARSERJSON_HH

#include <fstream>
#include <iostream>

#include "api.h"
#include "json.hpp"

namespace nz {
  class ParserJson
  {
    private:
      nlohmann::json  _json;
      bool            _fileInvalid;
      
    public:
      ParserJson(const std::string&);
      virtual ~ParserJson(void);

      zia::api::Conf  getConfig(void);
      void            dump(void);
  };
}

#endif //CPP_ZIA_PARSERJSON_HH
