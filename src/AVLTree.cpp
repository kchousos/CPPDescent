/**
 * @file ADTAVLTree.cpp
 * @author Konstantinos Chousos
 * @brief
 * @version 0.1
 * @date 2023-12-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cppdescent/AVLTree.hpp"
#include <stdlib.h>
#include <iostream>

int max(int a, int b) {
  return a > b ? a : b;
}

AVLTree::AVLTree(CompareFunc compare,
                 DestroyFunc destroy,
                 DestroyFunc destroy_key,
                 Pointer key)
    : compare(compare), destroy(destroy), destroy_key(destroy_key) {
  root = new AVLNode(key);
  this->size = 0;
}

void AVLTree::insert(Pointer key) {
  bool inserted;
  Pointer oldKey;
  this->root = this->root->insert(key, compare, &inserted, &oldKey);

  // If a new node is inserted change the AVL's size. Destroy the old key of the
  // node if it is updated
  if (inserted == true)
    this->size++;
  else if (this->destroy_key != nullptr)
    this->destroy_key(oldKey);
}

bool AVLTree::remove(Pointer key) {
  bool removed;
  Pointer oldKey = nullptr;
  this->root = this->root->remove(key, this->compare, &removed, &oldKey);

  // Το size αλλάζει μόνο αν πραγματικά αφαιρεθεί ένας κόμβος
  if (removed == true) {
    this->size--;

    if (this->destroy_key != nullptr)
      this->destroy_key(oldKey);
  }

  return removed;
}

AVLTree::~AVLTree() {}

/////////    AVL Node    /////////

AVLNode::AVLNode(Pointer key, int height, AVLNode* left, AVLNode* right)
    : key(key), height(height), left(left), right(right) {}

AVLNode* AVLNode::rightRotate() {
  AVLNode* x = this->left;
  if (x == nullptr)
    return this;

  AVLNode* rightNode = x->right;
  int h;

  x->setRight(this);
  this->setLeft(rightNode);

  if (rightNode == nullptr)
    h = 0;
  else
    h = this->left->height;

  this->setHeight(max(this->left->height, h) + 1);
  x->setHeight(max(x->left->height, x->right->height) + 1);

  return x;
}

AVLNode* AVLNode::leftRotate() {
  AVLNode* y = this->right;
  if (y == nullptr)
    return this;

  AVLNode* leftNode = y->left;

  y->setLeft(this);
  this->setRight(leftNode);

  this->setHeight(max(this->left->height, this->right->height) + 1);
  y->setHeight(max(y->left->height, y->right->height) + 1);

  return y;
}

int AVLNode::getBalance() {
  return this->left->height - this->right->height;
}

bool AVLNode::isBalanced() {
  if (this->right == nullptr && this->left == nullptr)
    return true;

  int lh = this->left->height;
  int rh = this->right->height;

  if (abs(lh - rh) <= 1 && this->left->isBalanced() &&
      this->right->isBalanced())
    return true;

  return false;
}

void AVLNode::updateHeight() {
  this->height = 1 + max(this->left->height, this->right->height);
}

AVLNode* AVLNode::repairBalance() {
  this->updateHeight();

  int balance = this->getBalance();
  if (balance > 1) {
    // Left subtree is unbalanced
    if (this->left->getBalance() >= 0)
      return this->rightRotate();
    else
      return this->leftRotate();

  } else if (balance < -1) {
    // Right subtree is unbalanced
    if (this->right->getBalance() <= 0)
      return this->leftRotate();
    else
      return this->rightRotate();
  }

  // No rotation needed
  return this;
}

AVLNode* AVLNode::insert(Pointer key,
                         CompareFunc compare,
                         bool* inserted,
                         Pointer* oldKey) {
  if (this->key == nullptr) {
    *inserted = true;
    return new AVLNode(key);
  }
  // To find the right place for the insertion we have to compare the current
  // node's key with the given one
  int compareResult = compare(key, this->key);
  if (compareResult == 0) {
    // std::cout << "HERE - if" << std::endl;
    // An equivalent key found, so we update it
    *inserted = false;
    *oldKey = this->key;
    this->key = key;

  } else if (compareResult < 0) {
    // std::cout << "HERE - else if" << std::endl;
    // value < node->value, keep searching in the left subtree
    this->left = this->insert(key, compare, inserted, oldKey);

  } else {
    // std::cout << "HERE - else" << std::endl;
    // value > node->value, keep searching in the left subtree
    AVLNode* right = this->right;

    this->right = this->insert(key, compare, inserted, oldKey);
  }

  return this->repairBalance();
}

AVLNode* AVLNode::removeMin(AVLNode** minNode) {
  if (this->left == nullptr) {
    // There is no left subtree, so the min node is the current (this)
    *minNode = this;
    return this->right;  // The new root is the right child

  } else {
    // There is left subtree, so the min node is in it. Continue recursively and
    // update node->left with the subtree's root
    this->left = this->left->removeMin(minNode);

    return this->repairBalance();
  }
}

AVLNode* AVLNode::remove(Pointer key,
                         CompareFunc compare,
                         bool* removed,
                         Pointer* oldKey) {
  int compareResult = compare(key, this->key);
  if (compareResult == 0) {
    // Equivalent value found, so we delete it.
    *removed = true;
    *oldKey = this->key;

    if (this->left == nullptr) {
      // There is no left subtree,so we delete the current node and the new root
      // is the right child
      AVLNode* right = this->right;
      // delete this;
      return right;

    } else if (this->right == nullptr) {
      // There is no right subtree, so we delete the current node and the new
      // root is the left child
      AVLNode* left = this->left;
      // delete this;
      return left;

    } else {
      // Both children exist. Replace the current node's value with the min
      // value of the right subtree and remove it.

      AVLNode* minRight;
      this->right = removeMin(&minRight);

      minRight->left = this->left;
      minRight->right = this->right;

      // delete this;

      return minRight->repairBalance();
    }
  }

  // compareResultes != 0, continue in the left or right subtree without
  // changing the root
  if (compareResult < 0)
    this->left = this->left->remove(key, compare, removed, oldKey);
  else
    this->right = this->right->remove(key, compare, removed, oldKey);

  return this->repairBalance();
}