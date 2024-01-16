// /**
//  * @file ADTAVLTree.cpp
//  * @author Konstantinos Chousos
//  * @brief
//  * @version 0.1
//  * @date 2023-12-14
//  *
//  * @copyright Copyright (c) 2023
//  *
//  */

#include "cppdescent/AVLTree.hpp"
#include <stdlib.h>
#include <iostream>

// int max(int a, int b) {
//   return a > b ? a : b;
// }

// AVLTree::AVLTree(CompareFunc compare,
//                  DestroyFunc destroy,
//                  DestroyFunc destroy_key,
//                  Pointer key)
//     : compare(compare), destroy(destroy), destroy_key(destroy_key) {
//   root = new AVLNode(key);
//   this->size = 0;
// }

// void AVLTree::insert(Pointer key) {
//   bool inserted = false;
//   Pointer oldKey;

//   this->root = this->root->insert(key, compare, &inserted, &oldKey);

//   // If a new node is inserted change the AVL's size. Destroy the old key of
//   the
//   // node if it is updated
//   if (inserted == true)
//     this->size++;
//   else if (this->destroy_key != nullptr && oldKey != nullptr)
//     this->destroy_key(oldKey);
//   return;
// }

// bool AVLTree::remove(Pointer key) {
//   bool removed;
//   Pointer oldKey = nullptr;
//   this->root = this->root->remove(key, this->compare, &removed, &oldKey);

//   // Το size αλλάζει μόνο αν πραγματικά αφαιρεθεί ένας κόμβος
//   if (removed == true) {
//     this->size--;

//     if (this->destroy_key != nullptr)
//       this->destroy_key(oldKey);
//   }

//   return removed;
// }

// Pointer AVLTree::find(Pointer key) {
//   AVLNode* node = this->root->find(key, this->compare);
//   return node == nullptr ? nullptr : node->getKey();
// }

// AVLTree::~AVLTree() {}

// /////////    AVL Node    /////////

// AVLNode::AVLNode(Pointer key, int height, AVLNode* left, AVLNode* right)
//     : key(key), height(height), left(left), right(right) {}

// AVLNode* rightRotate(AVLNode* root) {
//   AVLNode* x = root->getLeft();
//   // if (x == nullptr)
//   //   return root;

//   AVLNode* rightNode = x->getRight();

//   x->setRight(root);
//   root->setLeft(rightNode);

//   root->updateHeight();
//   x->updateHeight();

//   return x;
// }

// AVLNode* leftRotate(AVLNode* root) {
//   AVLNode* y = root->getRight();
//   // if (y == nullptr)
//   //   return root;

//   AVLNode* leftNode = y->getLeft();

//   y->setLeft(root);
//   root->setRight(leftNode);

//   root->updateHeight();
//   y->updateHeight();

//   return y;
// }

// AVLNode* leftRightRotate(AVLNode* root) {
//   root->setLeft(leftRotate(root->getLeft()));
//   return rightRotate(root);
// }

// AVLNode* rightLeftRotate(AVLNode* root) {
//   root->setRight(rightRotate(root->getRight()));
//   return leftRotate(root);
// }

// int AVLNode::getBalance() {
//   int balance = 0;
//   if (this->left == nullptr)
//     if (this->right == nullptr)
//       balance = 0;
//     else
//       balance = -this->right->height;
//   else if (this->right == nullptr)
//     balance = this->left->height;
//   else
//     balance = this->left->height - this->right->height;

//   return balance;
// }

// bool AVLNode::isBalanced() {
//   if (this->right == nullptr && this->left == nullptr)
//     return true;

//   int lh = this->left->height;
//   int rh = this->right->height;

//   if (abs(lh - rh) <= 1 && this->left->isBalanced() &&
//       this->right->isBalanced())
//     return true;

//   return false;
// }

// void AVLNode::updateHeight() {
//   int newHeight = 0;
//   if (this->left == nullptr)
//     if (this->right == nullptr)
//       newHeight = 1;
//     else
//       newHeight = this->right->height;
//   else if (this->right == nullptr)
//     newHeight = this->left->height;
//   else
//     newHeight = max(this->left->height, this->right->height);

//   this->height = 1 + newHeight;
// }

// AVLNode* AVLNode::repairBalance() {
//   this->updateHeight();

//   int balance = this->getBalance();
//   if (balance > 1) {
//     // Left subtree is unbalanced
//     if (this->left->getBalance() >= 0)
//       return rightRotate(this);
//     else
//       return leftRightRotate(this);

//   } else if (balance < -1) {
//     // Right subtree is unbalanced
//     if (this->right->getBalance() <= 0)
//       return leftRotate(this);
//     else
//       return rightLeftRotate(this);
//   }

//   // No rotation needed
//   return this;
// }

// AVLNode* AVLNode::insert(Pointer key,
//                          CompareFunc compare,
//                          bool* inserted,
//                          Pointer* oldKey) {
//   if (this->key == nullptr) {
//     *inserted = true;
//     this->key = key;
//     return this;
//   }
//   // To find the right place for the insertion we have to compare the current
//   // node's key with the given one
//   int compareResult = compare(key, this->key);
//   if (compareResult == 0) {
//     // An equivalent key found, so we update it
//     *inserted = false;
//     *oldKey = this->key;
//     this->key = key;

//   } else if (compareResult < 0) {
//     // value < node->value, keep searching in the left subtree
//     if (this->left != nullptr) {
//       this->left->insert(key, compare, inserted, oldKey);
//     } else {
//       *inserted = true;
//       this->left = new AVLNode(key);
//       // this->left->insert(key, compare, inserted, oldKey);
//     }

//   } else {
//     // value > node->value, keep searching in the left subtree
//     if (this->right != nullptr) {
//       this->right->insert(key, compare, inserted, oldKey);
//     } else {
//       *inserted = true;
//       this->right = new AVLNode(key);
//       // this->right->insert(key, compare, inserted, oldKey);
//     }
//   }

//   return this->repairBalance();
// }

// AVLNode* AVLNode::find(Pointer key, CompareFunc compare) {
//   if (this->key == nullptr)
//     return nullptr;

//   // Το πού βρίσκεται ο κόμβος που ψάχνουμε εξαρτάται από τη διάταξη της
//   τιμής
//   // value σε σχέση με την τιμή του τρέχοντος κόμβο (node->value)
//   //
//   int compare_res = compare(
//       key,
//       this->key);  // αποθήκευση για να μην καλέσουμε την compare 2 φορές
//   if (compare_res == 0)  // value ισοδύναμη της node->value, βρήκαμε τον
//   κόμβο
//     return this;
//   else if (compare_res < 0)  // value < node->value, ο κόμβος που ψάχνουμε
//   είναι
//                              // στο αριστερό υποδέντρο
//     return this->left != nullptr ? this->left->find(key, compare) : nullptr;
//   else  // value > node->value, ο κόμβος που ψάχνουμε είνια στο δεξιό
//   υποδέντρο
//     return this->right != nullptr ? this->right->find(key, compare) :
//     nullptr;
// }

// AVLNode* AVLNode::removeMin(AVLNode** minNode) {
//   if (this->left == nullptr) {
//     // There is no left subtree, so the min node is the current (this)
//     *minNode = this;
//     return this->right;  // The new root is the right child

//   } else {
//     // There is left subtree, so the min node is in it. Continue recursively
//     and
//     // update node->left with the subtree's root
//     this->left = this->left->removeMin(minNode);

//     return this->repairBalance();
//   }
// }

// AVLNode* AVLNode::remove(Pointer key,
//                          CompareFunc compare,
//                          bool* removed,
//                          Pointer* oldKey) {
//   if (this->key == nullptr) {
//     *removed = false;
//     return this;
//   }

//   int compareResult = compare(key, this->key);
//   if (compareResult == 0) {
//     // Equivalent value found, so we delete it.
//     *removed = true;
//     *oldKey = this->key;

//     if (this->left->key == nullptr) {
//       // There is no left subtree,so we delete the current node and the new
//       root
//       // is the right child
//       AVLNode* right = this->right;
//       delete this;
//       return right;

//     } else if (this->right->key == nullptr) {
//       // There is no right subtree, so we delete the current node and the new
//       // root is the left child
//       AVLNode* left = this->left;
//       delete this;
//       return left;

//     } else {
//       // Both children exist. Replace the current node's value with the min
//       // value of the right subtree and remove it.

//       AVLNode* minRight;
//       this->right->removeMin(&minRight);

//       minRight->left = this->left;
//       minRight->right = this->right;

//       delete this;

//       return minRight->repairBalance();
//     }
//   }

//   // compareResultes != 0, continue in the left or right subtree without
//   // changing the root
//   if (compareResult < 0) {
//     if (this->left != nullptr) {
//       this->left = this->left->remove(key, compare, removed, oldKey);
//     } else {
//       *removed = false;
//     }

//   } else if (compareResult > 0) {
//     if (this->right != nullptr) {
//       this->right = this->right->remove(key, compare, removed, oldKey);
//     } else {
//       *removed = false;
//     }
//   }

//   return this->repairBalance();
// }

int max(int a, int b) {
  return (a > b) ? a : b;
}

AVLNode::AVLNode(Pointer key) {
  this->key = key;
  this->left = nullptr;
  this->right = nullptr;
  this->height = 1;
}

int AVLNode::getHeight() {
  return this->height;
}

void AVLNode::updateHeight() {
  int rHeight = this->right == nullptr ? 0 : this->right->getHeight();
  int lHeight = this->left == nullptr ? 0 : this->left->getHeight();

  this->height = max(lHeight, rHeight) + 1;
}

int AVLNode::getBalance() {
  int balance;
  if (this->left == nullptr)
    if (this->right == nullptr)
      balance = 0;
    else
      balance = this->right->getHeight() * (-1);
  else if (this->right == nullptr)
    balance = this->left->getBalance();
  else
    balance = this->left->getBalance() - this->right->getBalance();

  return balance;
}

AVLNode* AVLNode::rightRotate() {
  AVLNode* x = this->left;
  if (x == nullptr)
    return this;
  AVLNode* T2 = x->right;

  x->right = this;
  this->left = T2;

  // update heights
  this->updateHeight();
  this->updateHeight();
  x->updateHeight();

  return x;
}

AVLNode* AVLNode::leftRotate() {
  AVLNode* y = this->right;
  if (y == nullptr)
    return this;
  AVLNode* T2 = y->left;

  y->left = this;
  this->right = T2;

  // update heights
  this->updateHeight();
  y->updateHeight();

  return y;
}

AVLNode* AVLNode::insert(Pointer key, CompareFunc compare, bool* inserted) {
  // 1. Perform a simple bst tree insertion
  int compareResult = compare(key, this->key);
  if (compareResult > 0) {
    if (this->right == nullptr)
      *inserted = true;
    this->right = this->right != nullptr
                      ? this->right->insert(key, compare, inserted)
                      : new AVLNode(key);
  } else if (compareResult < 0) {
    if (this->left == nullptr)
      *inserted = true;
    this->left = this->left != nullptr
                     ? this->left->insert(key, compare, inserted)
                     : new AVLNode(key);
  } else {
    return this;
  }

  // 2. update the height of the node
  this->updateHeight();

  // 3. get the balance factor of the current node
  int balance = this->getBalance();

  // 4. perform the rotations

  // Left-Left case
  if (balance > 1 && compare(key, this->left->key) < 0) {
    return this->rightRotate();
  }
  // Left-Right case
  else if (balance > 1 && compare(key, this->left->key) > 0) {
    this->left = this->left->leftRotate();
    return this->rightRotate();
  }
  // Right-Right case
  else if (balance < -1 && compare(key, this->right->key) > 0) {
    return this->leftRotate();
  } else if (balance < -1 && compare(key, this->right->key) < 0) {
    this->right = this->right->rightRotate();
    return this->leftRotate();
  }

  return this;
}

///////   AVLTree   ///////

AVLTree::AVLTree(CompareFunc compare, Pointer key) {
  this->root = new AVLNode(key);
  this->size = 1;
  this->compare = compare;
}

void AVLTree::insert(Pointer key) {
  bool* inserted = new bool;
  *inserted = false;

  this->root = this->root->insert(key, this->compare, inserted);

  if (*inserted == true)
    this->size++;

  delete inserted;
}