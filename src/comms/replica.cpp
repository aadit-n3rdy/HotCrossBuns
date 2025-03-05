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

void Replica::close_socket() {
  close(this->listeningSock);
  close(this->sendingSock);
}

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

  // receive identifier length and identifier
  recv(clientSock, &err, sizeof(err), 0);
  recv(clientSock, buff, err, 0);
  sbuff = buff;

  // if message type is of class Msg
  if (sbuff == "MSG") {

    // receive view number
    recv(clientSock, &msg.viewNumber, sizeof(msg.viewNumber), 0);

    // receiver message type
    recv(clientSock, &msg.type, sizeof(msg.type), 0);

    // receive message node command
    recv(clientSock, &err, sizeof(err), 0);
    recv(clientSock, &buff[0], err, 0);
    buff[err] = '\0';
    msg.node->cmd = buff;

    // receive quorum certificate identifier length and quorum certificate
    // identifier
    recv(clientSock, &err, sizeof(err), 0);
    recv(clientSock, buff, err, 0);
    buff[err] = '\0';
    sbuff = buff;

    // quorum certificate exists
    if (sbuff == "QCNOTNULL") {

      // receive quorum type
      recv(this->sendingSock, &msg.justify->type, sizeof(msg.justify->type), 0);

      // receive quorum view number
      recv(this->sendingSock, &msg.justify->viewNumber,
           sizeof(msg.justify->viewNumber), 0);

      // receive quorum node cmd
      recv(clientSock, &err, sizeof(err), 0);
      recv(clientSock, buff, err, 0);
      buff[err] = '\0';
      msg.justify->node->cmd = buff;
    } else {

      // quorum certificate does not exist
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

  // connect to the socket
  err = connect(this->sendingSock, (struct sockaddr *)&otherAddrs[idx],
                sizeof(otherAddrs[idx]));
  if (err != 0) {
    std::cout << std::strerror(errno) << '\n';
    exit(1);
  }

  // send MSG identifier
  err = strlen(buff);
  send(this->sendingSock, &err, sizeof(err), 0);
  send(this->sendingSock, buff, err, 0);

  // send view number
  send(this->sendingSock, &msg->viewNumber, sizeof(msg->viewNumber), 0);

  // send message type
  send(this->sendingSock, &msg->type, sizeof(msg->type), 0);

  // send msg node command
  err = msg->node->cmd.length();
  send(this->sendingSock, &err, sizeof(err), 0);
  send(this->sendingSock, (char *)msg->node->cmd.c_str(),
       msg->node->cmd.length(), 0);

  // send quorum certificate
  if (msg->justify != nullptr) {
    // send identifier informing receiver that quroum certificate is not null
    strcpy(buff, "QCNOTNULL");
    err = strlen(buff);
    send(this->sendingSock, &err, sizeof(err), 0);
    send(this->sendingSock, buff, err, 0);

    // send quorum type
    send(this->sendingSock, &msg->justify->type, sizeof(msg->justify->type), 0);

    // send quorum view number
    send(this->sendingSock, &msg->justify->viewNumber,
         sizeof(msg->justify->viewNumber), 0);

    // send quorum node command string
    err = msg->justify->node->cmd.length();
    send(this->sendingSock, &err, sizeof(err), 0);
    send(this->sendingSock, msg->justify->node->cmd.c_str(),
         msg->justify->node->cmd.length(), 0);
  } else {

    // send identifier informing receiver that quroum certificate is null
    strcpy(buff, "QCNULL");
    err = strlen(buff);
    send(this->sendingSock, &err, sizeof(err), 0);
    send(this->sendingSock, buff, err, 0);
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

// function to declare self as leader

// function to return pointer to class variable utils
Utilities *Replica::get_utils() { return &this->utils; }
