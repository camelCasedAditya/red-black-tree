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
void rotateLeft(Tree* pivot);
void rotateRight(Tree* pivot);
void deleteNode(int val, int search);
void replaceNode(Tree* node, Tree* replacement);
void fixTree(int color, Tree* node);
void moveDoubleBlack(Tree* initial, Tree* result);
int BLACKCOLOR = 1;
int REDCOLOR = 2;
int DOUBLEBLACK = 3;

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
    cout << "\'DELETE\' to delete a number" << endl;
    cout << "\'SEARCH\' to search for a number" << endl;
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
    else if (strcmp(input, "DELETE") == 0) {
      int num;
      // get number user wants to de;ete and makes sure its valid
      cout << "Enter the number you want to delete" << endl;
      cin >> num;
      if (num > 1000 || num < 1) {
        cout << "INVALID NUM: " << num << endl;
      }
      else {
        // call delete function to delete number from the tree
        deleteNode(num, 0);
      }
    }

    else if (strcmp(input, "SEARCH") == 0) {
      int num;
      // get number user wants to search for and makes sure its valid
      cout << "Enter the number you want to delete" << endl;
      cin >> num;
      if (num > 1000 || num < 1) {
        cout << "INVALID NUM: " << num << endl;
      }
      else {
        // call search function to search for number in the tree
        deleteNode(num, 1);
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
  if (pos == NULL) {
    return;
  }
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
  else if (pos->getColor() == DOUBLEBLACK) {
    cout << "\033[33m" << pos->getValue() << "\033[33m" << endl;
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

void deleteNode(int val, int search) {
  Tree* current = head;
  if (current == NULL) {
    return;
  }
  
  while(current != NULL && current->getValue() != val) {
    if (val > current->getValue() && current->getRight() != NULL) {
      current = current->getRight();
    }
    else if (val < current->getValue() && current->getLeft() != NULL) {
      current = current->getLeft();
    }
    else {
      break;
    }
  }
  
  if (current == NULL || current->getValue() != val) {
    cout << "That number DOES NOT exist in the tree" << endl;
    return;
  }
  else if (search == 1) {
    cout << "That number DOES exist in the tree!" << endl;
    return;
  }
  
  int originalColor = current->getColor();
  
  if (current->getLeft() == NULL) {
    Tree* replacement = current->getRight();
    
    if ((replacement == NULL && current->getColor() == BLACKCOLOR) || (replacement != NULL && replacement->getColor() == BLACKCOLOR && current->getColor() == BLACKCOLOR)) {
      replacement = new Tree();
      replacement->setValue(0);
      replacement->setColor(DOUBLEBLACK);
      replacement->setPrevious(current->getPrevious());
    }
    
    replaceNode(current, replacement);
    fixTree(originalColor, replacement);
    delete current;
  }
  else if (current->getRight() == NULL) {
    Tree* replacement = current->getLeft();
    
    if ((replacement == NULL && current->getColor() == BLACKCOLOR) || (replacement != NULL && replacement->getColor() == BLACKCOLOR && current->getColor() == BLACKCOLOR)) {
      replacement = new Tree();
      replacement->setValue(0);
      replacement->setColor(DOUBLEBLACK);
      replacement->setPrevious(current->getPrevious());
    }
    
    replaceNode(current, replacement);
    fixTree(originalColor, replacement);
    delete current;
  }
  else {
    Tree* temp = current->getLeft();
    while(temp->getRight() != NULL) {
      temp = temp->getRight();
    }
    
    int tempOriginalColor = temp->getColor();
    
    Tree* replacementChild = temp->getLeft();
    
    if (tempOriginalColor == BLACKCOLOR && (replacementChild == NULL || replacementChild->getColor() == BLACKCOLOR)) {
      Tree* doubleBlack = new Tree();
      doubleBlack->setValue(0);
      doubleBlack->setPrevious(temp->getPrevious());
      doubleBlack->setColor(DOUBLEBLACK);
      replacementChild = doubleBlack;
    }
    replaceNode(temp, replacementChild);
    replaceNode(current, temp);
    
    temp->setRight(current->getRight());
    if (temp->getRight() != NULL) {
      temp->getRight()->setPrevious(temp);
    }

    temp->setLeft(current->getLeft());
    if (temp->getLeft() != NULL) {
      temp->getLeft()->setPrevious(temp);
    }
    
    temp->setColor(originalColor);
    if (tempOriginalColor == BLACKCOLOR) {
      fixTree(BLACKCOLOR, replacementChild);
    }
    
    delete current;
  }
  if (head != NULL) {
    head->setColor(BLACKCOLOR);
  }
  cout << "Deletion of " << val << " completed successfully" << endl;
}

void replaceNode(Tree* node, Tree* replacement) {
  if (node->getPrevious() == NULL) {
    head = replacement;
  }
  else if (node->getPrevious()->getLeft() == node) {
    node->getPrevious()->setLeft(replacement);
  }
  else {
    node->getPrevious()->setRight(replacement);
  }
  //cout << node->getPrevious()->getValue() << endl;
  if(replacement != NULL) {
    replacement->setPrevious(node->getPrevious());
  }
  //cout << node->getPrevious()->getValue() << endl;
  return;
}

// Function to fix the tree after the node has been deleted
void fixTree(int color, Tree* node) {
  if (node == NULL) {
    return;
  }
  if (node == head) {
    node->setColor(BLACKCOLOR);
    return;
  }
  if (color == REDCOLOR || (node != NULL && node->getColor() == REDCOLOR)) {
    node->setColor(BLACKCOLOR);
    return;
  }
  Tree* current = node;
  while (current != NULL && current!=head && current->getColor() == DOUBLEBLACK) {
    Tree* parent = current->getPrevious();
    if (parent == NULL) {
      current->setColor(BLACKCOLOR);
      break;
    }
    
    Tree* sibling = current->getSibling();
    if (sibling != NULL && sibling->getColor() == REDCOLOR) {
      sibling->setColor(BLACKCOLOR);
      parent->setColor(REDCOLOR);
      if (current == parent->getLeft()) {
        rotateLeft(parent);
      }
      else {
        rotateRight(parent);
      }
      sibling = current->getSibling();
    }
    if (sibling != NULL && sibling->getColor() == BLACKCOLOR && 
    (sibling->getLeft() == NULL || sibling->getLeft()->getColor() == BLACKCOLOR) &&
    (sibling->getRight() == NULL || sibling->getRight()->getColor() == BLACKCOLOR)) {
      sibling->setColor(REDCOLOR);
      if (parent->getColor() == REDCOLOR) {
        parent->setColor(BLACKCOLOR);
        current->setColor(BLACKCOLOR);
        break;
      }
      else {
        moveDoubleBlack(current, parent);
        current = parent;
      }
    }
    else if (sibling != NULL && sibling->getColor() == BLACKCOLOR) {
      if (sibling == parent->getLeft() && sibling->getLeft() != NULL && sibling->getLeft()->getColor() == REDCOLOR) {
        sibling->setColor(parent->getColor());
        parent->setColor(BLACKCOLOR);
        sibling->getLeft()->setColor(BLACKCOLOR);
        rotateRight(parent);
        current->setColor(BLACKCOLOR);
        break;
      }
      else if (sibling == parent->getRight() && sibling->getRight() != NULL && sibling->getRight()->getColor() == REDCOLOR) {
        sibling->setColor(parent->getColor());
        parent->setColor(BLACKCOLOR);
        sibling->getRight()->setColor(BLACKCOLOR);
        rotateLeft(parent);
        current->setColor(BLACKCOLOR);
        break;
      }
      else if (sibling == parent->getLeft() && sibling->getRight() != NULL && sibling->getRight()->getColor() == REDCOLOR) {
        sibling->getRight()->setColor(parent->getColor());
        rotateLeft(sibling);
        rotateRight(parent);
        parent->setColor(BLACKCOLOR);
        current->setColor(BLACKCOLOR);
        break;
      }
      else if (sibling == parent->getRight() && sibling->getLeft() != NULL && sibling->getLeft()->getColor() == REDCOLOR) {
        sibling->getLeft()->setColor(parent->getColor());
        rotateRight(sibling);
        rotateLeft(parent);
        parent->setColor(BLACKCOLOR);
        current->setColor(BLACKCOLOR);
        break;
      }
    }
    if (sibling == NULL) {
      if (parent->getColor() == BLACKCOLOR) {
        moveDoubleBlack(current, parent);
        current = parent;
      }
      else {
        parent->setColor(BLACKCOLOR);
        current->setColor(BLACKCOLOR);
        break;
      }
    }
  }
  
  if (node->getValue() == 0) {
    Tree* parent = node->getPrevious();
    if (parent != NULL) {
      if (parent->getLeft() == node) {
        parent->setLeft(NULL);
      } 
      else {
        parent->setRight(NULL);
      }
      delete node;
    }
  }
  
  if (head != NULL) {
    head->setColor(BLACKCOLOR);
  }
  return;
}

// Function to move a double black color placeholder from one node to another
void moveDoubleBlack(Tree* initial, Tree* result) {
  // Set the color of the result node to DOUBLEBLACK
  result->setColor(DOUBLEBLACK);

  // If the initial node was a NULL placeholder node we delete it
  if (initial->getValue() == 0) {
    // We set the previous node's left or right to null because the placeholder node is deleted.
    if (initial->getPrevious() != NULL) {
      if (initial == initial->getPrevious()->getLeft()) {
        initial->getPrevious()->setLeft(NULL);
      }
      else {
        initial->getPrevious()->setRight(NULL);
      }
    }
    // delete the placeholder node
    delete initial;
  }
  else {
    // If the node is not a NULL placeholder we set its color to black
    initial->setColor(BLACKCOLOR);
  }
  return;
}

void case5(Tree* node) {
  // Exit the function if required nodes are NULL
  if (node == NULL || node->getPrevious() == NULL || node->getPrevious()->getPrevious() == NULL) {
    return;
  }
  if(node->getPrevious()->getRight() == node && node->getPrevious()->getPrevious()->getLeft() == node->getPrevious()) {
    rotateLeft(node->getPrevious());
    case6(node);
  }
  else if (node->getPrevious()->getLeft() == node && node->getPrevious()->getPrevious()->getRight() == node->getPrevious()) {
    rotateRight(node->getPrevious());
    case6(node);
  }
}

// Case 6 function 
void case6(Tree* node) {
  // Exit if the node is null
  if (node == NULL ) {
    return;
  }
  if(node->getLeft() == NULL && node->getRight() == NULL) {
    return;
  }
  cout << node->getValue() << endl;
  if (node->getPrevious()->getLeft() == node /*&& node->getPrevious()->getPrevious()->getLeft() == node->getPrevious()*/) {
    rotateRight(node->getPrevious());
    node->getRight()->setColor(REDCOLOR);
  }
  else if (node->getPrevious()->getRight() == node /*&& node->getPrevious()->getPrevious()->getRight() == node->getPrevious()*/) {
    cout << node->getValue() << endl;
    rotateLeft(node->getPrevious());
    node->getLeft()->setColor(REDCOLOR);
  }

  node->setColor(BLACKCOLOR);
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

void rotateLeft(Tree* pivot) {
  if (pivot == NULL || pivot->getRight() == NULL) {
    return;
  }

  Tree* newParent = pivot->getRight();
  pivot->setRight(newParent->getLeft());
  if (newParent->getLeft() != NULL) {
    newParent->getLeft()->setPrevious(pivot);
  }
  newParent->setPrevious(pivot->getPrevious());
  if (pivot->getPrevious() == NULL) {
    head = newParent; 
  }
  else if (pivot == pivot->getPrevious()->getLeft()){
    pivot->getPrevious()->setLeft(newParent);
  }
  else {
    pivot->getPrevious()->setRight(newParent);
  }
  newParent->setLeft(pivot);
  pivot->setPrevious(newParent);
}

void rotateRight(Tree* pivot) {
  if (pivot == NULL || pivot->getLeft() == NULL) {
    return;
  }
  Tree* newParent = pivot->getLeft();
  pivot->setLeft(newParent->getRight());
  if (newParent->getRight() != NULL) {
    newParent->getRight()->setPrevious(pivot);
  }
  newParent->setPrevious(pivot->getPrevious());
  if (pivot->getPrevious() == NULL) {
    head = newParent;
  }
  else if (pivot == pivot->getPrevious()->getRight()){
    pivot->getPrevious()->setRight(newParent);
  }
  else {
    pivot->getPrevious()->setLeft(newParent);
  }
  newParent->setRight(pivot);
  pivot->setPrevious(newParent);
}
