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
  return false;
}

zia::api::Net::Raw nzm::Buffer::getHttpResponse()
{
  return zia::api::Net::Raw();
}
