/**
 * @file ADTAVLTree.test.cpp
 * @author Konstantinos Chousos
 * @brief
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cppdescent/ADTAVLTree.hpp"
#include "gtest/gtest.h"

// Helper functions
int compareInts(Pointer a, Pointer b) {
  return *(int*)a - *(int*)b;
}

void deleteInts(Pointer value) {
  delete (int*)value;
}

int* createIntValue(int value) {
  int* p = new int;
  *p = value;
  return p;
}

TEST(AVLTree, rightRotate) {
  AVLTree* node = new AVLTree(compareInts, deleteInts, createIntValue(5));

  node = node->rightRotate();

  ASSERT_EQ(*(int*)node->getKey(), 5);

  delete node;

  // no right child
  node = new AVLTree(compareInts, deleteInts, createIntValue(5));
  node->setHeight(2);
  //   AVLTree* child = new AVLTree(compareInts, deleteInts, createIntValue(3))
  node->setLeft(new AVLTree(compareInts, deleteInts, createIntValue(3)));
  node = node->rightRotate();

  ASSERT_EQ(*(int*)node->getKey(), 3);
  ASSERT_EQ(*(int*)node->getRight()->getKey(), 5);

  ASSERT_EQ(node->getHeight(), 2);
  ASSERT_EQ(node->getRight()->getHeight(), 1);

  delete node;
}