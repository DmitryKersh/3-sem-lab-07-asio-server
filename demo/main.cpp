#include <header.hpp>
#include <asio.hpp>
#include <boost/log/trivial.hpp>

#include <iostream>

using asio::ip::tcp;

asio::io_service service;

class talk_to_client{
  tcp::socket socket;
 public:
  talk_to_client() : socket(service){
    tcp::endpoint endpoint{asio::ip::address::from_string("127.0.0.1"), 80};
    socket.connect(endpoint);
  }
  tcp::socket sock(){
    return socket;
  }
};

std::vector<std::shared_ptr<talk_to_client>> clients;

void accept_thread() {
  tcp::acceptor acceptor{service, tcp::endpoint{asio::ip::tcp::v4(), 8001}};
  while (true) {
    auto client = std::make_shared<talk_to_client>(); // talk_to_client - user defined class
    acceptor.accept(client->sock());
    std::recursive_mutex::scoped_lock lock{mutex};
    clients.push_back(client);
  }
}

void handle_clients_thread() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
    boost::recursive_mutex::scoped_lock lock{mutex};
    for (auto& client : clients) {
      // for each client calling answer_to_client();
    }
    // and then erase clients that timed out
  }
}

int main() {
  const tcp::endpoint endpoint{asio::ip::address::from_string("127.0.0.1"), 80};
  tcp::socket socket{service};
  socket.connect(endpoint);

  return 0;
}