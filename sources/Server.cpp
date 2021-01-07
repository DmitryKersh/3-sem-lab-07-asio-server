// Copyright 2020 Your Name <your_email>

#include <Server.hpp>
#include <iostream>

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

void Server::handle_incoming_clients() {
  tcp::acceptor acceptor(service_, properties_.endpoint);

  while (!shutdown){
    auto client = std::make_shared<Client>(service_);

    error_code e;
    std::cout << "connection" << std::endl; //TODO: log "waiting client"
    acceptor.accept(client->socket(), e);

    if (e) break;

    std::scoped_lock const lock(client_mutex);
    clients_.push(std::move(client));
  }
}

void Server::handle_connected_clients() {
  while (!shutdown){
    std::shared_ptr<Client> client;

    {
      std::scoped_lock const lock(client_mutex);
      if (clients_.empty()) continue;
    }

    client = clients_.front();
    clients_.pop();

    if (std::chrono::system_clock::now() - client->last_time_active() > properties_.client_timeout){
      error_code error;
      client->disconnect_inactive(error);
      if (error){
        //TODO: log "error while disconnecting inactive client"
      }
    }

    error_code error;
    // add the client to the queue if it still is alive
    if (!error) {
      std::scoped_lock const lock(client_mutex);
      clients_.push(client);
    }
  }
}
