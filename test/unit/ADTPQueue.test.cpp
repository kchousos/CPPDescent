/**
 * @file ADTPQueue.test.cpp
 * @author Konstantinos Chousos
 * @brief Tests for the ADT Priority Queue data structure.
 * @version 0.1
 * @date 2023-10-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "cppdescent/ADTPQueue.hpp"
#include "gtest/gtest.h"

// a sample compare function for ints
int compareInts(Pointer a, Pointer b) {
  return *(int*)a - *(int*)b;
}

void deleteInts(Pointer value) {
  delete (int*)value;
};

void shuffle(int* array[], int size) {
  for (int i = 0; i < size; i++) {
    int newPos = i + rand() / (RAND_MAX / (size - i) + 1);
    int* temp = array[newPos];
    array[newPos] = array[i];
    array[i] = temp;
  }
}

int* createInt(int value) {
  int* p = new int;
  *p = value;
  return p;
}

TEST(ADTPQueueTest, create) {
  PQueue* queueEmpty = new PQueue(compareInts, nullptr, nullptr);
  queueEmpty->setDestroyValue(nullptr);

  ASSERT_NE(queueEmpty, nullptr);
  ASSERT_EQ(queueEmpty->getSize(), 0);

  delete queueEmpty;

  // No destroy function because the values will be destroyed by the queue.
  Vector* values = new Vector(0, nullptr);
  values->insertLast(createInt(0));
  values->insertLast(createInt(1));
  values->insertLast(createInt(2));
  values->insertLast(createInt(3));

  PQueue* queue = new PQueue(compareInts, deleteInts, values);

  ASSERT_NE(queue, nullptr);
  ASSERT_EQ(queue->getSize(), 4);

  ASSERT_EQ(*(int*)queue->getMax(), 3);
  queue->removeMax();
  ASSERT_EQ(*(int*)queue->getMax(), 2);

  delete values;
  delete queue;
}

TEST(ADTPQueueTest, insert) {
  PQueue* queue = new PQueue(compareInts, nullptr, nullptr);
  int N = 1000;
  int* array = new int[N * sizeof(*array)];

  for (int i = 0; i < N; i++) {
    array[i] = i;
    queue->insert(&array[i]);
    ASSERT_EQ(queue->getSize(), i + 1);
    ASSERT_EQ(queue->getMax(), &array[i]);
  }

  delete queue;
  delete[] array;
}

TEST(ADTPQueueTest, remove) {
  PQueue* queue = new PQueue(compareInts, deleteInts, nullptr);

  int N = 10;
  int** array = new int*[N * sizeof(*array)];
  for (int i = 0; i < N; i++)
    array[i] = createInt(i);

  shuffle(array, N);

  for (int i = 0; i < N; i++)
    queue->insert(array[i]);

  for (int i = N - 1; i >= 0; i--) {
    int* value = (int*)queue->getMax();
    ASSERT_EQ(*value, i);
    ASSERT_EQ(queue->getMax(), value);
    queue->removeMax();
    ASSERT_EQ(queue->getSize(), i);
  }

  delete queue;

  PQueue* queue2 = new PQueue(compareInts, nullptr, nullptr);
  queue2->insert(&N);
  ASSERT_EQ(queue2->getMax(), &N);
  // queue2->removeMax();
  delete queue2;
  delete[] array;
}