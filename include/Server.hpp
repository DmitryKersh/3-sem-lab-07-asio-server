// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_SERVER_HPP_
#define INCLUDE_SERVER_HPP_

#include <Client.hpp>
#include <queue>

struct Properties {
  tcp::endpoint endpoint;
  std::chrono::seconds client_timeout;
  size_t n_threads;
};

class Server;

class Timer {
  std::chrono::seconds duration_;
  std::shared_ptr<Client> client;
  timetype start_time;
  bool stop_;

 public:
  Timer(std::shared_ptr<Client> c, std::chrono::seconds duration)
      : duration_(duration), client(c), stop_(false) {}

  void run() {
    start_time = NOW;
    while (NOW - start_time < duration_ && !stop_) {
      // sleep(1);
    }
    if (!stop_) {
      error_code e;
      client->disconnect_inactive(e);
    }
  }

  inline void stop() { stop_ = true; }
};

class Server {
 public:
  explicit Server(Properties properties);
  void stop();

 private:
  const Properties properties_;
  asio::io_service service_;

  std::atomic_bool shutdown{false};

  std::thread connections_thread;
  std::vector<std::thread> client_threads;

  mutable std::mutex client_mutex;

  std::queue<std::shared_ptr<Client>> clients_;

  ClientList client_list_;

  void handle_connected_clients();
  void handle_incoming_clients();
};

#endif  // INCLUDE_SERVER_HPP_
