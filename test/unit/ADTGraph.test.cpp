/**
 * @file ADTGraph.test.cpp
 * @author Pheadon Seitanidis
 * @brief Unit tests for ADTGraph
 * @version 0.1
 * @date 2023-11-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cppdescent/ADTGraph.hpp"
#include "cppdescent/ADTList.hpp"
#include "gtest/gtest.h"

/**
 * @brief A simple function to compare 2 integer pointers.
 *
 * @param a Int pointer 1
 * @param b Int pointer 2
 * @return int < 0 => a < b, > 0 => a > b, = 0 => a == b
 */
int compareInts(Pointer a, Pointer b) {
  return *(int*)a - *(int*)b;
}

/**
 * @brief Delete an int pointer.
 *
 * @param value The pointer to be deleted.
 */
void deleteInts(Pointer value) {
  delete (int*)value;
}

/**
 * @brief Create an int object in a new memory space with the given value.
 *
 * @param value The value of the created int object.
 * @return int* The created pointer p, where *p = value.
 */
int* createIntValue(int value) {
  int* p = new int;
  *p = value;
  return p;
}

/**
 * @brief Cast to size_t that has the same length as a Pointer
 *
 * @param value The value to be hashed.
 * @return unsigned int The hash value.
 */
uint hashPointer(Pointer value) {
  // return (size_t)&value;
  int* v = (int*)value;
  v = v + 1;
  return 0;
}

TEST(ADTGraphTest, create) {
  Graph* graph = new Graph(compareInts, nullptr);

  ASSERT_NE(graph, nullptr);
  ASSERT_EQ(graph->getSize(), 0);

  delete graph;
}

TEST(ADTGraphTest, insertRemove) {
  Graph* graph = new Graph(compareInts, deleteInts);

  graph->setHashFunction(hashPointer);
  int N = 1000;

  int** vertexArray = new int*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = createIntValue(i);
    graph->insertVertex(vertexArray[i]);

    ASSERT_EQ(graph->getSize(), i + 1);
  }

  List* list = graph->getVertices();
  ListNode* node = list->getHead();

  for (int i = 0; i < N; i++) {
    ASSERT_EQ(node->getValue(), vertexArray[i]);
    node = node->getNext();
  }

  for (int i = 0; i < N; i++) {
    graph->removeVertex(vertexArray[i]);
    ASSERT_EQ(graph->getSize(), N - i - 1);
  }

  delete list;

  list = graph->getVertices();
  ASSERT_EQ(list->getSize(), 0);

  for (int i = 0; i < N; i++)
    delete vertexArray[i];

  delete[] vertexArray;

  delete list;
  delete graph;
}

TEST(ADTGraphTest, getAdjacent) {
  Graph* graph = new Graph(compareInts, deleteInts);

  graph->setHashFunction(hashPointer);

  int N = 1000;

  int** vertexArray = new int*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = createIntValue(i);
    graph->insertVertex(vertexArray[i]);
    List* list = graph->getVertices();
    int* value = createIntValue(i);
    ASSERT_EQ(list->find(value, compareInts), vertexArray[i]);
    delete value;
    delete list;
  }

  for (int i = 1; i < N; i++)
    graph->insertEdge(vertexArray[0], vertexArray[i], i);

  List* list2 = graph->getAdjacent(vertexArray[0]);
  ASSERT_NE(list2->getHead(), nullptr);

  ListNode* node = list2->getHead();

  for (int i = 1; i < N; i++) {
    ASSERT_EQ(node->getValue(), vertexArray[i]) << "i: " << i << "\n";
    node = list2->next(node);
  }

  for (int i = 0; i < N; i++)
    delete vertexArray[i];

  delete[] vertexArray;
  delete list2;
  delete graph;
}
