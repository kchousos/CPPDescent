/**
 * @file linkedList.test.cpp
 * @author Konstantinos Chousos
 * @brief Unit tests for the ADT List.
 * @version 0.1
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "cppdescent/linkedList.hpp"
#include "gtest/gtest.h"

// a sample compare function for ints
int compareInts(Pointer a, Pointer b) {
  return *(int*)a - *(int*)b;
}

TEST(LinkedListTest, create) {
  // Check that the list has actually been created
  List list;
  ASSERT_EQ(list, !NULL);
  ASSERT_EQ(list.getSize(), 0) << list.getSize();
};

TEST(LinkedListTest, insert) {
  List list;
  int N = 1000;
  int* array = new int[N * sizeof(*array)];

  for (int i = 0; i < N; i++) {
    list.insertNext(LIST_BOF, &array[i]);

    // Was the size updated correctly?
    ASSERT_EQ(list.getSize(), i + 1);

    // Does the head contain the value that we inserted?
    ASSERT_EQ(list.nodeValue(list.getHead()), &array[i]);
  }

  // Check if the items are in reverse order
  ListNode node = list.getHead();

  for (int i = N - 1; i >= 0; i--) {
    ASSERT_EQ(list.nodeValue(node), &array[i]);
    node = list.next(node);
  }

  // Inserting NULL after the first node
  ListNode head_node = list.getHead();
  list.insertNext(head_node, nullptr);
  ASSERT_EQ(list.nodeValue(list.next(head_node)), nullptr);

  delete array;
};

TEST(LinkedListTest, removeNext) {
  // a list that automatically calls delete upon removing a node
  List list(operator delete);

  int N = 1000;
  int** array = new int*[N * sizeof(*array)];

  // dynamically allocated elements
  for (int i = 0; i < N; i++) {
    array[i] = new int;
    *array[i] = i;
    list.insertNext(LIST_BOF, array[i]);
  }

  for (int i = N - 1; i >= 0; i--) {
    // remove the head and check to see if it is the same as the input
    ASSERT_EQ(list.nodeValue(list.getHead()), array[i]);
    list.removeNext(LIST_BOF);

    // make sure the size is decreasing
    ASSERT_EQ(list.getSize(), i);
  }

  // now we check removal from an intermediary node
  for (int i = 0; i < N; i++) {
    array[i] = new int;
    *array[i] = i;
    list.insertNext(LIST_BOF, array[i]);
  }

  list.removeNext(list.getHead());
  ASSERT_EQ(list.getSize(), N - 1);

  delete array;
}

TEST(LinkedListTest, find) {
  List list;
  int N = 1000;
  int* array = new int[N * sizeof(*array)];

  // populate the list
  for (int i = 0; i < N; i++) {
    array[i] = i;
    list.insertNext(LIST_BOF, &array[i]);
  }

  for (int i = 0; i < N; i++) {
    int* value = (int*)list.find(&i, compareInts);
    ASSERT_EQ(value, &array[i]);
  }

  // non-existent value
  int not_exists = -1;
  ASSERT_EQ(list.find(&not_exists, compareInts), nullptr);

  delete array;
}

TEST(LinkedListTest, findNode) {
  List list;
  int N = 1000;
  int* array = new int[N * sizeof(*array)];

  for (int i = 0; i < N; i++) {
    array[i] = i;
    list.insertNext(LIST_BOF, &array[i]);
  }

  ListNode node = list.getHead();

  for (int i = N - 1; i >= 0; i--) {
    ListNode found_node = list.findNode(&i, compareInts);
    ASSERT_EQ(found_node, node);
    ASSERT_EQ(list.nodeValue(found_node), &array[i]);

    node = list.next(node);
  }

  delete array;
}