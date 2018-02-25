#ifndef CPP_ZIA_LOG_HH
#define CPP_ZIA_LOG_HH

#include <string>
#include <iostream>

namespace nz
{
  namespace Log
  {
    static void checkEndl(bool endl, std::ostream& os = std::cout)
    {
      if (endl) os << std::endl;
    }

    static void print(const std::string &data, bool endl = true)
    {
      std::cout << data;
      checkEndl(endl);
    }

    template<class T>
    static void print(const T &data, bool endl = true)
    {
      std::cout << data;
      checkEndl(endl);
    }

    template<class T>
    static void warning(const T &data, const std::string &warningType, bool endl = true)
    {
      std::cerr << "[WARNING] - " << warningType << ": " << data;
      checkEndl(endl, std::cerr);
    }

    template<class T>
    static void error(const T &data, const std::string &errorType, const std::int32_t errorCode, bool endl = true)
    {
      std::cerr << "[ERROR " << errorCode << "] - " << errorType << ": " << data;
      checkEndl(endl, std::cerr);
    }

    template<class T>
    static void inform(const T &data, bool endl = true)
    {
      std::cout << "[INFO]: " << data;
      checkEndl(endl);
    }

    template<class T>
    static void debug(const T &data, bool endl = true)
    {
      std::cout << "[DEBUG] " << ": " << data;
      checkEndl(endl);
    }
  }
}

#endif //CPP_ZIA_LOG_HH
