#include <iostream>

#include "comms/replica.hpp"

int main(int argc, char *argv[]) {
  std::cout << "Hello server\n";

  Replica *server = new Replica();

  server->create_socket();
  server->listen();
  server->recv_msg();

  server->close_socket();

  return 0;
}
