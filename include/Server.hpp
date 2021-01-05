// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_SERVER_HPP_
#define INCLUDE_SERVER_HPP_

#include <Client.hpp>

class Server{
 public:
  struct Properties{
    tcp::endpoint endpoint;
    std::chrono::seconds client_timeout;
    size_t n_threads;
  };

  explicit Server(Properties properties);

 private:
  const Properties properties_;
  asio::io_service service_;

  std::atomic_bool shutdown{false};

  std::thread connections_thread;
  std::vector<std::thread> client_threads;

  mutable std::mutex client_mutex;

  void handle_connected_clients();
  void handle_incoming_clients();
  void stop();
};

#endif // INCLUDE_SERVER_HPP_
