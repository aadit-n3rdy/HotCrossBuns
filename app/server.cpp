#include <iostream>

#include "comms/node.hpp"

int main(int argc, char *argv[]) {
  std::cout << "Hello server\n";

  Node *server = new Node();

  server->create_socket();
  server->listen();
  server->recv_msg();

  server->close_socket();

  return 0;
}
