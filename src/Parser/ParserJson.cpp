#include "ParserJson.hh"

nz::ParserJson::ParserJson(const std::string& path)
{
  std::ifstream   file(path);
  nlohmann::json  json;

  if (file.fail()) {
    throw ParserJsonException("Configuration file not found");
  }
  file >> json;
  this->_json = json;
  file.close();
}

nz::ParserJson::~ParserJson(void) {}

zia::api::Conf nz::ParserJson::getConfig(void)
{
  zia::api::Conf  config;

  for (auto it = this->_json.begin(); it != this->_json.end(); ++it) {
    if (it.value().is_string())
      config[it.key()].v = std::string(it.value());
    else if (it.value().is_number())
      config[it.key()].v = static_cast<long long>(it.value());
    else if (it.value().is_boolean())
      config[it.key()].v = static_cast<bool>(it.value());
    else if (it.value().is_array())
      {
        zia::api::ConfArray array;
        int i = 0;

        for (auto& value : it.value()) {
          array.push_back(zia::api::ConfValue());
          if (value.is_string())
            array[i].v = std::string(value);
          else if (value.is_number())
            array[i].v = static_cast<long long>(value);
          i++;
        }
        config[it.key()].v = array;
      }
  }
  return (config);
}

void  nz::ParserJson::dump(void)
{
  std::cout << std::setw(4) << this->_json << std::endl;
}