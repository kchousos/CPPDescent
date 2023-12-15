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

#define max(a, b)           \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b;      \
  })

AVLTree::AVLTree(CompareFunc compare, DestroyFunc destroy, Pointer key)
    : compare(compare),
      destroy(destroy),
      key(key),
      height(1),
      right(nullptr),
      left(nullptr) {}

int AVLTree::getBalance() {
  return this->left->getHeight() - this->right->getHeight();
}

AVLTree* AVLTree::rightRotate(AVLTree* y) {
  AVLTree* x = y->getLeft();
  if (x == nullptr)
    return y;
  AVLTree* T2 = x->getRight();

  x->setRight(y);
  y->setLeft(T2);

  y->setHeight(max(y->getLeft()->getHeight(), y->getRight()->getHeight()) + 1);
  x->setHeight(max(x->getLeft()->getHeight(), x->getRight()->getHeight()) + 1);

  return x;
}

AVLTree* AVLTree::leftRotate(AVLTree* x) {
  AVLTree* y = x->getRight();
  if (y == nullptr)
    return x;
  AVLTree* T2 = y->getLeft();

  y->setLeft(x);
  x->setRight(T2);

  x->setHeight(max(x->getLeft()->getHeight(), x->getRight()->getHeight()) + 1);
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
    return this->rightRotate(node);
}