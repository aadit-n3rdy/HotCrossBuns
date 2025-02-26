#include <iostream>

#include "comms/replica.hpp"

int main(int argc, char *argv[]) {
  std::string buff;

  std::cout << "Enter string: \n";
  std::getline(std::cin, buff);

  Replica *client = new Replica();

  client->create_socket();

  client->register_node();

  client->send_msg(0, buff);

  client->close_socket();

  return 0;
}
