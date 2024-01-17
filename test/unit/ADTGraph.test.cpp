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
#include <gsl/gsl_vector.h>
#include "cppdescent/ADTPQueue.hpp"
#include "cppdescent/cppdescent.hpp"
#include "gtest/gtest.h"

int compareNeighborsTest(Pointer vertex1, Pointer vertex2) {
  GraphVertex* v1 = (GraphVertex*)((GraphVertexPair*)vertex1)->getVertex2();
  GraphVertex* v2 = (GraphVertex*)vertex2;

  if (gsl_vector_equal((gsl_vector*)v1->getData(), (gsl_vector*)v2->getData()))
    return 0;
  else
    return 1;
}

int compareReverseTest(Pointer vertex1, Pointer vertex2) {
  GraphVertex* v1 = (GraphVertex*)((GraphVertexPair*)vertex1)->getVertex1();
  GraphVertex* v2 = (GraphVertex*)vertex2;

  if (gsl_vector_equal((gsl_vector*)v1->getData(), (gsl_vector*)v2->getData()))
    return 0;
  else
    return 1;
}

TEST(ADTGraphTest, create) {
  Graph* graph = new Graph(nullptr, nullptr);

  ASSERT_NE(graph, nullptr);
  ASSERT_EQ(graph->getSize(), 0);

  delete graph;
}

TEST(ADTGraphTest, insertRemoveVertex) {
  Graph* graph = new Graph(nullptr, nullptr);

  int N = 10;

  gsl_vector** vertexArray = new gsl_vector*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = gsl_vector_alloc(N);

    for (int j = 0; j < N; j++)
      gsl_vector_set(vertexArray[i], j, (float)(rand() % N + 1));
  }

  for (int i = 0; i < N; i++) {
    graph->insertVertex(vertexArray[i]);
    ASSERT_EQ(graph->getSize(), i + 1);
  }

  // Try to insert an already inserted value

  graph->insertVertex(vertexArray[0]);
  ASSERT_EQ(graph->getSize(), N);

  Vector* vertices = graph->getVerticesV();

  for (int i = 0; i < N; i++)
    ASSERT_EQ(((GraphVertex*)vertices->getAt(i))->getData(), vertexArray[i]);

  for (int i = 0; i < N; i++) {
    graph->removeVertex(vertexArray[i]);
    ASSERT_EQ(graph->getSize(), N - i - 1);
  }

  delete[] vertexArray;
  delete graph;
}

TEST(ADTGraphTest, insertRemoveEdges) {
  Graph* graph = new Graph(nullptr, nullptr);

  int N = 10;

  gsl_vector** vertexArray = new gsl_vector*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = gsl_vector_alloc(N);

    for (int j = 0; j < N; j++)
      gsl_vector_set(vertexArray[i], j, (float)(rand() % N + 1));
  }

  for (int i = 0; i < N; i++) {
    graph->insertVertex(vertexArray[i]);
    ASSERT_EQ(graph->getSize(), i + 1);
  }

  GraphVertex** gvertexArray = new GraphVertex*[N];

  for (int i = 0; i < N; i++)
    gvertexArray[i] = new GraphVertex(vertexArray[i], graph);

  for (int i = 1; i < N; i++) {
    graph->insertEdge(gvertexArray[0], gvertexArray[i]);
    ASSERT_TRUE(graph->isNeighborVertex(gvertexArray[0], gvertexArray[i]));
    ASSERT_EQ(gvertexArray[0]->getNeighbors()->getSize(), i);
  }

  // Try to insert an already inserted edge

  graph->insertEdge(gvertexArray[0], gvertexArray[1]);
  ASSERT_EQ(gvertexArray[0]->getNeighbors()->getSize(), N - 1);

  for (int i = 1; i < N; i++) {
    graph->removeEdge(gvertexArray[0], gvertexArray[i]);
    ASSERT_FALSE(graph->isNeighborVertex(gvertexArray[0], gvertexArray[i]));
  }

  for (int i = 0; i < N; i++)
    delete gvertexArray[i];

  delete[] gvertexArray;
  delete[] vertexArray;
  delete graph;
}

TEST(ADTGraphTest, getAdjacent) {
  Graph* graph = new Graph(nullptr, nullptr);

  int N = 10;

  gsl_vector** vertexArray = new gsl_vector*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = gsl_vector_alloc(N);

    for (int j = 0; j < N; j++)
      gsl_vector_set(vertexArray[i], j, (float)(rand() % N + 1));
  }

  for (int i = 0; i < N; i++) {
    graph->insertVertex(vertexArray[i]);
    ASSERT_EQ(graph->getSize(), i + 1);
  }

  GraphVertex** gvertexArray = new GraphVertex*[N];

  for (int i = 0; i < N; i++)
    gvertexArray[i] = new GraphVertex(vertexArray[i], graph);

  for (int i = 1; i < N; i++) {
    graph->insertEdge(gvertexArray[0], gvertexArray[i]);
    ASSERT_TRUE(graph->isNeighborVertex(gvertexArray[0], gvertexArray[i]));
  }

  Vector* adj = graph->getAdjacentV(gvertexArray[0]);

  for (int i = 1; i < N; i++)
    ASSERT_NE(adj->find(gvertexArray[i], compareNeighborsTest), nullptr);

  for (int i = 0; i < N; i++)
    delete gvertexArray[i];

  delete[] gvertexArray;
  delete[] vertexArray;
  delete graph;
}

TEST(ADTGraphTest, getReverse) {
  Graph* graph = new Graph(nullptr, nullptr);

  int N = 10;

  gsl_vector** vertexArray = new gsl_vector*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = gsl_vector_alloc(N);

    for (int j = 0; j < N; j++)
      gsl_vector_set(vertexArray[i], j, (float)(rand() % N + 1));
  }

  for (int i = 0; i < N; i++) {
    graph->insertVertex(vertexArray[i]);
    ASSERT_EQ(graph->getSize(), i + 1);
  }

  GraphVertex** gvertexArray = new GraphVertex*[N];

  for (int i = 0; i < N; i++)
    gvertexArray[i] = new GraphVertex(vertexArray[i], graph);

  for (int i = 1; i < N; i++) {
    graph->insertEdge(gvertexArray[0], gvertexArray[i]);
    ASSERT_TRUE(graph->isNeighborVertex(gvertexArray[0], gvertexArray[i]));
  }

  for (int i = 1; i < N; i++) {
    Vector* adj = graph->getReverseAdjacentV(gvertexArray[i]);
    ASSERT_NE(adj->find(gvertexArray[0], compareReverseTest), nullptr);
  }

  for (int i = 0; i < N; i++)
    delete gvertexArray[i];

  delete[] gvertexArray;
  delete[] vertexArray;
  delete graph;
}

TEST(ADTGraphTest, getGenearalNeighbors) {
  Graph* graph = new Graph(nullptr, nullptr);

  int N = 10;

  gsl_vector** vertexArray = new gsl_vector*[N];

  for (int i = 0; i < N; i++) {
    vertexArray[i] = gsl_vector_alloc(N);

    for (int j = 0; j < N; j++)
      gsl_vector_set(vertexArray[i], j, (float)(rand() % N + 1));
  }

  for (int i = 0; i < N; i++) {
    graph->insertVertex(vertexArray[i]);
    ASSERT_EQ(graph->getSize(), i + 1);
  }

  GraphVertex** gvertexArray = new GraphVertex*[N];

  for (int i = 0; i < N; i++)
    gvertexArray[i] = new GraphVertex(vertexArray[i], graph);

  for (int i = 1; i < N; i++) {
    graph->insertEdge(gvertexArray[0], gvertexArray[i]);
    graph->insertEdge(gvertexArray[i], gvertexArray[0]);
    ASSERT_TRUE(graph->isNeighborVertex(gvertexArray[0], gvertexArray[i]));
  }

  Vector* adj = graph->getGeneralNeighborsV(gvertexArray[0]);

  for (int i = 1; i < N; i++)
    ASSERT_NE(adj->find(gvertexArray[i], compareNeighborsTest), nullptr);

  for (int i = 1; i < N; i++)
    ASSERT_NE(adj->find(gvertexArray[i], compareReverseTest), nullptr);

  for (int i = 0; i < N; i++)
    delete gvertexArray[i];

  delete adj;
  delete[] gvertexArray;
  delete[] vertexArray;
  delete graph;
}

// TEST(ADTGraphTest, getGeneralNeighbors) {
//   Graph* graph = new Graph(compareInts, deleteInts);

//   graph->setHashFunction(hashPointer);

//   int N = 1000;

//   int** vertexArray = new int*[N];

//   for (int i = 0; i < N; i++) {
//     vertexArray[i] = createIntValue(i);
//     graph->insertVertex(vertexArray[i]);
//     List* list = graph->getVertices();
//     int* value = createIntValue(i);
//     ASSERT_EQ(list->find(value, compareInts), vertexArray[i]);
//     delete value;
//     delete list;
//   }

//   for (int i = 1; i < N; i++)
//     graph->insertEdge(vertexArray[0], vertexArray[i], i);

//   for (int i = 1; i < N; i++)
//     graph->insertEdge(vertexArray[i], vertexArray[0], i);

//   List* list = graph->getGeneralNeighbors(vertexArray[0]);
//   ASSERT_NE(list->getHead(), nullptr);

//   ListNode* node = list->getHead();

//   for (int i = 1; i < 2 * N - 1; i++) {
//     if (i < N) {
//       ASSERT_EQ(node->getValue(), vertexArray[i]) << "i: " << i << "\n";
//       node = list->next(node);
//     } else {
//       ASSERT_EQ(node->getValue(), vertexArray[i - N + 1]) << "i: " << i <<
//       "\n"; node = list->next(node);
//     }
//   }

//   for (int i = 0; i < N; i++)
//     delete vertexArray[i];

//   delete[] vertexArray;
//   delete list;
//   delete graph;
// }

// TEST(ADTGraphTest, getGeneralNeighborsPQ) {
//   Graph* graph = new Graph(compareInts, deleteInts);

//   graph->setHashFunction(hashPointer);

//   int N = 1000;

//   int** vertexArray = new int*[N];

//   for (int i = 0; i < N; i++) {
//     vertexArray[i] = createIntValue(i);
//     graph->insertVertex(vertexArray[i]);
//     List* list = graph->getVertices();
//     int* value = createIntValue(i);
//     ASSERT_EQ(list->find(value, compareInts), vertexArray[i]);
//     delete value;
//     delete list;
//   }

//   for (int i = 1; i < N; i++)
//     graph->insertEdge(vertexArray[0], vertexArray[i], i);

//   int** vertexArray2 = new int*[N];

//   for (int i = 0; i < N; i++) {
//     vertexArray2[i] = createIntValue(N + i);
//     graph->insertVertex(vertexArray2[i]);
//     List* list = graph->getVertices();
//     int* value = createIntValue(N + i);
//     ASSERT_EQ(list->find(value, compareInts), vertexArray2[i]);
//     delete value;
//     delete list;
//   }
//   for (int i = 1; i < N; i++)
//     graph->insertEdge(vertexArray2[i], vertexArray[0], N + i);

//   PQueue* generalNPQ = graph->getGeneralNeighborsPQ(vertexArray[0]);
//   ASSERT_NE(generalNPQ->getMax(), nullptr);

//   for (int i = 1; i < 2 * N - 1; i++) {
//     if (i < N) {
//       GraphVertexPair* pair = (GraphVertexPair*)generalNPQ->getMax();
//       ASSERT_EQ(compareInts(pair->getVertex1(), vertexArray2[N - i]), 0)
//           << "i: " << i << "\n"
//           << generalNPQ->getSize() << std::endl;
//       generalNPQ->removeMax();
//     } else {
//       GraphVertexPair* pair = (GraphVertexPair*)generalNPQ->getMax();
//       ASSERT_EQ(compareInts(pair->getVertex2(), vertexArray[2 * N - i - 1]),
//       0)
//           << "i: " << i << "\n";
//       generalNPQ->removeMax();
//     }
//   }

//   for (int i = 0; i < N; i++)
//     delete vertexArray[i];

//   for (int i = 0; i < N; i++)
//     delete vertexArray2[i];

//   delete[] vertexArray;
//   delete[] vertexArray2;
//   delete graph;
// }