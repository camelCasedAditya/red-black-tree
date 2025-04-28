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
void deleteNode(int val);
void replaceNode(Tree* node, Tree* replacement);
void fixTree(int color, Tree* node);
void moveDoubleBlack(Tree* initial, Tree* result);
void testRedBlackTreeDeletionCases();
int BLACKCOLOR = 1;
int REDCOLOR = 2;
int DOUBLEBLACK = 3;

int main() {
  int input = -1;
  bool running = true;
  //testRedBlackTreeDeletionCases();
  while (running == true) {
    char* input = new char[100];
    // prompt the user and inform them of possible commands
    cout << "Enter one of the following commands: " << endl;
    cout << "\'ADD\' to add numbers one by one" << endl;
    cout << "\'ADDFILE\' to add all numbers from a file (numbers.txt)." << endl;
    cout << "\'PRINT\' to print out the tree" << endl;
    cout << "\'DELETE\' to delete a number" << endl;
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
    if (strcmp(input, "DELETE") == 0) {
      int num;
      // get number user wants to add and makes sure its valid
      cout << "Enter the number you want to delete" << endl;
      cin >> num;
      if (num > 1000 || num < 1) {
        cout << "INVALID NUM: " << num << endl;
      }
      else {
        // call add function to add number to the tree
        deleteNode(num);
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
	cout << "Val: " << node->getValue() << endl;
      }
    }
  }
  return;
}

void deleteNode(int val) {
  Tree* current = head;
  if (current == NULL) {
    cout << "Tree is empty" << endl;
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
    cout << "That number does not exist in the tree" << endl;
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
    
    if (temp->getPrevious() != current) {
      replaceNode(temp, replacementChild);
      temp->setLeft(current->getLeft());
      if (temp->getLeft() != NULL) {
        temp->getLeft()->setPrevious(temp);
      }
    }
    replaceNode(current, temp);
    
    temp->setRight(current->getRight());
    if (temp->getRight() != NULL) {
      temp->getRight()->setPrevious(temp);
    }
    
    temp->setColor(originalColor);
    if (tempOriginalColor == BLACKCOLOR && replacementChild != NULL) {
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
  cout << "INTO REPLACE NODE FUNCTION" << endl;
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

void fixTree(int color, Tree* node) {
  cout << "Into FIX TREE FUNCTION" << endl;
  if (node == NULL) {
    cout << "NULL NODE" << endl;
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
    cout << "loop" << endl;
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

void moveDoubleBlack(Tree* initial, Tree* result) {
  result->setColor(DOUBLEBLACK);
  if (initial->getValue() == 0) {
    if (initial->getPrevious() != NULL) {
      if (initial == initial->getPrevious()->getLeft()) {
        initial->getPrevious()->setLeft(NULL);
      }
      else {
        initial->getPrevious()->setRight(NULL);
      }
    }
    delete initial;
  }
  else {
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
  cout << "case 6 " << endl;
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
    cout << "rotated" << endl;
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
    cout << "failed conditions" << endl;
    return;
  }
  cout << "right" << endl;
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

// THIS FUNCTION IS GENERATED BY AI TO TEST MY TREE
void testRedBlackTreeDeletionCases() {
  cout << "\n\n===== Starting RBT Deletion Test Sequence (Tracing User's Code) =====\n" << endl;

  // Clear tree if needed (assuming it starts NULL or is cleared)
  head = NULL; // Simple clear for test

  cout << "--- Phase 1: Setup and Basic Deletions ---" << endl;
  add(head, head, 10); cout << "ADD 10" << endl;
  add(head, head, 20); cout << "ADD 20" << endl;
  add(head, head, 5);  cout << "ADD 5" << endl;
  add(head, head, 15); cout << "ADD 15" << endl;
  add(head, head, 25); cout << "ADD 25" << endl;
  add(head, head, 30); cout << "ADD 30" << endl;
  cout << "\nInitial Tree State (After Phase 1 Adds):" << endl;
  if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;

  cout << "\n--- DELETE 30 ---" << endl;
  cout << "// Expected Behavior (Your Code): Node 30 is RED. deleteNode calls replaceNode." << endl;
  cout << "// fixTree might be called (if called unconditionally) with replacement=NULL." << endl;
  cout << "// fixTree(NULL) should likely return quickly (or print 'NULL NODE'). No double black involved." << endl;
  deleteNode(30);
  cout << "Tree State After Deleting 30:" << endl;
  if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;

  cout << "\n--- DELETE 25 ---" << endl;
  cout << "// Expected Behavior (Your Code): Node 25 is BLACK, Parent 20 is RED. Replacement is NULL." << endl;
  cout << "// deleteNode should create artificial node (value=0, color=DOUBLEBLACK)." << endl;
  cout << "// replaceNode inserts this '0' node. fixTree is called with '0' node." << endl;
  cout << "// fixTree(node=0): current=0(DB), parent=20(R), sibling=15(B). Children of 15 are NULL(B)." << endl;
  cout << "// -> Should hit Case 3 logic (Sibling Black, Children Black)." << endl;
  cout << "// -> Inside Case 3: Since parent(20) is RED, it should recolor parent(20) BLACK, maybe recolor sibling(15) RED, and terminate." << endl;
  cout << "// -> The artificial '0' node might persist or be handled by fixTree/deleteNode." << endl;
  deleteNode(25);
  cout << "Tree State After Deleting 25:" << endl;
  if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl; // Observe if '0' is present

  cout << "\n--- DELETE 15 ---" << endl;
  cout << "// Expected Behavior (Your Code): Node 15 is likely RED (recolored in previous step)." << endl;
  cout << "// deleteNode calls replaceNode. fixTree might be called with replacement=NULL." << endl;
  cout << "// fixTree(NULL) should return quickly. No double black involved." << endl;
  deleteNode(15);
  cout << "Tree State After Deleting 15:" << endl;
  if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;

  cout << "\n--- DELETE 5 ---" << endl;
  cout << "// Expected Behavior (Your Code): Node 5 is BLACK, Parent 10 is BLACK." << endl;
  cout << "// deleteNode creates artificial node '0'(DB). replaceNode inserts it." << endl;
  cout << "// fixTree(node=0): current=0(DB), parent=10(B), sibling=20(B - was recolored)." << endl;
  cout << "// -> Should hit Case 3 logic (Sibling Black, Children Black)." << endl;
  cout << "// -> Inside Case 3: Parent(10) is BLACK. Should recolor sibling(20) RED." << endl;
  cout << "// -> Should call moveDoubleBlack(initial=0, result=parent=10)." << endl;
  cout << "// -> moveDoubleBlack likely sets parent(10) to DOUBLEBLACK and removes '0'." << endl;
  cout << "// -> fixTree loop continues with current=10(DB). Parent=NULL (is head)." << endl;
  cout << "// -> Loop terminates (current==head). Root check sets head(10) BLACK." << endl;
  deleteNode(5);
  cout << "Tree State After Deleting 5:" << endl;
  if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;

  cout << "\n--- DELETE 20 ---" << endl;
  cout << "// Expected Behavior (Your Code): Node 20 is likely RED (recolored in previous step)." << endl;
  cout << "// fixTree might be called with replacement=NULL, should return quickly." << endl;
  deleteNode(20);
  cout << "Tree State After Deleting 20:" << endl;
  if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;


  cout << "\n--- Phase 2: More Complex Cases ---" << endl;
  // Tree should be just 10B now. Rebuild.
  head = NULL; cout << "Clearing Tree..." << endl; add(head, head, 10);
  add(head, head, 5);  cout << "ADD 5" << endl;
  add(head, head, 20); cout << "ADD 20" << endl;
  add(head, head, 15); cout << "ADD 15" << endl;
  add(head, head, 25); cout << "ADD 25" << endl;
  add(head, head, 3);  cout << "ADD 3" << endl;
  add(head, head, 7);  cout << "ADD 7" << endl;
  add(head, head, 1);  cout << "ADD 1" << endl;
  add(head, head, 4);  cout << "ADD 4" << endl;
  cout << "\nTree State (After Phase 2 Adds):" << endl;
  if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;

  // Deletes of RED leaves 1, 4, 7 will likely call fixTree(NULL) - skip detailed comments

  deleteNode(1); cout << "\nDELETE 1 (RED Leaf)\n";
  deleteNode(4); cout << "DELETE 4 (RED Leaf)\n";
  deleteNode(3); cout << "DELETE 3 (BLACK Leaf, Parent RED)\n"; // Similar to DELETE 25 case logic
  cout << "Tree State After Deleting 1, 4, 3:" << endl;
   if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;

  deleteNode(7); cout << "\nDELETE 7 (RED Leaf)\n";
   cout << "Tree State After Deleting 7:" << endl;
   if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;


  cout << "\n--- DELETE 5 ---" << endl;
  cout << "// Expected Behavior (Your Code): Node 5 BLACK, Parent 10 BLACK, Sibling 20 RED." << endl;
  cout << "// deleteNode creates artificial node '0'(DB)." << endl;
  cout << "// fixTree(node=0): current=0(DB), parent=10(B), sibling=20(R)." << endl;
  cout << "// -> Should hit Case 2 logic (Sibling Red)." << endl;
  cout << "// -> Inside Case 2: Recolor sibling(20) BLACK. Recolor parent(10) RED. Rotate parent(10) Left." << endl;
  cout << "// -> Sibling reference updates (new sibling is 15B)." << endl;
  cout << "// -> Code should continue within fixTree to handle black sibling case." << endl;
  cout << "// -> Now: current=0(DB), parent=10(R), sibling=15(B). Children NULL(B)." << endl;
  cout << "// -> Should hit Case 3 logic (Sibling Black, Children Black)." << endl;
  cout << "// -> Inside Case 3: Parent(10) is RED. Recolor parent(10) BLACK, maybe sibling(15) RED. Terminate." << endl;
  cout << "// -> Artificial '0' node fate depends on implementation details." << endl;
  deleteNode(5);
  cout << "Tree State After Deleting 5:" << endl;
   if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;

  add(head, head, 12); cout << "\nADD 12 (Setup for next delete)" << endl;
  cout << "Tree State After Adding 12:" << endl;
   if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;


  cout << "\n--- DELETE 25 ---" << endl;
  cout << "// Expected Behavior (Your Code): Node 25 BLACK, Parent 20 BLACK, Sibling 12 BLACK." << endl;
  cout << "// Tree state after ADD 12 should be: 20B / 12B \\ 25B | / \\ | 10R 15R" << endl;
  cout << "// deleteNode creates artificial node '0'(DB)." << endl;
  cout << "// fixTree(node=0): current=0(DB), parent=20(B), sibling=12(B)." << endl;
  cout << "// Sibling's children: near=10(R), far=15(R)." << endl;
  cout << "// -> Should hit Case 5 logic (Sibling Black, Far child Red)." << endl;
  cout << "// -> Inside Case 5: Recolor sibling(12) BLACK (parent's color). Recolor parent(20) BLACK. Recolor far child(15) BLACK." << endl;
  cout << "// -> Rotate parent(20) Right. Break from loop." << endl;
  cout << "// -> Artificial '0' node fate depends on implementation details." << endl;
  deleteNode(25);
  cout << "Tree State After Deleting 25:" << endl;
   if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;

  // Skip delete 15, 12 for brevity - they would involve propagation using artificial node + moveDoubleBlack

  cout << "\n--- Phase 3: Case 4 ---" << endl;
  head = NULL; cout << "Clearing tree for Phase 3..." << endl;
  add(head, head, 10); cout << "ADD 10" << endl;
  add(head, head, 5);  cout << "ADD 5" << endl;
  add(head, head, 20); cout << "ADD 20" << endl;
  add(head, head, 15); cout << "ADD 15" << endl;
  cout << "\nTree State (After Phase 3 Adds): 10B / 5B \\ 20B \\ 15R" << endl;
  if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;


  cout << "\n--- DELETE 5 ---" << endl;
  cout << "// Expected Behavior (Your Code): Node 5 BLACK, Parent 10 BLACK, Sibling 20 BLACK." << endl;
  cout << "// deleteNode creates artificial node '0'(DB)." << endl;
  cout << "// fixTree(node=0): current=0(DB), parent=10(B), sibling=20(B)." << endl;
  cout << "// Sibling's children: near=15(R), far=NULL(B)." << endl;
  cout << "// -> Should hit Case 4 logic (Sibling Black, Near child Red)." << endl;
  cout << "// -> Inside Case 4 (Right Sibling): Recolor near child(15) based on parent(10) color (BLACK?). Rotate sibling(20) Right. Rotate parent(10) Left. Recolor parent(10) BLACK. Break." << endl;
  cout << "//    (Check exact logic in your case 4 for recoloring)." << endl;
  cout << "// -> Artificial '0' node fate depends on implementation details." << endl;
  deleteNode(5);
  cout << "Tree State After Deleting 5:" << endl;
   if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;


  cout << "\n--- Phase 4: Deletion with Predecessor Cases ---" << endl;
  head = NULL; cout << "Clearing tree for Phase 4..." << endl;
  add(head, head, 50); add(head, head, 25); add(head, head, 75); add(head, head, 10);
  add(head, head, 30); add(head, head, 60); add(head, head, 80); add(head, head, 5);
  add(head, head, 35);
  cout << "\nTree State (After Phase 4 Adds):" << endl;
  if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;


  cout << "\n--- DELETE 25 ---" << endl;
  cout << "// Expected Behavior (Your Code): Delete Node(25R) with 2 children. Predecessor is 10B." << endl;
  cout << "// tempOriginalColor = BLACK. replacementChild = 5R." << endl;
  cout << "// replaceNode(10, 5). replaceNode(25, 10). 10->setColor(RED) (25's color)." << endl;
  cout << "// fixTree called because tempOriginalColor was BLACK. fixTree(BLACKCOLOR, 5R)." << endl;
  cout << "// -> Inside fixTree: Node 5R is RED. Recolor 5 to BLACK. Terminate." << endl;
  cout << "// -> No artificial node created in this specific scenario." << endl;
  deleteNode(25);
  cout << "Tree State After Deleting 25:" << endl;
   if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;


  cout << "\n--- DELETE 50 ---" << endl;
  cout << "// Expected Behavior (Your Code): Delete Node(50B) with 2 children. Predecessor is 35R." << endl;
  cout << "// tempOriginalColor = RED. replacementChild = NULL." << endl;
  cout << "// replaceNode(35, NULL). replaceNode(50, 35). 35->setColor(BLACK) (50's color)." << endl;
  cout << "// fixTree *not* called because tempOriginalColor was RED." << endl;
  cout << "// -> No artificial node created." << endl;
  deleteNode(50);
  cout << "Tree State After Deleting 50:" << endl;
   if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;

  // Cleanup deletion
  cout << "\n--- DELETE 60 ---" << endl;
   cout << "// Expected Behavior (Your Code): Node 60 BLACK, Parent 75 RED." << endl;
   cout << "// -> Creates artificial node '0'(DB)." << endl;
   cout << "// -> fixTree(node=0) hits Case 3 (Sibling 80B, Children NULL), Parent RED terminates." << endl;
  deleteNode(60);
  cout << "Tree State After Deleting 60:" << endl;
   if (head) printFormatted(head, 0); else cout << "Tree is empty." << endl;


  cout << "\n===== RBT Deletion Test Sequence (Tracing User's Code) Finished =====\n" << endl;
}