#include "ParserJson.hh"

nz::ParserJson::ParserJson(const std::string& path)
:
  _invalidFile(false)
{
  std::ifstream   file(path);
  nlohmann::json  json;

  try {
    file >> json;
  }
  catch (nlohmann::detail::parse_error) {
    _invalidFile = true;
    file.close();
    return;
  }
  _json = json;
  file.close();
}

nz::ParserJson::~ParserJson(void) {}

zia::api::Conf nz::ParserJson::getConfig(void)
{
  zia::api::Conf  config;

  if (_invalidFile)
    return (config);
  for (auto it = _json.begin(); it != _json.end(); ++it) {
    if (it.value().is_string())
      config[it.key()].v = it.value().get<std::string>();
    else if (it.value().is_number())
      config[it.key()].v = it.value().get<long long>();
    else if (it.value().is_boolean())
      config[it.key()].v = it.value().get<bool>();
    else if (it.value().is_array())
      {
        zia::api::ConfArray array;
        int i = 0;

        for (auto& value : it.value()) {
          array.push_back(zia::api::ConfValue());
          if (value.is_string())
			array[i].v = value.get<std::string>();
          if (value.is_number())
            array[i].v = value.get<long long>();
          i++;
        }
        config[it.key()].v = array;
      }
  }
  return (config);
}

void  nz::ParserJson::dump(void)
{
  if (_invalidFile)
    nz::Log::warning("Configuration file's empty", "INVALID_CONF");
  else
    nz::Log::debug(_json);
}