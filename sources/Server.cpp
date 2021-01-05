// Copyright 2020 Your Name <your_email>

#include <Server.hpp>

Server::Server(Properties properties) : properties_(properties),
                                        connections_thread(&Server::handle_incoming_clients, this),
                                        client_threads(properties_.n_threads){
  for (auto& thread : client_threads){
    thread = std::thread(&Server::handle_connected_clients, this);
  }
}

void Server::stop() {
  shutdown = true;

  service_.stop();

  connections_thread.join();

  for (auto& thread : client_threads){
    thread.join();
  }
}
