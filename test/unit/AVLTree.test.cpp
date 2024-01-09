/**
 * @file AVLTree.test.cpp
 * @author Konstantinos Chousos
 * @brief
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cppdescent/AVLTree.hpp"
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

void shuffle(int** array, int n) {
  for (int i = 0; i < n; i++) {
    int j = i + rand() / (RAND_MAX / (n - i) + 1);
    int* t = array[j];
    array[j] = array[i];
    array[i] = t;
  }
}

TEST(AVLTree, insert) {
  AVLTree* avl = new AVLTree(compareInts, deleteInts, deleteInts, nullptr);

  int N = 1000;

  int** valueArray = new int*[N];

  // Δοκιμάζουμε την insert με νέες τιμές κάθε φορά και με αυτόματο free
  for (int i = 0; i < N; i++) {
    valueArray[i] = createIntValue(i);

    avl->insert(valueArray[i]);

    ASSERT_EQ(avl->getSize(), (i + 1));
  }

  // Δοκιμάζουμε την insert με τιμές που υπάρχουν ήδη στο Set
  // και ελέγχουμε ότι δεν ενημερώθηκε το size (καθώς δεν προστέθηκε νέος
  // κόμβος)
  int* newValue = createIntValue(0);

  avl->insert(newValue);

  // std::cout << avl->getMax()->getKey() << std::endl;

  ASSERT_EQ(avl->getSize(), N);

  delete avl;

  // Δοκιμάζουμε την insert χωρίς αυτόματο free
  AVLTree* avl2 = new AVLTree(compareInts, nullptr, nullptr, nullptr);

  int local_value1 = 0, local_value2 = 1, local_value3 = 1;

  avl2->insert(&local_value1);
  avl2->insert(&local_value2);
  avl2->insert(&local_value3);

  delete avl2;
  delete[] valueArray;
}

TEST(AVLTree, remove) {
  AVLTree* avl = new AVLTree(compareInts, deleteInts, deleteInts, nullptr);

  int N = 1000;

  int** valueArray = new int*[N];

  // Δοκιμάζουμε την insert με νέες τιμές κάθε φορά και με αυτόματο free
  for (int i = 0; i < N; i++)
    valueArray[i] = createIntValue(i);

  shuffle(valueArray, N);

  for (int i = 0; i < N; i++)
    avl->insert(valueArray[i]);

  int not_exists = 2000;
  ASSERT_FALSE(avl->remove(&not_exists));

  for (int i = 0; i < N; i++) {
    avl->remove(valueArray[i]);
    ASSERT_EQ(avl->getSize(), N - i + 1);
  }

  delete avl;

  AVLTree* avl2 = new AVLTree(compareInts, nullptr, nullptr, nullptr);

  int local_value1 = 0;

  avl2->insert(&local_value1);
  ASSERT_TRUE(avl->remove(&local_value1));
  ASSERT_EQ(avl->getSize(), 0);

  delete avl2;
  delete[] valueArray;
}