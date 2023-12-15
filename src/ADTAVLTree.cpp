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

#include "cppdescent/ADTAVLTree.hpp"
#include <stdlib.h>

int max(int a, int b) {
  return a > b ? a : b;
}

AVLTree::AVLTree(CompareFunc compare, DestroyFunc destroy, Pointer key)
    : destroy(destroy),
      compare(compare),
      key(key),
      height(1),
      right(nullptr),
      left(nullptr) {}

int AVLTree::getBalance() {
  return this->left->getHeight() - this->right->getHeight();
}

AVLTree::~AVLTree() {
  if (this->getLeft() != nullptr)
    delete this->getLeft();
  if (this->getRight() != nullptr)
    delete this->getRight();
  if (this->destroy != nullptr)
    this->destroy(this->key);
}

AVLTree* AVLTree::rightRotate() {
  AVLTree* x = this->getLeft();
  if (x == nullptr)
    return this;
  AVLTree* T2 = x->getRight();
  int h;

  x->setRight(this);
  this->setLeft(T2);

  if (T2 == nullptr)
    h = 0;
  else
    h = this->getLeft()->getHeight();

  this->setHeight(max(this->getLeft()->getHeight(), h) + 1);
  x->setHeight(max(x->getLeft()->getHeight(), x->getRight()->getHeight()) + 1);

  return x;
}

AVLTree* AVLTree::leftRotate() {
  AVLTree* y = this->getRight();
  if (y == nullptr)
    return this;
  AVLTree* T2 = y->getLeft();

  y->setLeft(this);
  this->setRight(T2);

  this->setHeight(
      max(this->getLeft()->getHeight(), this->getRight()->getHeight()) + 1);
  y->setHeight(max(y->getLeft()->getHeight(), y->getRight()->getHeight()) + 1);

  return y;
}

AVLTree* AVLTree::insert(AVLTree* node, Pointer key) {
  // 1. Simple bst insertion
  if (node == nullptr) {
    AVLTree* newNode = new AVLTree(this->compare, this->destroy, key);
    return newNode;
  }

  if (this->compare(key, node->getKey()) > 0)
    node->setRight(insert(node->getRight(), key));
  else if (this->compare(key, node->getKey()) < 0)
    node->setLeft(insert(node->getLeft(), key));
  else
    return node;

  // 2. Update the height
  node->setHeight(
      max(node->getRight()->getHeight(), node->getLeft()->getHeight()) + 1);

  // 3. Get balance factor
  int balance = node->getBalance();

  // 4. Rotations
  // LL case
  if (balance > 1 && this->compare(key, node->getLeft()->getKey()) < 0)
    return node->rightRotate();
  else if (balance > 1 && this->compare(key, node->getLeft()->getKey()) > 0) {
    node->setLeft(node->getLeft()->leftRotate());
    return node->rightRotate();
  } else if (balance < -1 && this->compare(key, node->getRight()->getKey()) < 0)
    return node->leftRotate();
  else if (balance < -1 && this->compare(key, node->getRight()->getKey()) > 0) {
    node->setRight(node->getRight()->rightRotate());
    return node->leftRotate();
  }

  return node;
}

bool AVLTree::isBalanced() {
  if (this->getRight() == nullptr && this->getLeft() == nullptr)
    return true;

  int lh = this->getLeft()->getHeight();
  int rh = this->getRight()->getHeight();

  if (abs(lh - rh) <= 1 && this->getLeft()->isBalanced() &&
      this->getRight()->isBalanced())
    return true;

  return false;
}