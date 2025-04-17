#include <iostream>
#include <cstring>
#include <fstream>
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
  bool running = true;
  while (running == true) {
    char* input = new char[100];
    // prompt the user and inform them of possible commands
    cout << "Enter one of the following commands: " << endl;
    cout << "\'ADD\' to add numbers one by one" << endl;
    cout << "\'ADDFILE\' to add all numbers from a file (numbers.txt)." << endl;
    cout << "\'PRINT\' to print out the tree" << endl;
    cout << "\'QUIT\' to close the program" << endl;

    // get user input
    cin >> input;
    // If the user wants to add a number
    if (strcmp(input, "ADD") == 0) {
      int num;
      // get number user wants to add and makes sure its valid
      cout << "Enter the number you want to add" << endl;
      cin >> num;
      if (num > 1000 || num < 1) {
        cout << "INVALID NUM: " << num << endl;
      }
      else {
	// call add function to add number to the tree
	add(head, head, num);
      }
    }
    else if (strcmp(input, "ADDFILE") == 0) {
      // Opens file
      ifstream file("numbers.txt");
      int num;
      // Loops through all the numbers in the file
      while (file >> num) {
	// checks if number is valid
	if (num > 1000 || num < 1) {
	  cout << "INVALID NUM: " << num << endl;
	}
	else {
	  // add each number to the tree
	  add(head, head, num);
	}
      }
      file.close();
    }
    else if (strcmp(input, "PRINT") == 0) {
      // If user wants to print, print function is called
      printFormatted(head, 0);
    }
    else if (strcmp(input, "QUIT") == 0) {
      // sets running variable to false
      running = 0;
    }
    else {
      cout << "INVALID COMMAND" << endl;
    }
  }
  
  return 0;
}

// Function to add node to the tree
void add(Tree*& head, Tree* node, int num) {
  // If the tree is empty make the new node as head
  if (head == NULL) {
    head = new Tree();
    head->setValue(num);
    head->setColor(BLACKCOLOR);
    return;
  }
  // Move left or right down the tree based on if the number is larger or smaller than the current node's value
  if (num >= node->getValue() && node->getRight() != NULL) {
    add(head, node->getRight(), num);
    return;
  }
  else if (num < node->getValue() && node->getLeft() != NULL) {
    add(head, node->getLeft(), num);
    return;
  }
  else if (num >= node->getValue() && node->getRight() == NULL) {
    // Once we get to the end of the tree, add the new node with value as a red node
    Tree* temp = new Tree();
    temp->setValue(num);
    node->setRight(temp);
    temp->setPrevious(node);
    temp->setColor(REDCOLOR);

    // If the parent of the node is red we call case 2
    if (node->getColor() == REDCOLOR) {
      case2(temp);
    }

    // Ensures the head is black color
    case4(head);
    return;
  }

  // Same logic but when we add the new node as a left child
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


// Function to print out the red black tree formatted
void printFormatted(Tree* pos, int depth) {
  // Move all the way down to the right
  if (pos->getRight() != NULL) {
    printFormatted(pos->getRight(), depth+1);
  }
  for(int a = 0; a < depth; a+=1) {
    // For each depth print a tab
    cout << '\t';
  }
  if(pos->getColor() == REDCOLOR) {
    // If the node is red print it with red color
    cout << "\033[31m" << pos->getValue() << "\033[31m" << endl;
  }
  else {
    // if the node is black print it with white color
    cout << "\033[37m" << pos->getValue() << "\033[37m" << endl;
  }
  if(pos->getLeft() != NULL) {
    // Print all the left children
    printFormatted(pos->getLeft(), depth+1);
  }
  cout << "\033[37m" << "" << endl;
}


void case2(Tree* child) {
  // Exit function if some conditions are not met
  if (child == NULL || child->getPrevious() == NULL || child->getPrevious()->getPrevious() == NULL) {
    return;
  }

  // Define nodes needed to perform recoloring
  Tree* node = child->getPrevious();
  Tree* grandparent = node->getPrevious();
  Tree* uncle = node->getSibling();

  // If the parent and uncle is red
  if (node->getColor() == REDCOLOR) {
    if (uncle != NULL && uncle->getColor() == REDCOLOR) {
      // Change both the parent and uncle to black
      node->setColor(BLACKCOLOR);
      uncle->setColor(BLACKCOLOR);
      // Make the grand parent red and run recursively up the tree
      if (grandparent != head) {
        grandparent->setColor(REDCOLOR);
        case2(grandparent);
      } 
      else {
        grandparent->setColor(BLACKCOLOR);
      }
    } 
    else {
      // If the node is an inner grandchild we call case 5
      if ((node == grandparent->getLeft() && child == node->getRight()) || 
          (node == grandparent->getRight() && child == node->getLeft())) {
        case5(child);
      } 
      else {
	// Else we call case six because it is an outer grandchild
        case6(node);
      }
    }
  }
  return;
}


void case5(Tree* node) {
  // Exit the function if required nodes are NULL
  if (node == NULL || node->getPrevious() == NULL || node->getPrevious()->getPrevious() == NULL) {
    return;
  }
  // Define required nodes
  Tree* parent = node->getPrevious();
  Tree* grandparent = parent->getPrevious();

  // If the grandparent is not head
  if (parent != NULL && parent->getPrevious() != head) {
    // If the child is an inner grand child
    if (node != NULL && node->getPrevious() != NULL && node->getPrevious()->getPrevious() != NULL) {
      if ((node->getPrevious()->getRight() == node && node->getPrevious()->getPrevious()->getLeft() == node->getPrevious()) || (node->getPrevious()->getLeft() == node && node->getPrevious()->getPrevious()->getRight() == node->getPrevious())
	  ) {
        Tree* parent = node->getPrevious();
        //parent->setRight(node->getLeft());
        if (parent->getPrevious()->getLeft() == parent) {
	  //cout << "Works here" << endl;
          Tree* g = parent->getPrevious();
	  // Make the child the new parent and the parent the left child
          parent->setRight(node->getLeft());
          if (node->getLeft() != NULL) {
            node->getLeft()->setPrevious(parent);
          }
	  // Set the next and previous to ensure the tree is connected
          g->setLeft(node);
          node->setPrevious(g);
          node->setLeft(parent);
          parent->setPrevious(node);
	  // call case 6
          case6(parent->getPrevious());
          return;
        }
	// Same logic as before except for the opposite side
        if (parent->getPrevious()->getRight() == parent) {
          Tree* g = parent->getPrevious();
          parent->setLeft(node->getRight());
          if (node->getRight() != NULL) {
            node->getRight()->setPrevious(parent);
          }
          g->setRight(node);
          node->setPrevious(g);
          node->setRight(parent);
          parent->setPrevious(node);
          case6(parent->getPrevious());
          return;
        }
      }
    }
  }
  // Same logic as before except we rotate modify head
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
	  case6(parent->getPrevious());
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
	  case6(parent->getPrevious());
	  return;
        }
      }
    }
  }
}

// Case 6 function (Outer grand child)
void case6(Tree* node) {
  // Exit if the node is null
  cout << node->getValue() << endl;
  if (node == NULL) {
    return;
  }
  // If we are not rotating with head
  if (node->getPrevious() != head) {
    if (node->getPrevious() != NULL && node->getPrevious()->getPrevious() != NULL && (node->getLeft() != NULL || node->getRight() != NULL)) {
      if (node->getPrevious()->getLeft() == node && node->getLeft() != NULL) {
	// Set needed variables/nodes
	//cout << "in case 6" << endl;
	Tree* gg = node->getPrevious()->getPrevious();
	Tree* g = node->getPrevious();
	Tree* u = node->getSibling();
	Tree* c = node->getRight();
	// Make the parent the new grandparent
	if (gg->getLeft() == g) {
	  gg->setLeft(node);
	} 
	else {
	  gg->setRight(node);
	}
	node->setPrevious(gg);
	// Make the grandparent the right child
	node->setRight(g);
	g->setPrevious(node);
	// Set the uncle to the right child of the grand parent
	node->getRight()->setRight(u);
	if (u != NULL) {
	  u->setPrevious(node->getRight());
	}
	// Set the left of the old grandparent to the child node
	node->getRight()->setLeft(c);
	if (c != NULL) {
	  c->setPrevious(node->getRight());
	}
	// Recolor node and grandparent
	node->setColor(BLACKCOLOR);
        g->setColor(REDCOLOR);

	return;
      }
      // Same logic as before except for the opposite side
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
	node->setColor(BLACKCOLOR);
        g->setColor(REDCOLOR);
	return;
      }
    }
  }
  // Same logic as before except head is modifyed
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
	head->setColor(BLACKCOLOR);
        g->setColor(REDCOLOR);
	return;
      }
    }
  }
}

// Function to recolor head if it is black
void case4(Tree* node) {
  if (node == NULL) {
    return;
  }
  
  if (node == head) {
    node->setColor(BLACKCOLOR);
  }
}

//void rotateLeft(Tree* pivot) {
//if (pivot == NULL || pivot->getRight() == NULL) {
//  return;
//}

//Tree* newParent = pivot->getRight();
//if (newParent->getLeft() == != NULL) {
//  pivot->setRight(newParent->getLeft());
//  newParent->getLeft()->setPrevious(pivot);
//}
//newParent->setPrevious(pivot->getPrevious());
//if (pivot->getPrevious() == NULL) {
//  head = newParent; 
//}
//else if (pivot == pivot->getPrevious()->getLeft()){
//  newParent->getPrevious()->setLeft(newParent);
//}
//else {
//  newParent->getPrevious()->setRight(newParent);
//}
//newParent->setLeft(pivot);
//pivot->setPrevious(newParent);
//}
