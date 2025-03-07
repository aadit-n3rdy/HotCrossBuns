#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

enum mtype { NEW_VIEW, PREPARE, PRE_COMMIT, COMMIT, DECIDE };

class Node {
public:
  std::string cmd;
  Node *parent;
};

class QuroumCertificate {
public:
  QuroumCertificate();
  mtype type;
  int viewNumber;
  Node *node;
  std::vector<std::string> sig;
};

class Msg {
private:
public:
  Msg();

  int viewNumber;
  mtype type;
  Node *node;
  QuroumCertificate *justify;
  std::vector<std::string> partialSig;
};

class Utilities {
private:
  int curView;
  QuroumCertificate *lockedQC;
  QuroumCertificate *highQC;

  int node_extends(QuroumCertificate *qc);

public:
  Msg *create_msg(mtype type, Node *node, QuroumCertificate *qc);

  Msg *vote_msg(mtype type, Node *node, QuroumCertificate *qc);

  Node *create_leaf(std::string cmd, Node *parent);

  QuroumCertificate *create_qc(Msg *msg);

  int matching_msg(Msg *m, mtype type, int viewNumber);

  int matching_qc(QuroumCertificate *qc, mtype type, int viewNumber);

  int safenode(Node *node, QuroumCertificate *qc);

  void HotBuns();
};

#endif // !UTILS_HPP
