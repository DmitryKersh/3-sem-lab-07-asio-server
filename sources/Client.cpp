//
// Created by dmitrykersh on 05.01.2021.
//

#include <Client.hpp>

Client::Client(asio::io_service& service)
    : socket_{service}, last_time_active_(std::chrono::system_clock::now()) {}

Client::~Client() { close();
}

void Client::close() {
  socket_.close();
}

void Client::disconnect_inactive(error_code& error) {
  //TODO: add logging(info) BOOST_LOG_TRIVIAL(info) << "Disconnecting client on timeout" << std::endl;
  disconnect("Disconnected by timeout", error);
}

void Client::disconnect(std::string const& final_message, error_code& error) {
  send(final_message, error);
  close();
}

void Client::send(const std::string& payload, error_code& error) {
  socket_.write_some(asio::buffer(payload + ENDLINE), error);
}
