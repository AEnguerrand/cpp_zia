#include "Select.hh"

nzm::Select::Select()
{

}

void nzm::Select::run()
{

}

void nzm::Select::addTunnel(int fd, std::function<void(unsigned int fd)> readFunc, std::function<void(unsigned int fd)> writeFunc,
			    bool isServer)
{
  if (isServer) {
    }
}

void nzm::Select::removeTunnel(int fd)
{

}
