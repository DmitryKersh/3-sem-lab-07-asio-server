//
// Created by dmitrykersh on 05.01.2021.
//

#ifndef TEMPLATE_CLIENT_HPP
#define TEMPLATE_CLIENT_HPP

#include <asio.hpp>
auto constexpr ENDLINE = '\n';

using asio::error_code;
using asio::ip::tcp;

class Client {
 private:
  tcp::socket socket_;
  std::chrono::system_clock::time_point last_time_active_;

 public:
  explicit Client(asio::io_service& service);
  ~Client();

  void ping(error_code& error);
  void close();
  void disconnect_inactive(error_code& error);

  [[nodiscard]] std::chrono::system_clock::time_point last_time_active() const {
    return last_time_active_;
  }

  [[nodiscard]] tcp::socket const& socket() const { return socket_; }
  [[nodiscard]] tcp::socket& socket() { return socket_; }

  void send(std::string const& payload, error_code& error);

  bool handle(error_code& error);

 private:
  bool  is_logged;
  void disconnect(std::string const& final_message, error_code& error);
};
#endif  // TEMPLATE_CLIENT_HPP
