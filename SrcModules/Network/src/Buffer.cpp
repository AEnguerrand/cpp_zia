#include "Buffer.hh"

nzm::Buffer::Buffer()
{}

void nzm::Buffer::push(char data)
{
  this->_buffer.push_back(std::byte(data));
}

// TODO: change the way of check this
bool nzm::Buffer::hasHTTPRequest() const
{
  zia::api::Net::Raw ret;

  for (auto i : this->_buffer) {
      ret.push_back(i);
  }

  nz::HttpParser parser;

  try {
    parser.Parse(ret);
  } catch(...) {
    return false;
  }
  return true;
}

zia::api::Net::Raw nzm::Buffer::getHttpRequest()
{
    zia::api::Net::Raw ret;
	
	for (auto i : this->_buffer) {
    	ret.push_back(i);
    }

	// TODO: Clean buffer in
	this->_buffer.clear();
	return ret;
}

bool nzm::Buffer::hasHTTPResponse() const
{
  if (this->_buffer.empty())
      return false;
  return true;
}

zia::api::Net::Raw nzm::Buffer::getHttpResponse()
{
  zia::api::Net::Raw ret;

  for (auto i : this->_buffer) {
      ret.push_back(i);
  }

  // TODO: Clean only request get
  this->_buffer.clear();
  return ret;
}

void nzm::Buffer::pushRaw(const zia::api::Net::Raw &resp)
{
  for (auto & u : resp) {
      this->_buffer.push_back(u);
    }
}
