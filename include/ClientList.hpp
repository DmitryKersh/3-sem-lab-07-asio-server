//
// Created by dmitrykersh on 07.01.2021.
//

#ifndef ASIO_SERVER_CLIENTLIST_HPP
#define ASIO_SERVER_CLIENTLIST_HPP

#include <Client.hpp>

#include <set>

class ClientList{
  std::set<std::string> list;

 public:
  bool add_client(std::string const& user);
  bool remove_client(std::string const& user);
  std::string get_list() const;
};

#endif  // ASIO_SERVER_CLIENTLIST_HPP
