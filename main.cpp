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
  // Sets up the deletion search for the target node
  Tree* current = head;
  if (current == NULL) {
    return;
  }

  // Move down the tree based on the target value until the correct node is reached
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

  // We we get to the end of the tree and the value is not found we tell the user that it is not in the tree
  if (current == NULL || current->getValue() != val) {
    cout << "That number DOES NOT exist in the tree" << endl;
    return;
  }
  // If the user wanted to only search for the number we tell them that the number is found
  else if (search == 1) {
    cout << "That number DOES exist in the tree!" << endl;
    return;
  }

  // Store the original color of the node that is being deleted
  int originalColor = current->getColor();

  // If the left child of the node is null
  if (current->getLeft() == NULL) {
    // Set the replacement child to the right node
    Tree* replacement = current->getRight();

    // If both the replacement and the node to be deleted are black
    if ((replacement == NULL && current->getColor() == BLACKCOLOR) || (replacement != NULL && replacement->getColor() == BLACKCOLOR && current->getColor() == BLACKCOLOR)) {
      // create a new null node for the replacement and set its color to double black
      replacement = new Tree();
      replacement->setValue(0);
      replacement->setColor(DOUBLEBLACK);
      replacement->setPrevious(current->getPrevious());
    }

    // Replace the node to be deleted with the replacement node
    replaceNode(current, replacement);

    // Fix the tree to maintain the red black tree
    fixTree(originalColor, replacement);

    // delete the target node
    delete current;
  }
  // If the right node is null we do the mirror of the previous if statement
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
    // If both children of the target node exist

    // Find the predesessor for the target node
    Tree* temp = current->getLeft();
    while(temp->getRight() != NULL) {
      temp = temp->getRight();
    }

    // Store the color of the replacment node
    int tempOriginalColor = temp->getColor();

    // Store the children of the replacement if there are any
    Tree* replacementChild = temp->getLeft();

    // Both the replacement node and its children are black
    if (tempOriginalColor == BLACKCOLOR && (replacementChild == NULL || replacementChild->getColor() == BLACKCOLOR)) {
      // create a double black node to replace the replacement node
      Tree* doubleBlack = new Tree();
      doubleBlack->setValue(0);
      doubleBlack->setPrevious(temp->getPrevious());
      doubleBlack->setColor(DOUBLEBLACK);
      replacementChild = doubleBlack;
    }
    // Call function to replace the replacement node with the doubleblack node
    replaceNode(temp, replacementChild);
    replaceNode(current, temp);

    // Set the left and right of the replacement in the new spot
    temp->setRight(current->getRight());
    if (temp->getRight() != NULL) {
      temp->getRight()->setPrevious(temp);
    }

    temp->setLeft(current->getLeft());
    if (temp->getLeft() != NULL) {
      temp->getLeft()->setPrevious(temp);
    }

    // If the replacement color originally was black we fix the tree
    temp->setColor(originalColor);
    if (tempOriginalColor == BLACKCOLOR) {
      fixTree(BLACKCOLOR, replacementChild);
    }

    // delete the target node
    delete current;
  }
  // Set head to black
  if (head != NULL) {
    head->setColor(BLACKCOLOR);
  }
  // Inform user of successful deletion
  cout << "Deletion of " << val << " completed successfully" << endl;
}

// Function to move replace a node with another node
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
  // If the node is NULL we cant run this function so we exit
  if (node == NULL) {
    return;
  }
  // If the node to fix is head we set it to black
  if (node == head) {
    node->setColor(BLACKCOLOR);
    return;
  }
  // If the deleted node is red the replacement is red we just need to recolor it to black
  if (color == REDCOLOR || (node != NULL && node->getColor() == REDCOLOR)) {
    node->setColor(BLACKCOLOR);
    return;
  }
  // Set up double black while loop to propagate up the tree as we fix it
  Tree* current = node;
  while (current != NULL && current!=head && current->getColor() == DOUBLEBLACK) {

    // Set and check for parent
    Tree* parent = current->getPrevious();
    if (parent == NULL) {
      current->setColor(BLACKCOLOR);
      break;
    }
    // Get sibling node
    Tree* sibling = current->getSibling();

    // If the sibling is red
    if (sibling != NULL && sibling->getColor() == REDCOLOR) {
      // Recolor sibling and parent
      sibling->setColor(BLACKCOLOR);
      parent->setColor(REDCOLOR);
      // Rotate the tree to make the sibling the parent
      if (current == parent->getLeft()) {
        rotateLeft(parent);
      }
      else {
        rotateRight(parent);
      }
      sibling = current->getSibling();
    }
    // If the sibling is black and and both of siblings kids are black
    if (sibling != NULL && sibling->getColor() == BLACKCOLOR && (sibling->getLeft() == NULL || sibling->getLeft()->getColor() == BLACKCOLOR) && (sibling->getRight() == NULL || sibling->getRight()->getColor() == BLACKCOLOR)) {
      sibling->setColor(REDCOLOR);
      // If the parent is red we recolor is to black along with the current node. This adds a black node to the path that has the double black
      if (parent->getColor() == REDCOLOR) {
        parent->setColor(BLACKCOLOR);
        current->setColor(BLACKCOLOR);
        break;
      }
      else {
	// If the parent is already black we move the doubleblack up the tree
        moveDoubleBlack(current, parent);
        current = parent;
      }
    }
    else if (sibling != NULL && sibling->getColor() == BLACKCOLOR) {
      // If the sibling is black and both children are NOT black


      // If the current node is the right child and the farther nephew is red
      if (sibling == parent->getLeft() && sibling->getLeft() != NULL && sibling->getLeft()->getColor() == REDCOLOR) {
	// Set the siblings color to the parents color and make the parent black
	sibling->setColor(parent->getColor());
        parent->setColor(BLACKCOLOR);
        sibling->getLeft()->setColor(BLACKCOLOR);

	// Rotate around the parent to make the sibling the new parent
        rotateRight(parent);

	// Set current to black.
        current->setColor(BLACKCOLOR);
        break;
      }
      else if (sibling == parent->getRight() && sibling->getRight() != NULL && sibling->getRight()->getColor() == REDCOLOR) {
	// Same logiic as previous case except it is mirrored to other side
        sibling->setColor(parent->getColor());
        parent->setColor(BLACKCOLOR);
        sibling->getRight()->setColor(BLACKCOLOR);
        rotateLeft(parent);
        current->setColor(BLACKCOLOR);
        break;
      }
      // If the closer nephew is red
      else if (sibling == parent->getLeft() && sibling->getRight() != NULL && sibling->getRight()->getColor() == REDCOLOR) {
	// Rotate around sibling to make the closer newphhew the new sibling
	sibling->getRight()->setColor(parent->getColor());
        rotateLeft(sibling);

	// Rotate around parent to make the new sibling the parent
        rotateRight(parent);
        parent->setColor(BLACKCOLOR);
        current->setColor(BLACKCOLOR);
        break;
      }
      else if (sibling == parent->getRight() && sibling->getLeft() != NULL && sibling->getLeft()->getColor() == REDCOLOR) {
	// Same logic as previous case but mirrored
	sibling->getLeft()->setColor(parent->getColor());
        rotateRight(sibling);
        rotateLeft(parent);
        parent->setColor(BLACKCOLOR);
        current->setColor(BLACKCOLOR);
        break;
      }
    }
    // If the sibling is null
    if (sibling == NULL) {
      // If parent is black we move the double black up to the parent and loop back to check the cases
      if (parent->getColor() == BLACKCOLOR) {
        moveDoubleBlack(current, parent);
        current = parent;
      }
      else {
	// If the parent is red we recolor the current and parent to black to balance the number of black nodes
        parent->setColor(BLACKCOLOR);
        current->setColor(BLACKCOLOR);
        break;
      }
    }
  }

  // Remove the temporary null node from the tree if it was needed
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

// Function to rotate the tree to the left
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

// Function to rotate the tree to the right
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
