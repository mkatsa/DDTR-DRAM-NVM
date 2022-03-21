
#include <iostream>
#include <ctime>
#include <iomanip>

// **************************************************
// Adelson-Velskii and Landis dynamically balanced
// tree class.
class AVLTree {

  // - - - - - - - - - - - - - - - - - - - - - - - -
  // Private Node subclass for the nodes of the tree.
  private:
    class Node {

      // - - - - - - - - - - - - - - - - - - - - - -
      // Private data for the node of the tree.
      private:
        // The actual data being stored.
        int data;
        // The height of this node from the deepest
        // point.
        int height;
        // A pointer to the left subtree.
        Node *left_child;
        // A pointer to the node's parent.
        Node *parent;
        // A pointer to the right subtree.
        Node *right_child;

      // - - - - - - - - - - - - - - - - - - - - - -
      // Public methods to process the node.
      public:
        // Constructor initializing the data.
        Node(int val);
        // Calculate the balance point.
        int getBalance();
        // Get the actual data.
        int getData();
        // Get the height.
        int getHeight();
        // Get the left subtree.
        Node *getLeftChild();
        // Get the node's parent.
        Node *getParent();
        // Get the right subtree.
        Node *getRightChild();
        // Remove the node's parent.
        void removeParent();
        // Set the left subtree.
        Node *setLeftChild(Node *newLeft);
        // Set the right subtree.
        Node *setRightChild(Node *newRight);
        // Set the node's height.
        int updateHeight();
    }; // Node

  // - - - - - - - - - - - - - - - - - - - - - - - -
  // Private data for the tree.
  private:
    // A pointer to the top node of the tree.
    Node *root;

  // - - - - - - - - - - - - - - - - - - - - - - - -
  // Private methods to process the tree.
  private:
    // Balance the subtree.
    void balanceAtNode(Node *n);
    // Find the node containing the data.
    Node *findNode(int val);
    // Print the subtree.
    void printSubtree(Node *subtree, int depth,
        int offset, bool first);
    // Rotate the subtree left.
    void rotateLeft(Node *n);
    // Rotate the subtree left.
    void rotateRight(Node *n);
    // Set the root.
    void setRoot(Node *n);
    // Figure out the default spacing for element.
    int spacing(int offset);

  // - - - - - - - - - - - - - - - - - - - - - - - -
  // Public methods to process the tree.
  public:
    // Constructor to create an empty tree.
    AVLTree();
    // Constructor to populate the tree with one node.
    AVLTree(int val);
    // Get the tree's height.
    int getHeight();
    // Insert the value into the tree.
    bool insert(int val);
    // Print the tree.
    void print();
    // Remove the value from the tree.
    bool remove(int val);
}; // AVLTree

// **************************************************
// AVLTree's Node subclass methods.

// --------------------------------------------------
// Constructor initializing the data.
AVLTree::Node::Node(int val) {
  data = val;
  height = 0;
  parent = nullptr;
  left_child = nullptr;
  right_child = nullptr;
} // Node

// --------------------------------------------------
// Calculate the balance point. Negative, when the
// right side is deeper. Zero, when both sides are
// the same. Positive, when the left side is deeper.
int AVLTree::Node::getBalance() {

  // If we don't have a left subtree, check the
  // right.
  int result;
  if (left_child == nullptr)

    // If neither subtree exists, return zero.
    if (right_child == nullptr)
      result = 0;

    // Otherwise, the right subtree exists so make
    // it's height negative and subtract one.
    else
      result = -right_child->height-1;

  // Otherwise, we have a left subtree so if we
  // don't have a right one, return the left's
  // height plus one.
  else if (right_child == nullptr)
    result = left_child->height+1;

  // Otherwise, both subtrees exist so subtract
  // them to return the difference.
  else
    result = left_child->height-right_child->height;
  return result;
} // getBalance

// --------------------------------------------------
// Get the actual data.
int AVLTree::Node::getData() {
  return data;
} // getData

// --------------------------------------------------
// Get the height.
int AVLTree::Node::getHeight() {
  return height;
} // getHeight

// --------------------------------------------------
// Get the left subtree.
AVLTree::Node *AVLTree::Node::getLeftChild() {
  return left_child;
} // getLeftChild

// --------------------------------------------------
// Get the node's parent.
AVLTree::Node *AVLTree::Node::getParent() {
  return parent;
} // getParent

// --------------------------------------------------
// Get the right subtree.
AVLTree::Node *AVLTree::Node::getRightChild() {
  return right_child;
} // getRightChild

// --------------------------------------------------
// Remove the node's parent.
void AVLTree::Node::removeParent() {
  parent = nullptr;
} // removeParent

// --------------------------------------------------
// Set the left subtree.
AVLTree::Node *AVLTree::Node::setLeftChild(
    Node *newLeft) {

  // If there is a left node, set it's parent to
  // be us. In any event, make it our left subtree
  // and update the height.
  if (newLeft != nullptr)
    newLeft->parent = this;
  left_child = newLeft;
  updateHeight();
  return left_child;
} // setLeftChild

// --------------------------------------------------
// Set the right subtree.
AVLTree::Node *AVLTree::Node::setRightChild(
    Node *newRight) {

  // If there is a right node, set it's parent to
  // be us. In any event, make it our right subtree
  // and update the height.
  if (newRight != nullptr)
    newRight->parent = this;
  right_child = newRight;
  updateHeight();
  return right_child;
} // setRightChild

// --------------------------------------------------
// Set the node's height.
int AVLTree::Node::updateHeight() {

  // If we don't have a left subtree, check the
  // right.
  if (left_child == nullptr)

    // If we don't have either subtree, the height
    // is zero.
    if (right_child == nullptr)
      height = 0;

    // Otherwise, we only have the right subtree so
    // our height is one more than it's height.
    else
      height = right_child->height+1;

  // Otherwise, the left subtree exists so if we
  // don't have a right one, our height is one
  // more than it's height.
  else if (right_child == nullptr)
    height = left_child->height+1;

  // Otherwise, we have both subtree's so if the
  // left's height is greater than the right, our
  // height is one more than it.
  else if (left_child->height > right_child->height)
    height = left_child->height+1;

  // Otherwise, the right subtree's height will be
  // used so our height is one more than it.
  else
    height = right_child->height+1;
  return height;
} // updateHeight

// **************************************************
// AVLTree class methods.

// --------------------------------------------------
// Constructor to create an empty tree.
AVLTree::AVLTree() {
  root = nullptr;
} // AVLTree

// --------------------------------------------------
// Constructor to populate the tree with one node.
AVLTree::AVLTree(int val) {
  root = new Node(val);
} // AVLTree

// --------------------------------------------------
// Balance the subtree.
void AVLTree::balanceAtNode(Node *n) {

  // Get the current balance and if it is bad
  // enough on the left side, rotate it right
  // adjusting the subtree left, if required.
  int bal = n->getBalance();
  if (bal > 1) {
    if (n->getLeftChild()->getBalance() < 0)
      rotateLeft(n->getLeftChild());
    rotateRight(n);

  // Otherwise, if the balance is bad enough on
  // the right side, rotate it left adjusting the
  // subtree right, if required.
  } else if (bal < -1) {
    if (n->getRightChild()->getBalance() > 0)
      rotateRight(n->getRightChild());
    rotateLeft(n);
  } // if
} // balanceAtNode

// --------------------------------------------------
// Find the node containing the data.
AVLTree::Node *AVLTree::findNode(int val) {

  // While nodes remain, if we found the right
  // node, exit the loop. If the value we want
  // is less than the current, check the left
  // subtree, otherwise, the right.
  Node *temp = root;
  while (temp != nullptr) {
    if (val == temp->getData())
      break;
    else if (val < temp->getData())
      temp = temp->getLeftChild();
    else
      temp = temp->getRightChild();
  } // while
  return temp;
} // findNode

// --------------------------------------------------
// Get the tree's height.
int AVLTree::getHeight() {
  return root->getHeight();
} // getHeight

// --------------------------------------------------
// Insert the value into the tree.
// Returns:
//         true: If addition is successful
//         false: If item already exists
//
bool AVLTree::insert(int val) {

  // If the tree is empty, add the new node as the
  // root.
  if (root == nullptr)
    root = new Node(val);

  // Otherwise, we need to find the insertion point.
  else {

    // Starting at the tree root search for the
    // insertion point, until we have added the
    // new node.
    Node *added_node = nullptr;
    Node *temp = root;
    while (temp != nullptr && added_node == nullptr) {

      // If the value is less than the current
      // node's value, go left. If there isn't a
      // left subtree, insert the node, otherwise,
      // it is next to check.
      if (val < temp->getData()) {
        if (temp->getLeftChild() == nullptr) {
          added_node = temp->setLeftChild(
              new Node(val));
        } else
          temp = temp->getLeftChild();

      // Otherwise, if the value is greater than
      // the current node's value, go right. If
      // there isn't a right subtree, insert the
      // node, otherwise, it is next to check.
      } else if (val > temp->getData()) {
        if (temp->getRightChild() == nullptr) {
          added_node = temp->setRightChild(
              new Node(val));
        } else
          temp = temp->getRightChild();

      // Otherwise, the value is already in the
      // tree so abort.
      } else
        return false;
    } // while

    // From the new node upwards to the root,
    // updated the height and make sure the
    // subtree is balanced.
    temp = added_node;
    while(temp != nullptr) {
      temp->updateHeight();
      balanceAtNode(temp);
      temp = temp->getParent();
    } // while
  } // if
  return true;
} // insert

// --------------------------------------------------
// Print the tree in this pattern complaining if
// too deep or empty.
//              08
//      04              12
//  02      06      10      14
//01  03  05  07  09  11  13  15
void AVLTree::print() {

  // If the tree is empty, say so.
  if (root == nullptr)
    std::cout << "Tree is empty!" <<
        std::endl;

  // Otherwise, if the tree has a height more than 4
  // (5 rows), it is too wide.
  else if (root->getHeight() > 4)
    std::cout << "Not currently supported!" <<
        std::endl;

  // Otherwise, set up to display the tree. Get
  // the maximum depth and for each possible
  // level, output that level's elements and
  // finish off the line.
  else {
    int max = root->getHeight();
    for (int depth = 0; depth <= max; depth++) {
      printSubtree(root, depth, max-depth+1, true);
      std::cout << std::endl;
    } // for
  } // if
} // print

// --------------------------------------------------
// Print the subtree.  The leftmost branch will have
// first true. The level counts up from the bottom
// for the line we are doing. The depth is how
// many layers to skip over.
void AVLTree::printSubtree(Node *subtree, int depth,
    int level, bool first) {

  // If we need to go deeper in the subtree, do so.
  // If the subtree is empty, pass it down, otherwise
  // pass both left and right subtrees.
  if (depth > 0) {
    if (subtree == nullptr) {
      printSubtree(subtree, depth-1, level, first);
      printSubtree(subtree, depth-1, level, false);
    } else {
      printSubtree(subtree->getLeftChild(), depth-1,
          level, first);
      printSubtree(subtree->getRightChild(), depth-1,
          level, false);
    } // if

  // Otherwise, if the subtree is empty, display
  // an empty element. The leftmost element only
  // needs half the spacing.
  } else if (subtree == nullptr)
    std::cout << std::setw((first)?
        spacing(level)/2:spacing(level)) << "-";

  // Otherwise, we have a live element so display
  // it. Once more, use half spacing for the
  // leftmost element.
  else
    std::cout << std::setw((first)?
        spacing(level)/2:spacing(level)) <<
        subtree->getData();
} // printSubtree

// --------------------------------------------------
// Remove the value from the tree.
// Returns:
//         true: If removal is successful
//         false: If item is not found in the tree
//
bool AVLTree::remove(int val) {

  // Find the node to delete and if none, exit.
  Node *toBeRemoved = findNode(val);
  if (toBeRemoved == nullptr)
    return false;

  // Get the parent and set the side the node is
  // on of the parent.
  enum {left, right} side;
  Node *p = toBeRemoved->getParent();
  if (p != nullptr &&
      p->getLeftChild() == toBeRemoved)
    side = left;
  else
    side = right;

  // If the node to be removed doesn't have a left
  // subtree, check it's right subtree to figure
  // out our next move.
  if (toBeRemoved->getLeftChild() == nullptr)

    // If we don't have any subtrees, we are the
    // leaf so our parent doesn't need us.
    if (toBeRemoved->getRightChild() == nullptr) {

      // If we don't have a parent, the tree is now
      // empty so change the root to null and delete
      // our node.
      if (p == nullptr) {
        setRoot(nullptr);
        delete toBeRemoved;

      // Otherwise, change the parent so it doesn't
      // point to us, delete ourself, update the
      // parent's height, and rebalance the tree.
      } else {
        if (side == left)
          p->setLeftChild(nullptr);
        else
          p->setRightChild(nullptr);
        delete toBeRemoved;
        p->updateHeight();
        balanceAtNode(p);
      } // if

    // Otherwise, there is a right subtree so use
    // it to replace ourself.
    } else {

      // If we don't have a parent, the tree is now
      // the right subtree and delete our node.
      if (p == nullptr) {
        setRoot(toBeRemoved->getRightChild());
        delete toBeRemoved;

      // Otherwise, change the parent so it doesn't
      // point to us, delete ourself, update the
      // parent's height, and rebalance the tree.
      } else {
        if (side == left)
          p->setLeftChild(toBeRemoved->
              getRightChild());
        else
          p->setRightChild(toBeRemoved->
              getRightChild());
        delete toBeRemoved;
        p->updateHeight();
        balanceAtNode(p);
      } // if
    } // if

  // Otherwise, we have a left subtree so check the
  // right one of the node being removed to decide
  // what is next. If there isn't a right subtree,
  // the left one will replace ourself.
  else if (toBeRemoved->getRightChild() ==
      nullptr) {

    // If we don't have a parent, the tree is now
    // the left subtree and delete our node.
    if (p == nullptr) {
      setRoot(toBeRemoved->getLeftChild());
      delete toBeRemoved;

    // Otherwise, change the parent so it doesn't
    // point to us, delete ourself, update the
    // parent's height, and rebalance the tree.
    } else {
      if(side == left)
        p->setLeftChild(toBeRemoved->
            getLeftChild());
      else
        p->setRightChild(toBeRemoved->
            getLeftChild());
      delete toBeRemoved;
      p->updateHeight();
      balanceAtNode(p);
    } // if

  // Otherwise, the node to remove has both subtrees
  // so decide the best method to remove it.
  } else {

    // Check the balance to see which way to go.
    // If the left side is deeper, modify it.
    Node *replacement;
    Node *replacement_parent;
    Node *temp_node;
    int bal = toBeRemoved->getBalance();
    if (bal > 0) {

      // If the right subtree of the node's
      // left subtree is empty, we can move the
      // node's right subtree there.
      if (toBeRemoved->getLeftChild()->
          getRightChild() == nullptr) {
        replacement = toBeRemoved->getLeftChild();
        replacement->setRightChild(
            toBeRemoved->getRightChild());
        temp_node = replacement;

      // Otherwise, find the right most empty subtree
      // of our node's left subtree and it's
      // parent. This is our replacement. Make it's
      // parent point to it's left child instead
      // of itself. It is now free to replace the
      // deleted node. Copy both of the deleted
      // nodes subtrees into the replacement leaving
      // fixing up the parent below.
      } else {
        replacement = toBeRemoved->
            getLeftChild()->getRightChild();
        while (replacement->getRightChild() !=
            nullptr)
          replacement = replacement->getRightChild();
        replacement_parent = replacement->getParent();
        replacement_parent->setRightChild(
            replacement->getLeftChild());
        temp_node = replacement_parent;
        replacement->setLeftChild(
            toBeRemoved->getLeftChild());
        replacement->setRightChild(
            toBeRemoved->getRightChild());
      } // if

    // Otherwise, we are going to modify the right
    // side so, if the left subtree of the node's
    // right subtree is empty, we can move the
    // node's left subtree there.
    } else if (toBeRemoved->getRightChild()->
        getLeftChild() == nullptr) {
      replacement = toBeRemoved->getRightChild();
      replacement->setLeftChild(
          toBeRemoved->getLeftChild());
      temp_node = replacement;

    // Otherwise, find the left most empty subtree
    // of our node's right subtree and it's
    // parent. This is our replacement. Make it's
    // parent point to it's right child instead
    // of itself. It is now free to replace the
    // deleted node. Copy both of the deleted
    // nodes subtrees into the replacement leaving
    // fixing up the parent below.
    } else {
      replacement = toBeRemoved->
          getRightChild()->getLeftChild();
      while (replacement->getLeftChild() !=
          nullptr)
        replacement = replacement->getLeftChild();
      replacement_parent = replacement->getParent();
      replacement_parent->setLeftChild(
          replacement->getRightChild());
      temp_node = replacement_parent;
      replacement->setLeftChild(
          toBeRemoved->getLeftChild());
      replacement->setRightChild(
          toBeRemoved->getRightChild());
    } // if

    // Fix the parent to point to the new root.
    // If there isn't a parent, update the actual
    // tree root. Otherwise, there is a parent so
    // if we were the left subtree, update it,
    // otherwise, the right. In all cases, delete
    // the node and rebalance the tree.
    if (p == nullptr)
      setRoot(replacement);
    else if (side == left)
      p->setLeftChild(replacement);
    else
      p->setRightChild(replacement);
    delete toBeRemoved;
    balanceAtNode(temp_node);
  } // if
  return true;
} // remove

// --------------------------------------------------
// Rotate the subtree left.
void AVLTree::rotateLeft(Node *n) {

  // Get the node's parent and if it exists and the
  // node was it's left subtree, remember we are
  // processing the left, otherwise, the right.
  enum {left, right} side;
  Node *p = n->getParent();
  if (p != nullptr && p->getLeftChild() == n)
    side = left;
  else
    side = right;

  // Get the node's right subtree as the new root
  // and that subtree's left subtree. Make that
  // left subtree the node's new right. Put our
  // original node as the left subtree of our
  // new root.
  Node *temp = n->getRightChild();
  n->setRightChild(temp->getLeftChild());
  temp->setLeftChild(n);

  // Fix the original parent to point to the new
  // root. If there isn't a parent, update the
  // actual tree root. Otherwise, there is a
  // parent so if we were the left subtree, update
  // it, otherwise, the right.
  if (p == nullptr)
    setRoot(temp);
  else if (side == left)
    p->setLeftChild(temp);
  else
    p->setRightChild(temp);
} // rotateLeft

// --------------------------------------------------
// Rotate the subtree left.
void AVLTree::rotateRight(Node *n) {

  // Get the node's parent and if it exists and the
  // node was it's left subtree, remember we are
  // processing the left, otherwise, the right.
  enum {left, right} side;
  Node *p = n->getParent();
  if (p != nullptr && p->getLeftChild() == n)
    side = left;
  else
    side = right;

  // Get the node's left subtree as the new root
  // and that subtree's right subtree. Make that
  // right subtree the node's new left. Put our
  // original node as the right subtree of our
  // new root.
  Node *temp = n->getLeftChild();
  n->setLeftChild(temp->getRightChild());
  temp->setRightChild(n);

  // Fix the original parent to point to the new
  // root. If there isn't a parent, update the
  // actual tree root. Otherwise, there is a
  // parent so if we were the left subtree, update
  // it, otherwise, the right.
  if (p == nullptr)
    setRoot(temp);
  else if (side == left)
    p->setLeftChild(temp);
  else
    p->setRightChild(temp);
} // rotateRight

// --------------------------------------------------
// Set the root. Change the tree root to the node
// and if it exists, remove it's parent.
void AVLTree::setRoot(Node *n) {
  root = n;
  if (root != nullptr)
    root->removeParent();
} // setRoot

// --------------------------------------------------
// Figure out the default spacing for element. Each
// higher level doubles the preceeding. The bottom
// level is one.
int AVLTree::spacing(int level) {
  int result = 2;
  for (int i = 0; i < level; i++)
    result += result;
  return result;
} // spacing

// **************************************************
// Test the class.
int main() {

  // Allocate an array to keep track of the data we
  // add to the tree, initialize the random numbers,
  // allocate an empty tree.
  int data[10];
  srand(time(0));
  AVLTree *tree = new AVLTree();

  // Insert 10 unique random numbers into the tree.
  // For each number we are adding, attempt to insert
  // a random number, until it works because it is
  // unique. Afterwards, display the new number and
  // the current state of the tree.
  for (int i = 0; i < 10; i++) {
    while (!tree->insert(data[i] = rand()%100));
    std::cout << "Adding " << data[i] << std::endl;
    tree->print();
  } // for

  // Remove each of the numbers by displaying the
  // number being removed, performing the removal,
  // and displaying the current state of the tree.
  for (int i = 0; i < 10; i++) {
    std::cout << "Removing " << data[i] << std::endl;
    tree->remove(data[i]);
    tree->print();
  } // for
  return 0;
} // main
