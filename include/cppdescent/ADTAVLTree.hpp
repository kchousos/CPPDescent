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

 public:
  AVLTree(Pointer key);
  ~AVLTree();
  int getHeight() { return this->height; };
  int balance();
  AVLTree* rightRotate(AVLTree* y);
  AVLTree* leftRotate(AVLTree* x);
  AVLTree* insert(Pointer key);
  void preorder();
};