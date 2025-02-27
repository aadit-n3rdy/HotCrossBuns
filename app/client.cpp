#include <iostream>

#include "comms/replica.hpp"

int main(int argc, char *argv[]) {
  std::string buff = "ls | grep utils";

  Replica *client = new Replica();

  client->create_socket();

  client->register_node();

  Node *node = client->get_utils()->create_leaf(buff, nullptr);
  Msg *msg = client->get_utils()->create_msg(NEW_VIEW, node, nullptr);

  client->send_msg(0, msg);

  client->close_socket();

  return 0;
}
