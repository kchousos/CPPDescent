/**
 * @file cppdescent.test.cpp
 * @author Konstantinos Chousos, Phaedon Seitanidis
 * @brief Unit tests for the different uses of the cppdescent library.
 * @version 0.1
 * @date 2023-11-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cppdescent/cppdescent.hpp"
#include <cmath>
#include "gtest/gtest.h"

/**
 * @brief Returns the Euclidean distance between two points of arbitrary
 * dimension.
 *
 * @param first A pointer to the first point.
 * @param second A pointer to the second point.
 * @return long double The Euclidean distance.
 */
long double euclideanDistance(Pointer a, Pointer b) {
  Vector* first = (Vector*)a;
  Vector* second = (Vector*)b;
  long double result = 0;

  if (first->getSize() != second->getSize())
    return -1.0;

  for (int i = 0; i < first->getSize(); i++) {
    float diff = *(float*)first->getAt(i) - *(float*)second->getAt(i);
    result += diff * diff;
  }

  result = sqrtl(result);
  return result;
}

/**
 * @brief Compare edges using the euclideanDistance function.
 *
 * @param first A Pointer to the first element.
 * @param second A Pointer to the second element.
 * @return int
 */
int compareEdgesEuclidean(Pointer first, Pointer second) {
  GraphVertexPair* pair1 = (GraphVertexPair*)first;
  GraphVertexPair* pair2 = (GraphVertexPair*)second;

  long double a = euclideanDistance((Vector*)pair1->getVertex1(),
                                    (Vector*)pair1->getVertex2());
  long double b = euclideanDistance((Vector*)pair2->getVertex1(),
                                    (Vector*)pair2->getVertex2());

  int value = 0;
  if (b > a) {
    value = 1;
  } else if (a > b) {
    value = -1;
  }
  return value;
}

struct BruteForceManualDataset : testing::Test {
 protected:
  virtual void SetUp() override {
    Vector* point1 = new Vector(2, cppdescent::deleteFloat);
    Vector* point2 = new Vector(2, cppdescent::deleteFloat);
    Vector* point3 = new Vector(2, cppdescent::deleteFloat);
    Vector* point4 = new Vector(2, cppdescent::deleteFloat);
    Vector* point5 = new Vector(2, cppdescent::deleteFloat);
    // first point
    point1->setAt(0, cppdescent::createFloat(1.4));
    point1->setAt(1, cppdescent::createFloat(2.6));
    vec->setAt(0, point1);
    // second point
    point2->setAt(0, cppdescent::createFloat(15.4));
    point2->setAt(1, cppdescent::createFloat(-14.28));
    vec->setAt(1, point2);
    // third point
    point3->setAt(0, cppdescent::createFloat(23.3));
    point3->setAt(1, cppdescent::createFloat(11.8));
    vec->setAt(2, point3);
    // fourth point
    point4->setAt(0, cppdescent::createFloat(-128.04));
    point4->setAt(1, cppdescent::createFloat(3.7));
    vec->setAt(3, point4);
    // fifth point
    point5->setAt(0, cppdescent::createFloat(1317.6));
    point5->setAt(1, cppdescent::createFloat(-2535.7));
    vec->setAt(4, point5);
  }
  Vector* vec = new Vector(5, nullptr);
};

// TEST(IO, readData) {
//   Vector* vec = cppdescent::readBinData("./datasets/00000020.bin", 100);
//   ASSERT_NE(vec, nullptr);

//   float lastValue = 0.0726192221;

//   Vector* lastElement = (Vector*)vec->getAt(vec->getSize() - 1);
//   ASSERT_FLOAT_EQ(lastValue,
//                   *(float*)lastElement->getAt(lastElement->getSize() - 1));

//   int result = cppdescent::deleteDatapointVectors(vec);
//   ASSERT_EQ(result, 0);
// }

TEST(BruteForce, SIGMODDataset20) {
  Vector* vec = cppdescent::readBinData(
      "/home/kchou/Documents/DiT/7ο Εξάμηνο/Ανάπτυξη Λογισμικού για "
      "Πληροφοριακά Συστήματα (Project)/Εργασίες/Εργασία "
      "1/datasets/00000020.bin",
      100);

  int K[] = {3, 5, 10};

  for (int k = 0; k < 3; k++) {
    Graph* graph = cppdescent::KNNBruteForceGraph(
        vec, K[k], compareEdgesEuclidean, euclideanDistance);

    List* vertices = graph->getVertices();
    ASSERT_EQ(vertices->getSize(), 20);

    ListNode* vertex = vertices->getHead();

    for (int i = 0; i < vertices->getSize(); i++) {
      List* adjacent = graph->getAdjacent((Pointer)vertex->getValue());
      ASSERT_EQ(adjacent->getSize(), K[k]);
      delete adjacent;
      vertex = vertex->getNext();
    }

    delete vertices;
    delete graph;
  }

  int result = cppdescent::deleteDatapointVectors(vec);
  ASSERT_EQ(result, 0);
}

// TEST(BruteForce, SIGMODDataset1000) {
//   Vector* vec = cppdescent::readBinData("./datasets/00001000-1.bin", 100);

//   int K[] = {4, 5, 10};

//   for (int k = 0; k < 1; k++) {
//     Graph* graph = cppdescent::KNNBruteForceGraph(
//         vec, K[k], compareEdgesEuclidean, euclideanDistance);

//     // List* vertices = graph->getVertices();
//     // ASSERT_EQ(vertices->getSize(), 1000);

//     // ListNode* vertex = vertices->getHead();

//     // for (int i = 0; i < vertices->getSize(); i++) {
//     //   List* adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//     //   ASSERT_EQ(adjacent->getSize(), K[k]);
//     //   delete adjacent;
//     //   vertex = vertex->getNext();
//     // }

//     // delete vertices;
//     delete graph;
//   }

//   int result = cppdescent::deleteDatapointVectors(vec);
//   ASSERT_EQ(result, 0);
// }

TEST_F(BruteForceManualDataset, KEqualTo2) {
  int K = 2;

  Graph* graph = cppdescent::KNNBruteForceGraph(vec, K, compareEdgesEuclidean,
                                                euclideanDistance);

  List* vertices = graph->getVertices();
  ASSERT_EQ(vertices->getSize(), 5);

  ListNode* vertex = vertices->getHead();

  // The KNN graph for this small dataset has been computed by hand,
  // so we manually check each vertex's neighbors.

  // Test 1st vertex.

  List* adjacent = graph->getAdjacent((Pointer)vertex->getValue());
  ASSERT_EQ(adjacent->getSize(), K);
  ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
            nullptr);

  delete adjacent;
  vertex = vertex->getNext();

  // Test 2nd vertex.

  adjacent = graph->getAdjacent((Pointer)vertex->getValue());
  ASSERT_EQ(adjacent->getSize(), K);
  ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
            nullptr);

  delete adjacent;
  vertex = vertex->getNext();

  // Test 3rd vertex.

  adjacent = graph->getAdjacent((Pointer)vertex->getValue());
  ASSERT_EQ(adjacent->getSize(), K);
  ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
            nullptr);

  delete adjacent;
  vertex = vertex->getNext();

  // Test 4th vertex.

  adjacent = graph->getAdjacent((Pointer)vertex->getValue());
  ASSERT_EQ(adjacent->getSize(), K);
  ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
            nullptr);

  delete adjacent;
  vertex = vertex->getNext();

  // Test 5th vertex.

  adjacent = graph->getAdjacent((Pointer)vertex->getValue());
  ASSERT_EQ(adjacent->getSize(), K);
  ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
            nullptr);

  delete adjacent;
  vertex = vertex->getNext();

  delete vertices;
  delete graph;

  int result = cppdescent::deleteDatapointVectors(vec);
  ASSERT_EQ(result, 0);
}

TEST_F(BruteForceManualDataset, KEqualTo3) {
  int K = 3;

  Graph* graph = cppdescent::KNNBruteForceGraph(vec, K, compareEdgesEuclidean,
                                                euclideanDistance);

  List* vertices = graph->getVertices();
  ASSERT_EQ(vertices->getSize(), 5);

  ListNode* vertex = vertices->getHead();

  // The KNN graph for this small dataset has been computed by hand,
  // so we manually check each vertex's neighbors.

  // Test 1st vertex.

  List* adjacent = graph->getAdjacent((Pointer)vertex->getValue());
  ASSERT_EQ(adjacent->getSize(), K);
  ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(3), cppdescent::compareVertices),
            nullptr);

  delete adjacent;
  vertex = vertex->getNext();

  // Test 2nd vertex.

  adjacent = graph->getAdjacent((Pointer)vertex->getValue());
  ASSERT_EQ(adjacent->getSize(), K);
  ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(3), cppdescent::compareVertices),
            nullptr);

  delete adjacent;
  vertex = vertex->getNext();

  // Test 3rd vertex.

  adjacent = graph->getAdjacent((Pointer)vertex->getValue());
  ASSERT_EQ(adjacent->getSize(), K);
  ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(3), cppdescent::compareVertices),
            nullptr);

  delete adjacent;
  vertex = vertex->getNext();

  // Test 4th vertex.

  adjacent = graph->getAdjacent((Pointer)vertex->getValue());
  ASSERT_EQ(adjacent->getSize(), K);
  ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
            nullptr);

  delete adjacent;
  vertex = vertex->getNext();

  // Test 5th vertex.

  adjacent = graph->getAdjacent((Pointer)vertex->getValue());
  ASSERT_EQ(adjacent->getSize(), K);
  ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
            nullptr);
  ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
            nullptr);

  delete adjacent;
  vertex = vertex->getNext();

  delete vertices;
  delete graph;

  int result = cppdescent::deleteDatapointVectors(vec);
  ASSERT_EQ(result, 0);
}
