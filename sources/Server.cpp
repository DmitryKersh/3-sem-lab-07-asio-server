// Copyright 2020 Your Name <your_email>

#include <Server.hpp>
#include <iostream>

Server::Server(Properties properties)
    : properties_(properties),
      connections_thread(&Server::handle_incoming_clients, this),
      client_threads(properties_.n_threads) {
  BOOST_LOG_TRIVIAL(info) << "Starting server at port <port> with" << properties_.n_threads << "client threads";
  for (auto& thread : client_threads) {
    BOOST_LOG_TRIVIAL(debug) << "Starting thread";
    thread = std::thread(&Server::handle_connected_clients, this);
  }
}

void Server::stop() {
  BOOST_LOG_TRIVIAL(info) << "Stopping server";
  shutdown = true;
  BOOST_LOG_TRIVIAL(debug) << "Stopping IO service";
  service_.stop();
  BOOST_LOG_TRIVIAL(debug) << "Stopping connection thread";
  connections_thread.join();

  for (auto& thread : client_threads) {
    BOOST_LOG_TRIVIAL(debug) << "Stopping threads";
    thread.join();
  }
}

void Server::handle_incoming_clients() {
  tcp::acceptor acceptor(service_, properties_.endpoint);

  while (!shutdown) {
    auto client = std::make_shared<Client>(service_, client_list_);

    {
      error_code e;
      BOOST_LOG_TRIVIAL(info) << "waiting client...";
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
      clients_.pop();
    }

    if (NOW - client->last_time_active() > properties_.client_timeout) {
      error_code error;
      client->disconnect_inactive(error);
      BOOST_LOG_TRIVIAL(info) << "Disconnecting inactive client";
      if (error) {
        BOOST_LOG_TRIVIAL(warning) <<  "Error while disconnecting inactive client";
      }
    }

    error_code error;
    // add the client to the queue if it behaves correctly
    if (!error && client->handle(error)) {
      std::scoped_lock const lock(client_mutex);
      clients_.push(client);
    } else {
      // if something went wrong user is removed from list and needs to re-login
      client_list_.remove_client(*(client->username));
    }
  }
}
