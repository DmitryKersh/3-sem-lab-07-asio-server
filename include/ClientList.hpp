//
// Created by dmitrykersh on 07.01.2021.
//

#ifndef ASIO_SERVER_CLIENTLIST_HPP
#define ASIO_SERVER_CLIENTLIST_HPP

#include <asio.hpp>

#include <set>
#include <shared_mutex>
#include <optional>

#define NOW std::chrono::system_clock::now()

auto constexpr ENDLINE = '\n';

using asio::error_code;
using asio::ip::tcp;

typedef std::chrono::system_clock::time_point timetype;

class ClientList{
  timetype last_update_time;
  std::set<std::string> list;
  std::shared_mutex list_mutex;
 public:
  ClientList();
  bool add_client(std::string const& user);
  bool remove_client(std::string const& user);
  [[nodiscard]] std::string get_list() const;
};

#endif  // ASIO_SERVER_CLIENTLIST_HPP
