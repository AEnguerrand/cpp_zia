#ifndef CPP_ZIA_PARSERJSON_HH
#define CPP_ZIA_PARSERJSON_HH

#include <fstream>

#include "api.h"
#include "json.hpp"
#include "ParserJsonException.hpp"

namespace nz {
  class ParserJson
  {
    private:
      nlohmann::json  _json;
      
    public:
      ParserJson(const std::string&);
      virtual ~ParserJson(void);

      zia::api::Conf  getConfig(void);
      void            dump(void);
  };
}

#endif //CPP_ZIA_PARSERJSON_HH
