#include <iostream>
#include <cstring>
#include "Tree.h"

using namespace std;

void add(Tree*& head, Tree* node, int num);
Tree* head = NULL;
void printFormatted(Tree* pos, int depth);
void case2(Tree* child);
void case4(Tree* node);
void case5(Tree* node);
void case6(Tree* node);
int BLACKCOLOR = 1;
int REDCOLOR = 2;

int main() {
  //add(head, head, 10);
  //printFormatted(head, 0);
  //cout << "1" << endl;
  //add(head, head, 20);
  //  printFormatted(head, 0);
  //cout << "2" << endl;
  //add(head, head, 30);
  //  printFormatted(head, 0);
  //cout << "3" << endl;
  //add(head, head, 15);
  //  printFormatted(head, 0);
  //cout << "4" << endl;
  //add(head, head, 25);
  // printFormatted(head, 0);
  //cout << "5" << endl;
  //add(head, head, 5);
  //cout << "PASS" << endl;
  //printFormatted(head, 0);
  int input = -1;
  while (input != 0) {
    cout << "Enter a number: " << endl;
    cin >> input;
    add(head, head, input);
    printFormatted(head, 0);
  }
  return 0;
}

void add(Tree*& head, Tree* node, int num) {
  // if the head is null we set the new number to the head
  if (head == NULL) {
    head = new Tree();
    head->setValue(num);
    head->setColor(BLACKCOLOR);
    return;
  }
  // If the number is greater than the current node and the right is not null we move right
  if (num >= node->getValue() && node->getRight() != NULL) {
    add(head, node->getRight(), num);
    return;
  }
  // If the number is less tan the current node and the left is not null we move left
  else if (num < node->getValue() && node->getLeft() != NULL) {
    add(head, node->getLeft(), num);
    return;
  }
  // If the right is null and we want to move right we add the number on the right
  else if (num >= node->getValue() && node->getRight() == NULL) {
    Tree* temp = new Tree();
    temp->setValue(num);
    node->setRight(temp);
    temp->setPrevious(node);
    temp->setColor(REDCOLOR);
    case2(temp);
    case4(head);
    case5(temp);
    case6(temp->getPrevious());
    return;
  }
  // If the left is null and we want to move left we add the number on the left
  else if (num < node->getValue() && node->getLeft() == NULL) {
    Tree* temp = new Tree();
    temp->setValue(num);
    node->setLeft(temp);
    temp->setPrevious(node);
    temp->setColor(REDCOLOR);
    if (temp->getPrevious() != NULL && temp->getPrevious()->getColor() == REDCOLOR) {
      case2(temp);
      case4(head);
      //case5(temp);
      //case6(temp->getPrevious());
    }
    if (temp->getPrevious() != NULL && temp->getPrevious()->getColor() == REDCOLOR) {
      case5(temp);
    }
    if (temp->getPrevious() != NULL && temp->getPrevious()->getColor() == REDCOLOR) {
      case6(temp->getPrevious());
    }
    return;
  }
  return;
}


void printFormatted(Tree* pos, int depth) {
  // Go to right most number in the branch
  if (pos->getRight() != NULL) {
    printFormatted(pos->getRight(), depth+1);
  }

  // Prints out a tab for each depth to make tree shape
  for(int a = 0; a < depth; a+=1) {
    cout << '\t';
  }
  // prints out the value
  if(pos->getColor() == REDCOLOR) {
    cout << "\033[31m" << pos->getValue() << "\033[31m" << endl;
  }
  else {
    cout << "\033[37m" << pos->getValue() << "\033[37m" << endl;
  }
  // prints out the left numbers
  if(pos->getLeft() != NULL) {
    printFormatted(pos->getLeft(), depth+1);
  }
  cout << "\n";
}

// MAY NEED TO TAKE IN HEAD LATER
void case2(Tree* child) {
  cout << "in" << endl;
  Tree* node = child->getPrevious();
  if (node == head) {
    cout << "head" << endl;
    node->setColor(BLACKCOLOR);
    cout << "colored head" << endl;
    return;
  }
  cout << "pre if" << endl;
  if (node != NULL && node->getColor() == REDCOLOR && (node->getSibling() != NULL && node->getSibling()->getColor() == REDCOLOR)) {
    cout << "in here" << endl;
    node->setColor(BLACKCOLOR);
    cout << "black" << endl;
    node->getSibling()->setColor(BLACKCOLOR);
    cout << "black2" << endl;
    node->getPrevious()->setColor(REDCOLOR);
    cout << "red" << endl;
  }
  case4(head);
  cout << "p" << endl;
  if (node != NULL && node->getPrevious() != NULL) {
    case5(node->getPrevious());
  }
  cout << "d" << endl;
  if (node != NULL && node->getPrevious() != NULL && node->getPrevious()->getPrevious() != NULL) {
    case6(node->getPrevious()->getPrevious());
  }
  case2(node);
	//node->getPrevious();
  return;
}

void case4(Tree* node) {
  if (node == head) {
    node->setColor(BLACKCOLOR);
  }
}

void case5(Tree* node) {
  cout << "in here" << endl;
  Tree* parent = node->getPrevious();
  if (parent->getPrevious() != head) {
    if (node != NULL && node->getPrevious() != NULL && node->getPrevious()->getPrevious() != NULL) {
      if ((node->getPrevious()->getRight() == node
	   && node->getPrevious()->getPrevious()->getLeft() == node->getPrevious())
	  || (node->getPrevious()->getLeft() == node
	      && node->getPrevious()->getPrevious()->getRight() == node->getPrevious())
	  ) {
        Tree* parent = node->getPrevious();
        //parent->setRight(node->getLeft());
        if (parent->getPrevious()->getLeft() == parent) {
	  Tree* g = parent->getPrevious();
	  parent->setRight(node->getLeft());
	  if (node->getLeft() != NULL) {
	    node->getLeft()->setPrevious(parent);
	  }
	  parent->getPrevious()->setLeft(node);
	  node->setPrevious(g);
	  node->setLeft(parent);
	  parent->setPrevious(node);
	  cout << "Case 5->1" << endl;
	  case6(parent);
	  return;
        }
	cout << parent->getPrevious()->getValue() << endl;
        if (parent->getPrevious()->getRight() == parent) {
	  cout << "HERE" << endl;
	  Tree* g = parent->getPrevious();
	  parent->setLeft(node->getRight());
	  if (node->getRight() != NULL) {
	    node->getRight()->setPrevious(parent);
	  }
	  parent->getPrevious()->setRight(node);
	  node->setPrevious(g);
	  node->setRight(parent);
	  parent->setPrevious(node);
	  cout << "Case 5->2" << endl;
	  case6(parent);
	  return;
        }
      }
    }
  }
  if (parent->getPrevious() == head) {
    if (node != NULL && node->getPrevious() != NULL && node->getPrevious()->getPrevious() != NULL) {
      if ((node->getPrevious()->getRight() == node
           && node->getPrevious()->getPrevious()->getLeft() == node->getPrevious())
          || (node->getPrevious()->getLeft() == node
              && node->getPrevious()->getPrevious()->getRight() == node->getPrevious())
          ) {
        Tree* parent = node->getPrevious();
        //parent->setRight(node->getLeft());
        if (parent->getPrevious()->getLeft() == parent) {
          parent->setRight(node->getLeft());
	  if (node->getLeft() != NULL) {
	    node->getLeft()->setPrevious(parent);
	  }
          head->setLeft(node);
          node->setPrevious(head);
          node->setLeft(parent);
          parent->setPrevious(node);
          cout << "Case 5->1" << endl;
	  case6(parent);
	  return;
        }
        if (parent->getPrevious()->getRight() == parent) {
          Tree* g = parent->getPrevious();
          parent->setLeft(node->getRight());
	  if (node->getRight() != NULL) {
	    node->getRight()->setPrevious(parent);
	  }
          head->setRight(node);
          node->setPrevious(head);
          node->setRight(parent);
          parent->setPrevious(node);
          cout << "Case 5->2" << endl;
	  case6(parent);
	  return;
        }
      }
    }
  }
}

void case6(Tree* node) {
  cout << node->getValue() << endl;
  if (node->getPrevious() != head) {
    if (node->getPrevious() != NULL && node->getPrevious()->getPrevious() != NULL && (node->getLeft() != NULL || node->getRight() != NULL)) {
      if (node->getPrevious()->getLeft() == node && node->getLeft() != NULL) {
	Tree* gg = node->getPrevious()->getPrevious();
	Tree* g = node->getPrevious();
	Tree* u = node->getSibling();
	Tree* c = node->getRight();
 	node->getPrevious()->getPrevious()->setLeft(node);
	node->setPrevious(gg);
	node->setRight(g);
	g->setPrevious(node);
	node->getRight()->setRight(u);
	if (u != NULL) {
	  u->setPrevious(node->getRight());
	}
	node->getRight()->setLeft(c);
	if (c != NULL) {
	  c->setPrevious(node->getRight());
	}
	cout << "Case 6->1" << endl;
	node->setColor(BLACKCOLOR);
        g->setColor(REDCOLOR);

	return;
      }
      else if (node->getPrevious()->getRight() == node && node->getRight() != NULL) {
	Tree* gg = node->getPrevious()->getPrevious();
	Tree* g = node->getPrevious();
	Tree* u = node->getSibling();
	Tree* c = node->getLeft();
	node->getPrevious()->getPrevious()->setRight(node);
	node->setPrevious(gg);
	node->setLeft(g);
	g->setPrevious(node);
	node->getLeft()->setLeft(u);
	if (u != NULL) {
	  u->setPrevious(node->getLeft());
	}
	node->getLeft()->setRight(c);
	if (c != NULL) {
	  c->setPrevious(node->getLeft());
	}
	cout << "Case 6->2" << endl;
	node->setColor(BLACKCOLOR);
        g->setColor(REDCOLOR);
	return;
      }
    }
  }
  if (node->getPrevious() == head) {
    if (node->getPrevious() != NULL && (node->getLeft() != NULL || node->getRight() != NULL)) {
      if (node->getPrevious()->getLeft() == node && node->getLeft() != NULL) {
        Tree* g = node->getPrevious();
        Tree* u = node->getSibling();
        Tree* c = node->getRight();
        head = node;
	node->setPrevious(NULL);
        node->setRight(g);
	g->setPrevious(node);
        node->getRight()->setRight(u);
	if (u != NULL) {
	  u->setPrevious(node->getRight());
	}
        node->getRight()->setLeft(c);
	if (c != NULL) {
	  c->setPrevious(node->getRight());
	}
        cout << "Case 6->1" << endl;
	head->setColor(BLACKCOLOR);
	g->setColor(REDCOLOR);
	return;
      }
      else if (node->getPrevious()->getRight() == node && node->getRight() != NULL) {
        Tree* g = node->getPrevious();
        Tree* u = node->getSibling();
        Tree* c = node->getLeft();
        head = node;
	head->setPrevious(NULL);
        node->setLeft(g);
	g->setPrevious(node);
        node->getLeft()->setLeft(u);
	if (u != NULL) {
	  u->setPrevious(node->getLeft());
	}
	node->getLeft()->setRight(c);
	if (c != NULL) {
	  c->setPrevious(node->getLeft());
	}
        cout << "Case 6->2" << endl;
	head->setColor(BLACKCOLOR);
        g->setColor(REDCOLOR);
	return;
      }
    }
  }
}
