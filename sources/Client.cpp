//
// Created by dmitrykersh on 05.01.2021.
//

#include <Client.hpp>
#include <string>

std::vector<std::string> split(std::string const& s, char delimiter) {
  std::vector<std::string> result;
  size_t begin = 0;
  size_t end;

  while ((end = s.find(delimiter, begin)) != std::string::npos) {
    result.push_back(s.substr(begin, end - begin));
    begin = end + 1;
  }
  result.push_back(s.substr(begin));
  return result;
}

Client::Client(asio::io_service& service)
    : socket_{service},
      last_time_active_(std::chrono::system_clock::now()),
      is_logged(false) {}

Client::~Client() { close(); }

void Client::close() { socket_.close(); }

void Client::disconnect_inactive(error_code& error) {
  // TODO: log(info) "Disconnecting client on timeout"
  disconnect("Disconnected by timeout", error);
}

void Client::disconnect(std::string const& final_message, error_code& error) {
  send(final_message, error);
  close();
}

void Client::send(const std::string& payload, error_code& error) {
  socket_.write_some(asio::buffer(payload + ENDLINE), error);
}

void Client::ping(error_code& error) { send("ping OK", error); }

bool Client::handle(error_code& error) {
  std::string query;

  asio::streambuf buffer;
  asio::read_until(socket_, buffer, ENDLINE, error);

  if (error) {
    disconnect("Error while reading query", error);
    // TODO: log (warning) Error while reading query
    return false;
  }

  last_time_active_ = std::chrono::system_clock::now();

  std::istream input(&buffer);
  std::getline(input, query);

  auto const words = split(query, ' ');

  if (words.empty()) {
    disconnect("Empty query", error);
    // TODO: log (warning) Empty query
    return false;
  }

  for (auto const& word : words) {
    // TODO: log (trace) [word]
  }

  if (is_logged){
    if (words[0] == "ping"){
      ping(error);
      // TODO: log (info) Client pinged
    } else if (words[0] == "clients"){
      // TODO: implementation here
    } else {
      // TODO: log (warning) Unknown command for logged user: *words[0]*
      disconnect("Unknown command for logged user: \"" + words[0] + "\"", error);
      return false;
    }
  } else {
    if (words[0] == "login"){
      // TODO: implementation here
    } else {
      // TODO: log (warning) Unknown command for unlogged user: *words[0]*
      disconnect("Unknown command for unlogged user: \"" + words[0] + "\"", error);
      return false;
    }
  }
}
