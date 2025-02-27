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

Msg Replica::recv_msg() {
  Msg msg;
  int clientSock, err;
  char buff[BUFF_SIZE] = {0};
  std::string sbuff;

  clientSock = accept(this->listeningSock, nullptr, nullptr);

  recv(clientSock, buff, sizeof(buff), 0);
  sbuff = buff;

  // if message type is of class Msg
  if (sbuff == "MSG") {
    recv(clientSock, &msg.viewNumber, sizeof(msg.viewNumber), 0);
    recv(clientSock, &msg.type, sizeof(msg.type), 0);
    memset(buff, 0, sizeof(buff));
    err = recv(clientSock, buff, sizeof(buff), 0);
    buff[err] = '\0';
    msg.node->cmd = buff;

    memset(buff, 0, sizeof(buff));
    err = recv(clientSock, buff, sizeof(buff), 0);
    buff[err] = '\0';
    sbuff = buff;
    if (sbuff == "QCNOTNULL") {
      recv(this->sendingSock, &msg.justify->type, sizeof(msg.justify->type), 0);
      recv(this->sendingSock, &msg.justify->viewNumber,
           sizeof(msg.justify->viewNumber), 0);
      err = recv(clientSock, buff, sizeof(buff), 0);
      buff[err] = '\0';
      msg.justify->node->cmd = buff;
    } else {
      msg.justify = nullptr;
    }
  }

  close(this->listeningSock);

  return msg;
}

// function to send an object of class Msg
void Replica::send_msg(int idx, Msg *msg) {
  int err;
  char buff[BUFF_SIZE] = {0};
  strcpy(buff, "MSG");

  err = connect(this->sendingSock, (struct sockaddr *)&otherAddrs[idx],
                sizeof(otherAddrs[idx]));
  if (err != 0) {
    std::cout << std::strerror(errno) << '\n';
    exit(1);
  }

  // send Msg object
  send(this->sendingSock, buff, sizeof(buff), 0);
  send(this->sendingSock, &msg->viewNumber, sizeof(msg->viewNumber), 0);
  send(this->sendingSock, &msg->type, sizeof(msg->type), 0);
  send(this->sendingSock, msg->node->cmd.c_str(), msg->node->cmd.length(), 0);
  if (msg->justify != nullptr) {
    memset(buff, '\0', sizeof(buff));
    strcpy(buff, "QCNOTNULL");
    send(this->sendingSock, buff, sizeof(buff), 0);
    send(this->sendingSock, &msg->justify->type, sizeof(msg->justify->type), 0);
    send(this->sendingSock, &msg->justify->viewNumber,
         sizeof(msg->justify->viewNumber), 0);
    send(this->sendingSock, msg->justify->node->cmd.c_str(),
         msg->justify->node->cmd.length(), 0);
  } else {
    memset(buff, '\0', sizeof(buff));
    strcpy(buff, "QCNULL");
    send(this->sendingSock, buff, sizeof(buff), 0);
  }
}

// function to view object of class Msg
void Replica::view_msg(Msg *msg) {
  std::cout << "MSG\n";
  std::cout << "viewNumber: " << msg->viewNumber << '\n';
  std::cout << "type: " << enum_as_str(msg->type) << '\n';
  std::cout << "Node: \n";
  std::cout << "\tcmd: " << msg->node->cmd << '\n';
  if (msg->justify != nullptr) {
    std::cout << "QC: \n";
    std::cout << "\tviewNumber: " << msg->justify->viewNumber << '\n';
    std::cout << "\ttype: " << enum_as_str(msg->justify->type) << '\n';
    std::cout << "\tNode: \n";
    std::cout << "\t\tcmd" << msg->justify->node->cmd;
  }
}

// function to convert enum to string
std::string Replica::enum_as_str(mtype type) {
  switch (type) {
  case NEW_VIEW:
    return "NEW VIEW";
  case PREPARE:
    return "PREPARE";
  case PRE_COMMIT:
    return "PRE COMMIT";
  case DECIDE:
    return "DECIDE";
  case COMMIT:
    return "COMMIT";
    break;
  default:
    return "UNKNOWN";
  }
}

// function to return pointer to class variable utils
Utilities *Replica::get_utils() { return &this->utils; }
