// Copyright 2020 Your Name <your_email>

#include <Server.hpp>
#include <iostream>

Server::Server(Properties properties)
    : properties_(properties),
      connections_thread(&Server::handle_incoming_clients, this),
      client_threads(properties_.n_threads),
      last_time_list_update_(NOW) {
  // TODO: log (info) Starting server at port <port> with <n_threads> client
  // threads
  for (auto& thread : client_threads) {
    // TODO: log (trace) Starting thread
    thread = std::thread(&Server::handle_connected_clients, this);
  }
}

void Server::stop() {
  // TODO: log (info) Stopping server
  shutdown = true;
  // TODO: log (info) Stopping IO service
  service_.stop();
  // TODO: log (info) Stopping connection thread
  connections_thread.join();

  for (auto& thread : client_threads) {
    // TODO: log (trace) Stopping thread
    thread.join();
  }
}

void Server::handle_incoming_clients() {
  tcp::acceptor acceptor(service_, properties_.endpoint);

  while (!shutdown) {
    auto client = std::make_shared<Client>(service_, client_list_);

    {
      error_code e;
      std::cout << "connection"
                << std::endl;  // TODO: log (trace) "waiting client"
      acceptor.accept(client->socket(), e);

      if (e) break;
    }

    {
      std::scoped_lock const lock(client_mutex);
      clients_.push(std::move(client));
    }
  }
}

void Server::handle_connected_clients() {
  while (!shutdown) {
    std::shared_ptr<Client> client;

    {
      std::scoped_lock const lock(client_mutex);
      if (clients_.empty()) continue;
      client = clients_.front();
      // std::cout << "pop" << std::endl;
      clients_.pop();
    }

    if (NOW - client->last_time_active() > properties_.client_timeout) {
      error_code error;
      client->disconnect_inactive(error);
      if (error) {
        // TODO: log (info) "error while disconnecting inactive client"
      }
    }

    error_code error;
    // add the client to the queue if it behaves correctly
    if (!error && client->handle(last_time_list_update_, error)) {
      std::scoped_lock const lock(client_mutex);
      // std::cout << "push" << std::endl;
      clients_.push(client);
    }
  }
}
