#include <Server.hpp>

#include <boost/log/trivial.hpp>
#include <iostream>


int main() {
  asio::io_service service;

  // TODO: log (info) Starting server at port <port>
  Server server(Properties{tcp::endpoint{tcp::v4(), 8080}, std::chrono::seconds(10), 2});


  std::string input;
  while (input != "stop") std::getline(std::cin, input);
  server.stop();

  return 0;
}