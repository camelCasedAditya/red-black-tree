#ifndef TREE_H //beginning of header guard
#define TREE_H
#include <iostream>
#include <cstring>
using namespace std;

class Tree{
  // initalize the functions for the node class
 public:
  Tree();
  ~Tree();
  void setValue(int newvalue);
  int getValue();
  void setColor(int newcolor);
  int getColor();
  void setLeft(Tree* newnext);
  Tree* getLeft();
  void setRight(Tree* newnext);
  Tree* getRight();
  void setPrevious(Tree* newprevious);
  Tree* getPrevious();
  Tree* getSibling();

    // Node object attributes
 private:
  int value;
  int color;
  Tree* left;
  Tree* right;
  Tree* previous;
};
#endif
