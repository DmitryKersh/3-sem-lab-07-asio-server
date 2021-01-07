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

Client::Client(asio::io_service& service, ClientList& list)
    : socket_{service},
      last_time_active_(NOW),
      last_time_update_list_(NOW),
      username(std::nullopt),
      list_(list)
{}

Client::~Client() { close(); }

void Client::close() { socket_.close(); }

void Client::disconnect_inactive(error_code& error) {
  disconnect("Disconnected by timeout", error);
}

void Client::disconnect(std::string const& final_message, error_code& error) {
  send(final_message, error);
  close();
}

void Client::send(const std::string& payload, error_code& error) {
  socket_.write_some(asio::buffer(payload + ENDLINE), error);
}

inline void Client::ping_ok(error_code& error) { send("ping_ok", error); }
inline void Client::list_changed(error_code& error) { send("client_list_changed", error); }

bool Client::handle(error_code& error) {
  std::string query;

  asio::streambuf buffer;
  asio::read_until(socket_, buffer, ENDLINE, error);

  if (error) {
    disconnect("Error while reading query", error);
    BOOST_LOG_TRIVIAL(warning) <<  "Error while reading query";
    return false;
  }

  last_time_active_ = NOW;

  std::istream input(&buffer);
  std::getline(input, query);

  auto const words = split(query, ' ');

  if (words.empty()) {
    disconnect("Empty query", error);
    BOOST_LOG_TRIVIAL(warning) <<  "Empty query";
    return false;
  }

  BOOST_LOG_TRIVIAL(trace) << "Query:";
  for (auto const& word : words) {
    BOOST_LOG_TRIVIAL(trace) << word;
  }


  if (username.has_value()){
    if (words[0] == "ping"){
      if (last_time_update_list_ == list_.last_update_time()){
        ping_ok(error);
      } else {
        list_changed(error);
      }
      return true;
    }

    if (words[0] == "clients"){
      send(list_.get_list(), error);
      return true;
    }

    if (words[0] == "exit"){
      disconnect("Client " + *username + " exited", error);
      return false;
    }

    BOOST_LOG_TRIVIAL(warning) << "Unknown command for logged user:" << words[0];
    disconnect("Unknown command for logged user: \"" + words[0] + "\"", error);
    return false;

  } else {
    if (words[0] == "login"){
      if (words.size() > 1){
        std::string name = words[1];

        if (list_.add_client(name)){
          last_time_update_list_ = list_.last_update_time();
          username = name;
          send("login ok. Name: " + name, error);
          return true;
        }

        BOOST_LOG_TRIVIAL(warning) << "Username already exists:" << words[1];
        disconnect("Username already exists\"" + words[1] + "\"", error);
        return false;

      }

      BOOST_LOG_TRIVIAL(warning) << "Empty username";
      disconnect("Empty username", error);
      return false;

    }

    BOOST_LOG_TRIVIAL(warning) <<  "Unknown command for unlogged user:" << words[0];
    disconnect("Unknown command for unlogged user: \"" + words[0] + "\"", error);
    return false;
  }
}
