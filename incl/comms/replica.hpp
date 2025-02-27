#ifndef NODE_HPP
#define NODE_HPP

#define MAX_CONN 5
#define PORT 8080
#define BUFF_SIZE 1024
#define NODE_COUNT 10

#include "hotbuns/utils.hpp"
#include <netinet/in.h>
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

  std::string enum_as_str(mtype type);

public:
  Replica();

  void create_socket();

  void close_socket();

  void register_node();

  void listen();

  Msg recv_msg();

  void send_msg(int idx, Msg *msg);

  void view_msg(Msg *msg);

  Utilities *get_utils();

  ~Replica();
};

#endif // !NODE_HPP
