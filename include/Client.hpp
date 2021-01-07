//
// Created by dmitrykersh on 05.01.2021.
//

#ifndef TEMPLATE_CLIENT_HPP
#define TEMPLATE_CLIENT_HPP

#include <ClientList.hpp>

class Client {
 private:
  tcp::socket socket_;
  timetype last_time_active_;
  timetype last_time_update_list_;

 public:
  explicit Client(asio::io_service& service, ClientList& list);
  ~Client();

  void ping_ok(error_code& error);
  void list_changed(error_code& error);

  void close();
  void disconnect_inactive(error_code& error);

  [[nodiscard]] timetype last_time_active() const {
    return last_time_active_;
  }

  void reset_last_time_active(){
    last_time_active_ = NOW;
  }

  [[nodiscard]] tcp::socket const& socket() const { return socket_; }
  [[nodiscard]] tcp::socket& socket() { return socket_; }

  void send(std::string const& payload, error_code& error);

  bool handle(error_code& error/*, size_t timelimit*/);
  std::optional<std::string> username;
 private:
  ClientList& list_;

  void timer();
  void disconnect(std::string const& final_message, error_code& error);
};
#endif  // TEMPLATE_CLIENT_HPP
