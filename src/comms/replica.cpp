#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "comms/replica.hpp"

Replica::Replica() {
  this->local_viewNumber = 0;
  this->global_viewNumber = 0;
  this->otherIdx = 0;
}

void Replica::create_socket() {
  this->listeningSock = socket(AF_INET, SOCK_STREAM, 0);
  this->sendingSock = socket(AF_INET, SOCK_STREAM, 0);
}

void Replica::close_socket() { close(this->listeningSock); }

void Replica::register_node() {
  this->otherAddrs[otherIdx].sin_family = AF_INET;
  this->otherAddrs[otherIdx].sin_port = htons(8080);
  this->otherAddrs[otherIdx].sin_addr.s_addr = INADDR_ANY;
  otherIdx++;
}

void Replica::listen() {
  int err;

  this->Addr.sin_family = AF_INET;
  this->Addr.sin_port = htons(PORT);
  this->Addr.sin_addr.s_addr = INADDR_ANY;

  err = bind(this->listeningSock, (struct sockaddr *)&this->Addr,
             sizeof(this->Addr));
  if (err != 0) {
    std::cout << std::strerror(errno) << '\n';
    exit(1);
  }

  ::listen(this->listeningSock, MAX_CONN);
}

void Replica::recv_msg() {
  int clientSock;
  char buff[BUFF_SIZE] = {0};

  clientSock = accept(this->listeningSock, nullptr, nullptr);

  recv(clientSock, buff, std::size(buff), 0);
  std::cout << "Received message: " << buff << '\n';

  close(this->listeningSock);
}
void Replica::send_msg(int idx, std::string buff) {
  int err;

  err = connect(this->sendingSock, (struct sockaddr *)&otherAddrs[idx],
                sizeof(otherAddrs[idx]));
  if (err != 0) {
    std::cout << std::strerror(errno) << '\n';
    exit(1);
  }

  send(this->sendingSock, buff.c_str(), buff.length(), 0);
  std::cout << "Sent message: " << buff << '\n';
}
