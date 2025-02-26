#ifndef NODE_HPP
#define NODE_HPP

#define MAX_CONN 5
#define PORT 8080
#define BUFF_SIZE 1024
#define NODE_COUNT 10

#include "hotbuns/utils.hpp"
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

class Replica {
private:
  int local_viewNumber;
  int global_viewNumber;

  sockaddr_in Addr;
  sockaddr_in otherAddrs[NODE_COUNT];
  int otherIdx;
  int listeningSock, sendingSock;

  Utilities utils;

public:
  Replica();

  void create_socket();

  void close_socket();

  void register_node();

  void listen();

  void recv_msg();

  void send_msg(int idx, std::string buff);

  ~Replica();
};

#endif // !NODE_HPP
