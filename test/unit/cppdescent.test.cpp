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
 * @return float The Euclidean distance.
 */
float euclideanDistance(Pointer a, Pointer b) {
  Vector* first = (Vector*)a;
  Vector* second = (Vector*)b;
  float result = 0;

  if (first->getSize() != second->getSize())
    return -1.0;

  for (int i = 0; i < first->getSize(); i++) {
    float diff = *(float*)first->getAt(i) - *(float*)second->getAt(i);
    result += pow(diff, 2);
  }

  result = sqrtf(result);
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

  float a = euclideanDistance((Vector*)pair1->getVertex1(),
                              (Vector*)pair1->getVertex2());
  float b = euclideanDistance((Vector*)pair2->getVertex1(),
                              (Vector*)pair2->getVertex2());

  // b - a and not a - b because we want the PQueue to return the *minimum*.
  return (int)(b - a);
}

// TEST(IO, readData) {
//   Vector* vec = cppdescent::readBinData("./datasets/00000020.bin");
//   ASSERT_NE(vec, nullptr);

//   float lastValue = 0.0726192221;

//   Vector* lastElement = (Vector*)vec->getAt(vec->getSize() - 1);
//   ASSERT_EQ(lastValue, *(float*)lastElement->getAt(lastElement->getSize() -
//   1));

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
    }

    delete vertices;
    delete graph;
  }

  int result = cppdescent::deleteDatapointVectors(vec);
  ASSERT_EQ(result, 0);
}

// TEST(BruteForce, sampleDataset) {
//   Vector* vec = new Vector(5, nullptr);

//   for (int i = 0; i < 5; i++) {
//     Vector* point = new Vector(2, cppdescent::deleteFloat);
//     for (int j = 0; j < 2; j++) {
//       point->setAt(j, )
//     }
//     vec->setAt(i, point);
//   }

//   int K[] = {3, 5, 10};

//   for (int k = 0; k < 3; k++) {
//     Graph* graph = cppdescent::KNNBruteForceGraph(vec, K[k]);

//     List* vertices = graph->getVertices();
//     ASSERT_EQ(vertices->getSize(), 20);

//     ListNode* vertex = vertices->getHead();

//     for (int i = 0; i < vertices->getSize(); i++) {
//       List* adjacent = graph->getAdjacent((Pointer)vertex->getValue());
//       ASSERT_EQ(adjacent->getSize(), K[k]);
//       delete adjacent;
//     }

//     delete vertices;
//     delete graph;
//   }

//   int result = cppdescent::deleteDatapointVectors(vec);
//   ASSERT_EQ(result, 0);
// }