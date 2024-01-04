/**
 * @file ADTAVLTree.hpp
 * @author Konstantinos Chousos
 * @brief An implementation of AVL tree data structure.
 * @version 0.1
 * @date 2023-12-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "common.hpp"

class AVLNode {
 private:
  Pointer key;
  int height;
  AVLNode* left;
  AVLNode* right;

 public:
  AVLNode(Pointer key,
          int height = 1,
          AVLNode* left = nullptr,
          AVLNode* right = nullptr);
  AVLNode* rightRotate();
  AVLNode* leftRotate();
  AVLNode* repairBalance();
  AVLNode* insert(Pointer key,
                  CompareFunc compare,
                  bool* inserted,
                  Pointer* oldValue);
  AVLNode* removeMin(AVLNode** minNode);
  AVLNode* remove(Pointer key,
                  CompareFunc compare,
                  bool* removed,
                  Pointer* oldValue);
  Pointer getKey() { return this->key; };
  int getHeight() { return this->height; };
  void setHeight(int height) { this->height = height; };
  AVLNode* getLeft() { return this->left; };
  AVLNode* getRight() { return this->right; };
  void setLeft(AVLNode* left) { this->left = left; };
  void setRight(AVLNode* right) { this->left = right; };
  void updateHeight();
  int getBalance();
  bool isBalanced();
};

class AVLTree {
 private:
  AVLNode* root;
  int size;
  AVLNode* max;
  AVLNode* min;
  CompareFunc compare;
  DestroyFunc destroy;
  DestroyFunc destroy_key;

 public:
  AVLTree(CompareFunc compare,
          DestroyFunc destroy,
          DestroyFunc destroy_key,
          Pointer key);
  ~AVLTree();
  void insert(Pointer key);
  bool remove(Pointer key);
  int getSize() { return this->size; };
  void setDestroyKey(DestroyFunc destroyKey) {
    this->destroy_key = destroyKey;
  };
  AVLNode* getMax() { return this->max; };
  AVLNode* getMin() { return this->min; };
};