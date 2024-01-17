/**
 * @file ADTVector.test.cpp
 * @author Konstantinos Chousos
 * @brief Tests for the ADTVector class.
 * @version 0.1
 * @date 2023-10-29
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "cppdescent/ADTVector.hpp"
#include "gtest/gtest.h"

int compareInts(Pointer a, Pointer b) {
  return *(int*)a - *(int*)b;
}

void deleteInts(Pointer value) {
  delete (int*)value;
};

TEST(ADTVectorTest, create) {
  Vector* v = new Vector(0, nullptr);
  Vector* v2 = new Vector(10, nullptr);

  v->setDestroyValue(nullptr);
  v2->setDestroyValue(nullptr);

  ASSERT_EQ(v->getSize(), 0);
  ASSERT_EQ(v2->getSize(), 10);

  delete v;
  delete v2;
}

TEST(ADTVectorTest, insertLast) {
  Vector* v = new Vector(0, nullptr);
  int N = 1000;
  int* array = new int[N * sizeof(*array)];

  for (int i = 0; i < N; i++) {
    v->insertLast(&array[i]);
    ASSERT_EQ(v->getSize(), i + 1);
    ASSERT_EQ(v->getAt(i), &array[i]);
  }

  for (int i = 0; i < N; i++)
    ASSERT_EQ(v->getAt(i), &array[i]);

  delete[] array;
  delete v;
}

TEST(ADTVectorTest, removeLast) {
  Vector* vEmpty = new Vector(0, nullptr);
  ASSERT_EQ(vEmpty->removeLast(), -1);
  delete vEmpty;

  Vector* v = new Vector(1000, nullptr);
  int N = 1000;
  int* array = new int[N * sizeof(*array)];

  for (int i = 0; i < N; i++)
    v->setAt(i, &array[i]);

  for (int i = N - 1; i >= 0; i--) {
    ASSERT_EQ(v->getAt(i), &array[i]);
    v->removeLast();
    ASSERT_EQ(v->getSize(), i);
  }

  delete[] array;
  delete v;
}

TEST(ADTVectorTest, setAtgetAt) {
  // edge cases
  Vector* vEmpty = new Vector(0, nullptr);
  ASSERT_EQ(vEmpty->getAt(0), nullptr);
  int num = 1;
  ASSERT_EQ(vEmpty->setAt(0, &num), -1);
  delete vEmpty;

  int N = 1000;
  Vector* v = new Vector(N / 2, nullptr);
  ASSERT_EQ(v->getSize(), N / 2);

  int* array = new int[N * sizeof(*array)];

  // Add another N/2 null elements
  for (int i = 0; i < N / 2; i++)
    v->insertLast(nullptr);

  for (int i = 0; i < N; i++) {
    ASSERT_EQ(v->getAt(i), nullptr);
    v->setAt(i, &array[i]);
    ASSERT_EQ(v->getAt(i), &array[i]);
  }

  delete[] array;
  delete v;
}

TEST(ADTVectorTest, iteration) {
  Vector* v = new Vector(0, nullptr);
  int N = 1000;
  int* array = new int[N * sizeof(*array)];

  ASSERT_EQ(v->first(), VECTOR_BOF);
  ASSERT_EQ(v->last(), VECTOR_EOF);

  for (int i = 0; i < N; i++)
    v->insertLast(&array[i]);

  int i = 0;
  for (vectorNode* node = v->first(); node != VECTOR_EOF; node = v->next(node))
    ASSERT_EQ(v->nodeValue(node), &array[i++]);
  ASSERT_EQ(i, N);

  for (vectorNode* node = v->last(); node != VECTOR_BOF;
       node = v->previous(node))
    ASSERT_EQ(v->nodeValue(node), &array[--i]);
  ASSERT_EQ(i, 0);

  delete[] array;
  delete v;
}

TEST(ADTVectorTest, find) {
  Vector* v = new Vector(1000, nullptr);
  int N = 1000;
  int* array = new int[N * sizeof(*array)];

  for (int i = 0; i < N; i++) {
    array[i] = i;
    v->setAt(i, &array[i]);
  }

  for (int i = 0; i < N; i++) {
    int* found = (int*)v->find(&i, compareInts);
    ASSERT_EQ(*found, i);

    vectorNode* node = v->findNode(&i, compareInts);
    ASSERT_EQ(*(int*)node->getValue(), i);
  }

  int notExists = -1;
  ASSERT_EQ(v->find(&notExists, compareInts), nullptr);
  ASSERT_EQ(v->findNode(&notExists, compareInts), VECTOR_EOF);
  ASSERT_EQ(v->findPos(&notExists, compareInts), -1);

  ASSERT_NE(v->binaryFind(&array[600], compareInts), nullptr);

  delete[] array;
  delete v;
}

// To test this we need to run valgrind for any memory leaks.
TEST(ADTVectorTest, destroy) {
  Vector* v = new Vector(1, deleteInts);
  v->setAt(0, new int(1));
  v->insertLast(new int(2));
  v->removeLast();
  delete v;
}