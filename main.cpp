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
  if (head == NULL) {
    head = new Tree();
    head->setValue(num);
    head->setColor(BLACKCOLOR);
    return;
  }
  if (num >= node->getValue() && node->getRight() != NULL) {
    add(head, node->getRight(), num);
    return;
  }
  else if (num < node->getValue() && node->getLeft() != NULL) {
    add(head, node->getLeft(), num);
    return;
  }
  else if (num >= node->getValue() && node->getRight() == NULL) {
    Tree* temp = new Tree();
    temp->setValue(num);
    node->setRight(temp);
    temp->setPrevious(node);
    temp->setColor(REDCOLOR);
    
    if (node->getColor() == REDCOLOR) {
      case2(temp);
    }
    
    case4(head);
    return;
  }

  else if (num < node->getValue() && node->getLeft() == NULL) {
    Tree* temp = new Tree();
    temp->setValue(num);
    node->setLeft(temp);
    temp->setPrevious(node);
    temp->setColor(REDCOLOR);
    
    if (node->getColor() == REDCOLOR) {
      case2(temp);
    }
    case4(head);
    return;
  }
  return;
}


void printFormatted(Tree* pos, int depth) {
  if (pos->getRight() != NULL) {
    printFormatted(pos->getRight(), depth+1);
  }
  for(int a = 0; a < depth; a+=1) {
    cout << '\t';
  }
  if(pos->getColor() == REDCOLOR) {
    cout << "\033[31m" << pos->getValue() << "\033[31m" << endl;
  }
  else {
    cout << "\033[37m" << pos->getValue() << "\033[37m" << endl;
  }
  if(pos->getLeft() != NULL) {
    printFormatted(pos->getLeft(), depth+1);
  }
  cout << "\n";
}

// MAY NEED TO TAKE IN HEAD LATER
void case2(Tree* child) {
  if (child == NULL || child->getPrevious() == NULL || child->getPrevious()->getPrevious() == NULL) {
    cout << "Exited due to a NULL node" << endl;
    return;
  }
  
  Tree* node = child->getPrevious();
  Tree* grandparent = node->getPrevious();
  Tree* uncle = node->getSibling();
  
  cout << "pre if" << endl;
  if (node->getColor() == REDCOLOR) {
    if (uncle != NULL && uncle->getColor() == REDCOLOR) {
      node->setColor(BLACKCOLOR);
      uncle->setColor(BLACKCOLOR);
      if (grandparent != head) {
        grandparent->setColor(REDCOLOR);
        case2(grandparent);
      } 
      else {
        grandparent->setColor(BLACKCOLOR);
      }
    } 
    else {
      if ((node == grandparent->getLeft() && child == node->getRight()) || 
          (node == grandparent->getRight() && child == node->getLeft())) {
        case5(child);
      } 
      else {
        case6(node);
      }
    }
  }
  return;
}


void case5(Tree* node) {
  if (node == NULL || node->getPrevious() == NULL || node->getPrevious()->getPrevious() == NULL) {
    return;
  }
  Tree* parent = node->getPrevious();
  Tree* grandparent = parent->getPrevious();
  
  if (parent != NULL && parent->getPrevious() != head) {
    if (node != NULL && node->getPrevious() != NULL && node->getPrevious()->getPrevious() != NULL) {
      if ((node->getPrevious()->getRight() == node && node->getPrevious()->getPrevious()->getLeft() == node->getPrevious()) || (node->getPrevious()->getLeft() == node && node->getPrevious()->getPrevious()->getRight() == node->getPrevious())
	  ) {
        Tree* parent = node->getPrevious();
        //parent->setRight(node->getLeft());
        if (parent->getPrevious()->getLeft() == parent) {
          Tree* g = parent->getPrevious();
          parent->setRight(node->getLeft());
          if (node->getLeft() != NULL) {
            node->getLeft()->setPrevious(parent);
          }
          g->setLeft(node);
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
          g->setRight(node);
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
  if (parent != NULL && parent->getPrevious() == head) {
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
  if (node == NULL) {
    return;
  }
  cout << node->getValue() << endl;
  if (node->getPrevious() != head) {
    if (node->getPrevious() != NULL && node->getPrevious()->getPrevious() != NULL && (node->getLeft() != NULL || node->getRight() != NULL)) {
      if (node->getPrevious()->getLeft() == node && node->getLeft() != NULL) {
	Tree* gg = node->getPrevious()->getPrevious();
	Tree* g = node->getPrevious();
	Tree* u = node->getSibling();
	Tree* c = node->getRight();
  if (gg->getLeft() == g) {
    gg->setLeft(node);
  } 
  else {
    gg->setRight(node);
  }
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
	if (gg->getLeft() == g) {
    gg->setLeft(node);
  } 
  else {
    gg->setRight(node);
  }
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

void case4(Tree* node) {
  if (node == NULL) {
    return;
  }
  
  if (node == head) {
    node->setColor(BLACKCOLOR);
  }
}
