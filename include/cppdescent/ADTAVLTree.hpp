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
  Pointer key;
  int height;
  AVLTree* left;
  AVLTree* right;
  CompareFunc compare;
  DestroyFunc destroy;
  AVLTree* max;
  AVLTree* min;

 public:
  AVLTree(CompareFunc compare, DestroyFunc destroy, Pointer key);
  ~AVLTree();
  DestroyFunc setDestroyValue(DestroyFunc destroyValue);
  AVLTree* find(Pointer key);
  int getBalance();
  void rightRotate();
  void leftRotate();
  void insert(Pointer key);
  void preorder();
  int getHeight() { return this->height; };
  void setHeight(int height) { this->height = height; };
  AVLTree* getLeft() { return this->left; };
  AVLTree* getRight() { return this->right; };
  void setLeft(AVLTree* left) { this->left = left; };
  void setRight(AVLTree* right) { this->left = right; };
};