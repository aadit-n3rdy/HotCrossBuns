#include <iostream>

#include "comms/replica.hpp"

int main(int argc, char *argv[]) {
  std::cout << "Starting Server: \n";
  std::cout << "--------------\n";

  Msg msg;

  Replica *server = new Replica();

  server->create_socket();
  server->listen();
  msg = server->recv_msg();

  server->view_msg(&msg);

  server->close_socket();

  return 0;
}
