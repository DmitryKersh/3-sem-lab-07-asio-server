//
// Created by dmitrykersh on 07.01.2021.
//

#include <ClientList.hpp>

bool ClientList::add_client(const std::string& user) {
  return list.insert(user).second;
}

bool ClientList::remove_client(const std::string& user) {
  return list.erase(user);
}

std::string ClientList::get_list() const {
  std::string str = "[ ";

  for (auto const& user : list){
    str += (user + "; ");
  }

  return str + ']';
}
