#include <Server.hpp>
#include <iostream>

static void log_init() {
  namespace logging = ::boost::log;
  namespace sources = logging::sources;
  namespace expressions = logging::expressions;
  namespace keywords = logging::keywords;
  namespace sinks = logging::sinks;

  logging::add_common_attributes();

  logging::core::get()->set_filter(logging::trivial::severity >=
                                   logging::trivial::trace);

  logging::add_file_log(keywords::file_name = "logs/file_%3N.log",
                        keywords::rotation_size = 5 * 1024,
                        keywords::time_based_rotation =
                            sinks::file::rotation_at_time_point(12, 0, 0));
}

int main() {
  asio::io_service service;
  log_init();
  const int port = 8080;

  Server server(
      Properties{tcp::endpoint{tcp::v4(), port}, std::chrono::seconds(20), 2});

  std::string input;
  while (input != "stop") {
    std::getline(std::cin, input);
  }

  server.stop();

  return 0;
}