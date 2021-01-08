//
// Created by dmitrykersh on 07.01.2021.
//

#include <ClientList.hpp>

bool ClientList::add_client(const std::string& user) {
  std::unique_lock const lock(list_mutex);
  if (list.insert(user).second) {
    last_update_time_ = NOW;
    return true;
  }
  return false;
}

bool ClientList::remove_client(const std::string& user) {
  std::unique_lock const lock(list_mutex);
  if (list.erase(user)) {
    last_update_time_ = NOW;
    return true;
  }
  return false;
}

std::string ClientList::get_list() const {
  std::shared_lock const lock(list_mutex);
  std::string str = "[ ";

  for (auto const& user : list) {
    str += (user + "; ");
  }
  str += ']';
  BOOST_LOG_TRIVIAL(debug) << "Got client list: " << str;
  return str;
}
ClientList::ClientList() : last_update_time_(NOW) {}
