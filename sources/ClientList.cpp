//
// Created by dmitrykersh on 07.01.2021.
//

#include <ClientList.hpp>

bool ClientList::add_client(const std::string& user) {
  if (list.insert(user).second){
    last_update_time_ = NOW;
    return true;
  }
  return false;
}

bool ClientList::remove_client(const std::string& user) {
  if (list.erase(user)){
    last_update_time_ = NOW;
    return true;
  }
  return false;
}

std::string ClientList::get_list() const {
  std::string str = "[ ";

  for (auto const& user : list){
    str += (user + "; ");
  }

  return str + ']';
}
ClientList::ClientList() : last_update_time_(NOW){}
