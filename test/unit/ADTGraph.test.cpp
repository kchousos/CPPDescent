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
#include "cppdescent/ADTPQueue.hpp"
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

int myCompareVertices(GraphVertex* v1, GraphVertex* v2) {
  return compareInts(v1->getData(), v2->getData());
}

int compareEdges(Pointer a, Pointer b) {
  GraphVertexPair* pair1 = (GraphVertexPair*)a;
  GraphVertexPair* pair2 = (GraphVertexPair*)b;
  int first = pair1->getOwner()->getCompareData()(
      ((GraphVertex*)pair1->getVertex1())->getData(),
      ((GraphVertex*)pair2->getVertex1())->getData());
  int second = pair1->getOwner()->getCompareData()(
      ((GraphVertex*)pair1->getVertex2())->getData(),
      ((GraphVertex*)pair2->getVertex2())->getData());
  if (first)
    return first;
  else if (second)
    return second;

  return 0;
}

// int compareEdges(Pointer first, Pointer second) {
//   GraphVertexPair* pair1 = (GraphVertexPair*)first;
//   GraphVertexPair* pair2 = (GraphVertexPair*)second;

//   if (pair1->getVertex1() != pair2->getVertex1() ||
//       pair1->getVertex2() != pair2->getVertex2())
//     return 1;

//   return 0;
// }

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
  GraphVertexPair* pair = (GraphVertexPair*)value;
  size_t hash = (size_t)pair->getVertex1()->getData() +
                (size_t)pair->getVertex2()->getData();
  return hash;
}

TEST(ADTGraphTest, create) {
  Graph* graph = new Graph(1, compareInts, nullptr);

  ASSERT_NE(graph, nullptr);
  ASSERT_EQ(graph->getSize(), 0);

  delete graph;
}

TEST(ADTGraphTest, removeEdges) {
  int N = 10;
  Graph* graph = new Graph(N, compareInts, deleteInts);

  graph->setHashFunction(hashPointer);

  int** vertexArray = new int*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = createIntValue(i);
    graph->insertVertex(vertexArray[i]);
  }

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      graph->insertEdge(vertexArray[i], vertexArray[j],
                        *vertexArray[i] - *vertexArray[j]);
      ASSERT_FLOAT_EQ(graph->getWeight(vertexArray[i], vertexArray[j]),
                      *vertexArray[i] - *vertexArray[j]);
    }

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      graph->removeEdge(vertexArray[i], vertexArray[j]);
      List* adjacent = graph->getAdjacent(vertexArray[i]);
      ASSERT_EQ(adjacent->getSize(), N - j - 1);
      GraphVertex* v = new GraphVertex(vertexArray[j], graph);
      ASSERT_EQ(adjacent->find(v, (CompareFunc)myCompareVertices), nullptr);
      delete v;
      delete adjacent;
    }

  for (int i = 0; i < N; i++)
    delete vertexArray[i];

  delete[] vertexArray;
  delete graph;
}

TEST(ADTGraphTest, insertRemove) {
  int N = 1000;
  Graph* graph = new Graph(N, compareInts, deleteInts);

  graph->setHashFunction(hashPointer);

  int** vertexArray = new int*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = createIntValue(i);
    graph->insertVertex(vertexArray[i]);

    ASSERT_EQ(graph->getSize(), i + 1);
  }

  List* list = graph->getVertices();
  ListNode* node = list->getHead();

  for (int i = 0; i < N; i++) {
    ASSERT_EQ(((GraphVertex*)node->getValue())->getData(), vertexArray[i]);
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
  int N = 1000;
  Graph* graph = new Graph(N, compareInts, deleteInts);

  graph->setHashFunction(hashPointer);

  int** vertexArray = new int*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = createIntValue(i);
    graph->insertVertex(vertexArray[i]);
    List* list = graph->getVertices();
    int* value = createIntValue(i);
    GraphVertex* v = new GraphVertex(value, graph);
    ASSERT_EQ(((GraphVertex*)list->find(v, (CompareFunc)myCompareVertices))
                  ->getData(),
              vertexArray[i]);
    delete value;
    delete v;
    delete list;
  }

  for (int i = 1; i < N; i++)
    graph->insertEdge(vertexArray[0], vertexArray[i], i);

  List* list2 = graph->getAdjacent(vertexArray[0]);
  ASSERT_NE(list2->getHead(), nullptr);

  ListNode* node = list2->getHead();

  for (int i = 1; i < N; i++) {
    ASSERT_EQ(((GraphVertex*)node->getValue())->getData(), vertexArray[i])
        << "i: " << i << "\n";
    node = list2->next(node);
  }

  for (int i = 0; i < N; i++)
    delete vertexArray[i];

  delete[] vertexArray;
  delete list2;
  delete graph;
}

TEST(ADTGraphTest, getAdjacentPQ) {
  int N = 1000;
  Graph* graph = new Graph(N, compareInts, deleteInts);

  graph->setHashFunction(hashPointer);

  int** vertexArray = new int*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = createIntValue(i);
    graph->insertVertex(vertexArray[i]);
    List* list = graph->getVertices();
    int* value = createIntValue(i);
    GraphVertex* v = new GraphVertex(value, graph);
    ASSERT_EQ(((GraphVertex*)list->find(v, (CompareFunc)myCompareVertices))
                  ->getData(),
              vertexArray[i]);
    delete value;
    delete v;
    delete list;
  }

  for (int i = 1; i < N; i++)
    graph->insertEdge(vertexArray[0], vertexArray[i], i);

  PQueue* adjPQ = graph->getAdjacentPQ(vertexArray[0]);
  ASSERT_NE(adjPQ->getMax(), nullptr);
  for (int i = 1; i < N; i++) {
    GraphVertexPair* pair = (GraphVertexPair*)adjPQ->getMax();
    GraphVertex* v = pair->getVertex2();
    int* data = (int*)v->getData();
    ASSERT_EQ(compareInts(data, vertexArray[N - i]), 0) << "i: " << i << "\n";
    adjPQ->removeMax();
  }

  for (int i = 0; i < N; i++)
    delete vertexArray[i];

  delete[] vertexArray;
  delete graph;
}

TEST(ADTGraphTest, getReverseAdjacent) {
  int N = 1000;
  Graph* graph = new Graph(N, compareInts, deleteInts);

  graph->setHashFunction(hashPointer);

  int** vertexArray = new int*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = createIntValue(i);
    graph->insertVertex(vertexArray[i]);
    List* list = graph->getVertices();
    int* value = createIntValue(i);
    GraphVertex* v = new GraphVertex(value, graph);
    ASSERT_EQ(((GraphVertex*)list->find(v, (CompareFunc)myCompareVertices))
                  ->getData(),
              vertexArray[i]);
    delete value;
    delete v;
    delete list;
  }

  for (int i = 1; i < N; i++)
    graph->insertEdge(vertexArray[i], vertexArray[0], i);

  List* list2 = graph->getReverseAdjacent(vertexArray[0]);
  ASSERT_NE(list2->getHead(), nullptr);

  ListNode* node = list2->getHead();

  for (int i = 1; i < N; i++) {
    ASSERT_EQ(((GraphVertex*)node->getValue())->getData(), vertexArray[i])
        << "i: " << i << "\n";
    node = list2->next(node);
  }

  for (int i = 0; i < N; i++)
    delete vertexArray[i];

  delete[] vertexArray;
  delete list2;
  delete graph;
}

TEST(ADTGraphTest, getReverseAdjacentPQ) {
  int N = 1000;
  Graph* graph = new Graph(N, compareInts, deleteInts);

  graph->setHashFunction(hashPointer);

  int** vertexArray = new int*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = createIntValue(i);
    graph->insertVertex(vertexArray[i]);
    List* list = graph->getVertices();
    int* value = createIntValue(i);
    GraphVertex* v = new GraphVertex(value, graph);
    ASSERT_EQ(((GraphVertex*)list->find(v, (CompareFunc)myCompareVertices))
                  ->getData(),
              vertexArray[i]);
    delete v;
    delete value;
    delete list;
  }

  for (int i = 1; i < N; i++)
    graph->insertEdge(vertexArray[i], vertexArray[0], i);

  PQueue* revAdjPQ = graph->getReverseAdjacentPQ(vertexArray[0]);
  ASSERT_NE(revAdjPQ->getMax(), nullptr);

  for (int i = 1; i < N; i++) {
    GraphVertexPair* pair = (GraphVertexPair*)revAdjPQ->getMax();
    GraphVertex* v = pair->getVertex1();
    int* data = (int*)v->getData();
    ASSERT_EQ(compareInts(data, vertexArray[N - i]), 0) << "i: " << i << "\n";
    revAdjPQ->removeMax();
  }

  for (int i = 0; i < N; i++)
    delete vertexArray[i];

  delete[] vertexArray;
  delete graph;
}

TEST(ADTGraphTest, getGeneralNeighbors) {
  int N = 1000;
  Graph* graph = new Graph(N, compareInts, deleteInts);

  graph->setHashFunction(hashPointer);

  int** vertexArray = new int*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = createIntValue(i);
    graph->insertVertex(vertexArray[i]);
    List* list = graph->getVertices();
    int* value = createIntValue(i);
    GraphVertex* v = new GraphVertex(value, graph);
    ASSERT_EQ(((GraphVertex*)list->find(v, (CompareFunc)myCompareVertices))
                  ->getData(),
              vertexArray[i]);
    delete value;
    delete v;
    delete list;
  }

  for (int i = 1; i < N; i++)
    graph->insertEdge(vertexArray[0], vertexArray[i], i);

  for (int i = 1; i < N; i++)
    graph->insertEdge(vertexArray[i], vertexArray[0], i);

  List* list = graph->getGeneralNeighbors(vertexArray[0]);
  ASSERT_NE(list->getHead(), nullptr);

  ListNode* node = list->getHead();

  for (int i = 1; i < 2 * N - 1; i++) {
    if (i < N) {
      ASSERT_EQ(((GraphVertex*)node->getValue())->getData(), vertexArray[i])
          << "i: " << i << "\n";
      node = list->next(node);
    } else {
      ASSERT_EQ(((GraphVertex*)node->getValue())->getData(),
                vertexArray[i - N + 1])
          << "i: " << i << "\n";
      node = list->next(node);
    }
  }

  for (int i = 0; i < N; i++)
    delete vertexArray[i];

  delete[] vertexArray;
  delete list;
  delete graph;
}

TEST(ADTGraphTest, getGeneralNeighborsPQ) {
  int N = 1000;
  Graph* graph = new Graph(N, compareInts, deleteInts);

  graph->setHashFunction(hashPointer);

  int** vertexArray = new int*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = createIntValue(i);
    graph->insertVertex(vertexArray[i]);
    List* list = graph->getVertices();
    int* value = createIntValue(i);
    GraphVertex* v = new GraphVertex(value, graph);
    ASSERT_EQ(((GraphVertex*)list->find(v, (CompareFunc)myCompareVertices))
                  ->getData(),
              vertexArray[i]);
    delete value;
    delete v;
    delete list;
  }

  for (int i = 1; i < N; i++)
    graph->insertEdge(vertexArray[0], vertexArray[i], i);

  int** vertexArray2 = new int*[N];

  for (int i = 0; i < N; i++) {
    vertexArray2[i] = createIntValue(N + i);
    graph->insertVertex(vertexArray2[i]);
    List* list = graph->getVertices();
    int* value = createIntValue(N + i);
    GraphVertex* v = new GraphVertex(value, graph);
    ASSERT_EQ(((GraphVertex*)list->find(v, (CompareFunc)myCompareVertices))
                  ->getData(),
              vertexArray2[i]);
    delete value;
    delete v;
    delete list;
  }

  for (int i = 1; i < N; i++)
    graph->insertEdge(vertexArray2[i], vertexArray[0], N + i);

  PQueue* generalNPQ = graph->getGeneralNeighborsPQ(vertexArray[0]);
  ASSERT_NE(generalNPQ->getMax(), nullptr);

  for (int i = 1; i < 2 * N - 1; i++) {
    if (i < N) {
      GraphVertexPair* pair = (GraphVertexPair*)generalNPQ->getMax();
      GraphVertex* v = pair->getVertex1();
      int* data = (int*)v->getData();
      ASSERT_EQ(compareInts(data, vertexArray2[N - i]), 0)
          << "i: " << i << "\n";
      generalNPQ->removeMax();
    } else {
      GraphVertexPair* pair = (GraphVertexPair*)generalNPQ->getMax();
      GraphVertex* v = pair->getVertex2();
      int* data = (int*)v->getData();
      ASSERT_EQ(compareInts(data, vertexArray[2 * N - i - 1]), 0)
          << "i: " << i << "\n";
      generalNPQ->removeMax();
    }
  }

  for (int i = 0; i < N; i++)
    delete vertexArray[i];

  for (int i = 0; i < N; i++)
    delete vertexArray2[i];

  delete[] vertexArray;
  delete[] vertexArray2;
  delete graph;
}