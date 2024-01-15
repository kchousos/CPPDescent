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

#define delta 0.001
#define rho 0.5

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

struct NNDescentManualDataset : testing::Test {
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

TEST(IO, readData) {
  Vector* vec = cppdescent::readBinData("./datasets/00000020.bin", 100);
  ASSERT_NE(vec, nullptr);

  float lastValue = 0.0726192221;

  Vector* lastElement = (Vector*)vec->getAt(vec->getSize() - 1);
  ASSERT_FLOAT_EQ(lastValue,
                  *(float*)lastElement->getAt(lastElement->getSize() - 1));

  delete vec;
}

// TEST_F(BruteForceManualDataset, binaryGraphFiles) {
//   int K = 2;

//   Graph* graph = cppdescent::KNNBruteForceGraph(
//       vec, K, (CompareFunc)cppdescent::compareEdgesEuclidean);

//   cppdescent::writeBinGraph("./build/cache/testgraph.bin", graph, K);

//   delete graph;

//   graph = cppdescent::readBinGraph("./build/cache/testgraph.bin", 2);

//   List* vertices = graph->getVertices();
//   ASSERT_EQ(vertices->getSize(), 5);

//   ListNode* vertex = vertices->getHead();

//   // The KNN graph for this small dataset has been computed by hand,
//   // so we manually check each vertex's neighbors.

//   // Test 1st vertex.

//   List* adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   // Test 2nd vertex.

//   adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   // Test 3rd vertex.

//   adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   // Test 4th vertex.

//   adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   // Test 5th vertex.

//   adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;

//   delete vertices;
//   delete graph;

//   // int result = cppdescent::deleteDatapointVectors(vec);
//   // ASSERT_EQ(result, 0);
// }

// TEST(BruteForce, SIGMODDataset20) {
//   Vector* vec = cppdescent::readBinData((char*)"./datasets/00000020.bin",
//   100);

//   int K[] = {3, 5, 10};

//   for (int k = 0; k < 3; k++) {
//     Graph* graph = cppdescent::KNNBruteForceGraph(
//         vec, K[k], (CompareFunc)cppdescent::compareEdgesEuclidean);

//     List* vertices = graph->getVertices();
//     ASSERT_EQ(vertices->getSize(), 20);

//     ListNode* vertex = vertices->getHead();

//     // FIXME
//     // for (int i = 0; i < vertices->getSize(); i++) {
//     //   List* adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//     //   ASSERT_EQ(adjacent->getSize(), K[k]);
//     //   delete adjacent;
//     //   vertex = vertex->getNext();
//     // }

//     delete vertices;
//     delete graph;
//   }

//   int result = cppdescent::deleteDatapointVectors(vec);
//   ASSERT_EQ(result, 0);
// }

// TEST_F(BruteForceManualDataset, KEqualTo2) {
//   int K = 2;

//   Graph* graph =
//       cppdescent::KNNBruteForceGraph(vec, K,
//       cppdescent::compareEdgesEuclidean);

//   List* vertices = graph->getVertices();
//   ASSERT_EQ(vertices->getSize(), 5);

//   ListNode* vertex = vertices->getHead();

//   // The KNN graph for this small dataset has been computed by hand,
//   // so we manually check each vertex's neighbors.

//   // Test 1st vertex.

//   List* adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   // Test 2nd vertex.

//   adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   // Test 3rd vertex.

//   adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   // Test 4th vertex.

//   adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   // Test 5th vertex.

//   adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   delete vertices;
//   delete graph;

//   int result = cppdescent::deleteDatapointVectors(vec);
//   ASSERT_EQ(result, 0);
// }

// TEST_F(BruteForceManualDataset, KEqualTo3) {
//   int K = 3;

//   Graph* graph =
//       cppdescent::KNNBruteForceGraph(vec, K,
//       cppdescent::compareEdgesEuclidean);

//   List* vertices = graph->getVertices();
//   ASSERT_EQ(vertices->getSize(), 5);

//   ListNode* vertex = vertices->getHead();

//   // The KNN graph for this small dataset has been computed by hand,
//   // so we manually check each vertex's neighbors.

//   // Test 1st vertex.

//   List* adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(3), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   // Test 2nd vertex.

//   adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(3), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   // Test 3rd vertex.

//   adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(3), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   // Test 4th vertex.

//   adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   // Test 5th vertex.

//   adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//   ASSERT_EQ(adjacent->getSize(), K);
//   ASSERT_NE(adjacent->find(vec->getAt(1), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(2), cppdescent::compareVertices),
//             nullptr);
//   ASSERT_NE(adjacent->find(vec->getAt(0), cppdescent::compareVertices),
//             nullptr);

//   delete adjacent;
//   vertex = vertex->getNext();

//   delete vertices;
//   delete graph;

//   int result = cppdescent::deleteDatapointVectors(vec);
//   ASSERT_EQ(result, 0);
// }

// TEST(MetricFunctions, euclidean) {
//   Vector* vec1 = new Vector(2, cppdescent::deleteFloat);
//   Vector* vec2 = new Vector(2, cppdescent::deleteFloat);

//   vec1->setAt(0, cppdescent::createFloat(1.2));
//   vec1->setAt(1, cppdescent::createFloat(3.3));

//   vec2->setAt(0, cppdescent::createFloat(4.5));
//   vec2->setAt(1, cppdescent::createFloat(10.2));

//   ASSERT_FLOAT_EQ(7.64853, cppdescent::euclideanDistance(vec1, vec2));

//   GraphVertexPair* pair1 = new GraphVertexPair(nullptr, vec1, vec2);
//   GraphVertexPair* pair2 = new GraphVertexPair(nullptr, vec1, vec2);

//   ASSERT_EQ(0, cppdescent::compareEdgesEuclidean(pair1, pair2));

//   delete vec1;
//   delete vec2;
//   delete pair1;
//   delete pair2;
// }

// TEST(MetricFunctions, manhattan) {
//   Vector* vec1 = new Vector(2, cppdescent::deleteFloat);
//   Vector* vec2 = new Vector(2, cppdescent::deleteFloat);

//   vec1->setAt(0, cppdescent::createFloat(1.2));
//   vec1->setAt(1, cppdescent::createFloat(3.3));

//   vec2->setAt(0, cppdescent::createFloat(4.5));
//   vec2->setAt(1, cppdescent::createFloat(10.2));

//   ASSERT_FLOAT_EQ(10.2, cppdescent::manhattanDistance(vec1, vec2));

//   GraphVertexPair* pair1 = new GraphVertexPair(nullptr, vec1, vec2);
//   GraphVertexPair* pair2 = new GraphVertexPair(nullptr, vec1, vec2);

//   ASSERT_EQ(0, cppdescent::compareEdgesManhattan(pair1, pair2));

//   delete vec1;
//   delete vec2;
//   delete pair1;
//   delete pair2;
// }

// TEST(HelperFunctions, recall) {
//   Vector* vec = cppdescent::readBinData((char*)"./datasets/00000020.bin",
//   100);

//   int K = 8;
//   int N = 20;

//   Graph* bfGraph =
//       cppdescent::KNNBruteForceGraph(vec, K,
//       cppdescent::compareEdgesEuclidean);
//   Graph* nnGraph = cppdescent::NNDescent_KNNGraph(
//       vec, K, delta, rho, cppdescent::euclideanDistance);

//   ASSERT_GE(cppdescent::recall(bfGraph, nnGraph, N, K), 87.5);

//   delete bfGraph;
//   delete nnGraph;

//   cppdescent::deleteDatapointVectors(vec);
// }