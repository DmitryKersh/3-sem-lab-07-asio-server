#include <Server.hpp>

#include <boost/log/trivial.hpp>
#include <iostream>


int main() {
  asio::io_service service;

  while (getchar() != 's'){
    getchar();
  }

  return 0;
}