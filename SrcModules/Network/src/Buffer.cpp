#include "Buffer.hh"

nzm::Buffer::Buffer()
{}

void nzm::Buffer::push(char data)
{
  this->_buffer.push_back(data);
}

char nzm::Buffer::pop()
{
  char data = this->_buffer.front();
  this->_buffer.pop_front();
  return 0;
}

bool nzm::Buffer::hasHTTPRequest() const
{
  return true;
}

zia::api::Net::Raw nzm::Buffer::getHttpRequest()
{
  return zia::api::Net::Raw();
}

bool nzm::Buffer::hasHTTPResponse() const
{
  return true;
}

zia::api::Net::Raw nzm::Buffer::getHttpResponse()
{
  // FOR TEST
  std::string reponse;
  std::string content = "<h1>Hello Word</h1>";

  reponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(content.length()) +  "\r\n\r\n" + content;

  for (char i : reponse) {
      this->push(i);
    }
  // FOR TEST - END
  zia::api::Net::Raw ret;

  for (char i : this->_buffer) {
      ret.push_back(std::byte(i));
    }
  return ret;
}
