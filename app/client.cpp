#include <iostream>

#include "comms/node.hpp"

int main(int argc, char *argv[]) {
  std::string buff;

  std::cout << "Enter string: \n";
  std::getline(std::cin, buff);

  Node *client = new Node();

  client->create_socket();

  client->register_node();

  client->send_msg(0, buff);

  client->close_socket();

  return 0;
}
