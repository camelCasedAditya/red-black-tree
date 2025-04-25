#include <iostream>
#include <cstring>
#include "Tree.h"
using namespace std;

// Node object constructor
Tree::Tree(){
  value = 0;
  color = 0;
  left = NULL;
  right = NULL;
  previous = NULL;
}

// Node object destructor
Tree::~Tree(){
  left = NULL;
  right = NULL;
  previous = NULL;
}

// function to set the value of a node
void Tree::setValue(int newvalue){
  value = newvalue;
}

// Function to get the value of a node
int Tree::getValue(){
  return value;
}

void Tree::setColor(int newcolor) {
  color = newcolor;
}

int Tree::getColor() {
  return color;
}

// Function to set the left child node
void Tree::setLeft(Tree* newnext){
  left = newnext;
}

// function to get the left child node
Tree* Tree::getLeft(){
  return left;
}

// Function to set the right child node
void Tree::setRight(Tree* newnext){
  right = newnext;
}

// function to get the right child node
Tree* Tree::getRight(){
  return right;
}

void Tree::setPrevious(Tree* newprevious) {
  previous = newprevious;
}
Tree* Tree::getPrevious() {
  return previous;
}

Tree* Tree::getSibling() {
  if (this->getPrevious()->getRight() == this) {
    return this->getPrevious()->getLeft();
  }
  else if (this->getPrevious()->getLeft() == this) {
    return this->getPrevious()->getRight();
  }
  else {
    return NULL;
  }
}
