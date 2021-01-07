//
// Created by dmitrykersh on 07.01.2021.
//

#ifndef ASIO_SERVER_CLIENTLIST_HPP
#define ASIO_SERVER_CLIENTLIST_HPP

#include <asio.hpp>

// logging stuff

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/keywords/file_name.hpp>
#include <boost/log/keywords/rotation_size.hpp>
#include <boost/log/keywords/time_based_rotation.hpp>
#include <boost/log/sinks.hpp>

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

// other

#include <set>
#include <shared_mutex>
#include <optional>

// macro

#define NOW std::chrono::system_clock::now()

auto constexpr ENDLINE = '\n';

using asio::error_code;
using asio::ip::tcp;

typedef std::chrono::system_clock::time_point timetype;

class ClientList{
  timetype last_update_time_;
  std::set<std::string> list;
  std::shared_mutex list_mutex;
 public:
  ClientList();
  [[nodiscard]] inline timetype last_update_time() const { return last_update_time_; }
  bool add_client(std::string const& user);
  bool remove_client(std::string const& user);
  [[nodiscard]] std::string get_list() const;
};

#endif  // ASIO_SERVER_CLIENTLIST_HPP
