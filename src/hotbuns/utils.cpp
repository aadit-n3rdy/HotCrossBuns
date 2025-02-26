#include "hotbuns/utils.hpp"

// function to check if a passed npde extends from the lockedQC node
// TODO: currently function returns 1 by default
// change function to check if lockedQC node extends to qc
int Utilities::node_extends(QuroumCertificate *qc) { return 1; }

// function to create a message
Msg *Utilities::create_msg(mtype type, Node node, QuroumCertificate qc) {
  Msg *msg = new Msg();

  msg->type = type;
  msg->viewNumber = this->curView;
  msg->node = node;
  msg->justify = qc;

  return msg;
}

// function to sign a msg for voting
// NOTE: currently signing is not secure
Msg *Utilities::vote_msg(mtype type, Node node, QuroumCertificate qc) {
  Msg *msg = create_msg(type, node, qc);
  msg->partialSig.push_back(node.cmd);

  return msg;
}

// function to create a leaf
Node *create_leaf(Node *parent, std::string cmd) {
  Node *b = new Node();

  b->parent = parent;
  b->cmd = cmd;

  return b;
}

// function to generate a quorum certificate
QuroumCertificate *Utilities::create_qc(Msg *msg) {
  QuroumCertificate *qc = new QuroumCertificate();

  qc->type = msg->type;
  qc->viewNumber = msg->viewNumber;
  qc->node = msg->node;
  // TODO: add tcombine facility
  qc->sig = msg->partialSig;

  return qc;
}

// function to check if msgs are messing
int Utilities::matching_msg(Msg *m, mtype type, int viewNumber) {
  return (m->type == type) && (m->viewNumber == viewNumber);
}

// function to check for matching quorum certificates
int Utilities::matching_qc(QuroumCertificate *qc, mtype type, int viewNumber) {
  return (qc->type == type) && (qc->viewNumber == viewNumber);
}

// predicate to check if a node is safe to accept
int Utilities::safenode(Node *node, QuroumCertificate *qc) {
  return this->node_extends(qc) ||
         (qc->viewNumber > this->lockedQC->viewNumber);
}
