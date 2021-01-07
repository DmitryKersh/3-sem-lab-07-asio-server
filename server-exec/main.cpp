#include <Server.hpp>

#include <boost/log/trivial.hpp>
#include <iostream>


int main() {
  asio::io_service service;

  Server server(Properties{tcp::endpoint{tcp::v4(), 8080}, std::chrono::seconds(5), 2});

  std::string input;
  while (input != "stop") std::getline(std::cin, input);
  server.stop();

  return 0;
}