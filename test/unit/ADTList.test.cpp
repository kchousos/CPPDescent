/**
 * @file ADTList.test.cpp
 * @author Konstantinos Chousos
 * @brief Unit tests for the ADT List.
 * @version 0.1
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "cppdescent/ADTList.hpp"
#include "gtest/gtest.h"

// a sample compare function for ints
int compareInts(Pointer a, Pointer b) {
  return *(int*)a - *(int*)b;
}

void deleteInts(Pointer value) {
  delete (int*)value;
};

/**
 * @brief Create an int object in a new memory space with the given value.
 *
 * @param value The value of the created int object.
 * @return int* The created pointer p, where *p = value.
 */
int* createInt(int value) {
  int* p = new int;
  *p = value;
  return p;
}

TEST(ADTListTest, create) {
  // Check that the list has actually been created
  List* list = new List(nullptr);
  ASSERT_NE(list, nullptr);
  ASSERT_EQ(list->getSize(), 0) << list->getSize();
  delete list;
};

TEST(ADTListTest, insert) {
  List* list = new List(nullptr);
  int N = 1000;
  int* array = new int[N * sizeof(*array)];

  for (int i = 0; i < N; i++) {
    list->insertNext(LIST_BOF, &array[i]);

    // Was the size updated correctly?
    ASSERT_EQ(list->getSize(), i + 1);

    // Does the head contain the value that we inserted?
    ASSERT_EQ(list->nodeValue(list->getHead()), &array[i]);
  }

  // Check if the items are in reverse order
  ListNode* node = list->getHead();

  for (int i = N - 1; i >= 0; i--) {
    ASSERT_EQ(list->nodeValue(node), &array[i]);
    node = list->next(node);
  }

  // Inserting NULL after the first node
  ListNode* head_node = list->getHead();
  list->insertNext(head_node, nullptr);
  ASSERT_EQ(list->nodeValue(list->next(head_node)), nullptr);

  delete[] array;
  delete list;
};

TEST(ADTListTest, removeNext) {
  // a list that automatically calls delete upon removing a node
  List* list = new List();
  // test setDestroyValue
  list->setDestroyValue(deleteInts);

  int N = 1000;
  int** array = new int*[N * sizeof(*array)];

  // dynamically allocated elements
  for (int i = 0; i < N; i++) {
    array[i] = new int;
    *array[i] = i;
    list->insertNext(LIST_BOF, array[i]);
  }

  for (int i = N - 1; i >= 0; i--) {
    // remove the head and check to see if it is the same as the input
    ASSERT_EQ(list->nodeValue(list->getHead()), array[i]);
    list->removeNext(LIST_BOF);

    // make sure the size is decreasing
    ASSERT_EQ(list->getSize(), i);
  }

  // now we check removal from an intermediary node
  for (int i = 0; i < N; i++) {
    array[i] = new int;
    *array[i] = i;
    list->insertNext(LIST_BOF, array[i]);
  }

  list->removeNext(list->getHead());
  ASSERT_EQ(list->getSize(), N - 1);

  // delete *array;
  delete[] array;
  delete list;
}

TEST(ADTListTest, find) {
  List* list = new List();
  int N = 1000;
  int* array = new int[N * sizeof(*array)];

  // populate the list
  for (int i = 0; i < N; i++) {
    array[i] = i;
    list->insertNext(LIST_BOF, &array[i]);
  }

  for (int i = 0; i < N; i++) {
    int* value = (int*)list->find(&i, compareInts);
    ASSERT_EQ(value, &array[i]);
  }

  // non-existent value
  int not_exists = -1;
  ASSERT_EQ(list->find(&not_exists, compareInts), nullptr);

  delete[] array;
  delete list;
}

TEST(ADTListTest, findNode) {
  List* list = new List();
  int N = 1000;
  int* array = new int[N * sizeof(*array)];

  for (int i = 0; i < N; i++) {
    array[i] = i;
    list->insertNext(LIST_BOF, &array[i]);
  }

  ListNode* node = list->getHead();

  for (int i = N - 1; i >= 0; i--) {
    ListNode* found_node = list->findNode(&i, compareInts);
    ASSERT_EQ(found_node, node);
    ASSERT_EQ(list->nodeValue(found_node), &array[i]);

    node = list->next(node);
  }

  delete[] array;
  delete list;
}

TEST(ADTListTest, tail) {
  List* list = new List();

  ListNode* empty = list->getTail();
  ASSERT_EQ(empty, nullptr);

  int N = 5;
  int* array = new int[N * sizeof(*array)];

  for (int i = 0; i < N; i++)
    list->insertNext(LIST_BOF, &array[i]);

  ListNode* oldTail = list->getTail();

  int* num = new int;
  *num = 5;
  list->insertNext(oldTail, &num);
  ListNode* newTail = list->getTail();

  // test that the tail changed
  ASSERT_NE(newTail, oldTail);

  list->removeNext(oldTail);
  newTail = list->getTail();
  ASSERT_EQ(newTail, oldTail);

  delete num;
  delete list;
}

TEST(ADTListTest, mergeLists) {
  List* list = new List;

  int N = 1000;
  int** array = new int*[N];

  for (int i = 0; i < N; i++)
    array[i] = createInt(i);

  ListNode* node = LIST_BOF;
  for (int i = 0; i < N; i++) {
    list->insertNext(LIST_BOF, array[i]);
    // node = list->next(node);
    // std::cout << "HERE" << std::endl;
  }

  List* list2 = new List;
  int** array2 = new int*[N];

  for (int i = 0; i < N; i++)
    array2[i] = createInt(i);

  ListNode* node2 = LIST_BOF;
  for (int i = 0; i < N; i++) {
    list2->insertNext(LIST_BOF, array2[i]);
    // node2 = list2->next(node2);
  }

  node = list->getHead();
  node2 = list2->getHead();
  for (int i = 0; i < N; i++) {
    ASSERT_EQ(node->getValue(), array[N - i - 1]);
    ASSERT_EQ(node2->getValue(), array2[N - i - 1]);

    node = list->next(node);
    node2 = list2->next(node2);
  }
  ASSERT_EQ(list->mergeLists(list2), 0);
  ASSERT_EQ(list->getSize(), 2 * N);
  ASSERT_EQ(list->getTail()->getValue(), list2->getTail()->getValue());

  node = list->getHead();
  for (int i = 0; i < 2 * N; i++) {
    if (i < N)
      ASSERT_EQ(node->getValue(), array[N - i - 1]);
    else
      ASSERT_EQ(node->getValue(), array2[2 * N - i - 1]);
    node = list->next(node);
  }

  for (int i = 0; i < N; i++) {
    delete array[i];
    delete array2[i];
  }

  delete[] array;
  delete[] array2;

  delete list;
  delete list2;
}