/**
 * @file ADTAVLTree.hpp
 * @author Konstantinos Chousos
 * @brief An implementation of an Abstract Data Type AVL tree.
 * @version 0.1
 * @date 2023-12-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "common.hpp"

class AVLTree {
 private:
  DestroyFunc destroy;
  CompareFunc compare;
  Pointer key;
  int height;
  AVLTree* right;
  AVLTree* left;
  // AVLTree* max;
  // AVLTree* min;

 public:
  AVLTree(CompareFunc compare, DestroyFunc destroy, Pointer key);
  ~AVLTree();
  DestroyFunc setDestroyValue(DestroyFunc destroyValue);
  AVLTree* find(Pointer key);
  int getBalance();
  AVLTree* rightRotate();
  AVLTree* leftRotate();
  bool isBalanced();
  AVLTree* insert(AVLTree* node, Pointer key);
  Pointer getKey() { return this->key; };
  int getHeight() { return this->height; };
  void setHeight(int height) { this->height = height; };
  AVLTree* getLeft() { return this->left; };
  AVLTree* getRight() { return this->right; };
  void setLeft(AVLTree* left) { this->left = left; };
  void setRight(AVLTree* right) { this->left = right; };
};