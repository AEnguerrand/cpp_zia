#include "ParserJson.hh"

nz::ParserJson::ParserJson(const std::string& path)
{
  std::ifstream   file(path);
  nlohmann::json  json;

  std::cerr << "Test 1" << std::endl;
  try {
    file >> json;
  }
  catch (nlohmann::detail::parse_error) {
    this->_fileInvalid = true;
    return;
  }
  this->_json = json;
  this->_fileInvalid = false;
  file.close();
}

nz::ParserJson::~ParserJson(void) {}

zia::api::Conf nz::ParserJson::getConfig(void)
{
  zia::api::Conf  config;

  if (this->_fileInvalid)
    return (config);
  for (auto it = this->_json.begin(); it != this->_json.end(); ++it) {
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
  if (this->_fileInvalid) {
    std::cout << "Configuration file is empty" << std::endl;
  }
  else {
    std::cout << std::setw(4) << this->_json << std::endl;
  }
}