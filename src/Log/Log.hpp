#ifndef CPP_ZIA_LOG_HH
#define CPP_ZIA_LOG_HH

#include <string>
#include <iostream>

namespace nz
{
  class Log
  {
   public:
    Log() {};

    virtual ~Log() {};

    static void print(const std::string &data, bool endl = true)
    {
      std::cout << data;
      if (endl) std::cout << std::endl;
    }

    template<class T>
    static void print(const T &data, bool endl = true)
    {
      std::cout << data;
      if (endl) std::cout << std::endl;
    }

    template<class T>
    static void warning(const T &data, const std::string &warningType, bool endl = true)
    {
      std::cout << "[WARNING] - " << warningType << ": " << data;
      if (endl) std::cout << std::endl;
    }

    template<class T>
    static void error(const T &data, const std::string &errorType, const std::int32_t errorCode, bool endl = true)
    {
      std::cout << "[ERROR " << errorCode << "] - " << errorType << ": " << data;
      if (endl) std::cout << std::endl;
    }

    template<class T>
    static void inform(const T &data, bool endl = true)
    {
      std::cout << "[INFO]: " << data;
      if (endl) std::cout << std::endl;
    }

    template<class T>
    static void debug(const T &data, bool endl = true)
    {
      std::cout << "[DEBUG] " << ": " << data;
      if (endl) std::cout << std::endl;
    }
  };
}

#endif //CPP_ZIA_LOG_HH
