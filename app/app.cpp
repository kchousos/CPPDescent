#include <chrono>
#include <cmath>
#include <iostream>
#include "cppdescent/cppdescent.hpp"

/**
 * @brief Returns the Euclidean distance between two points of arbitrary
 * dimension.
 *
 * @param first A pointer to the first point.
 * @param second A pointer to the second point.
 * @return long double The Euclidean distance.
 */
float euclideanDistance(Pointer a, Pointer b) {
  Vector* first = (Vector*)a;
  Vector* second = (Vector*)b;
  float result = 0;

  if (first->getSize() != second->getSize())
    return -1.0;

  for (int i = 0; i < first->getSize(); i++) {
    float diff = *(float*)first->getAt(i) - *(float*)second->getAt(i);
    result += diff * diff;
  }

  result = sqrtf(result);
  return result;
}

float manhattanDistance(Pointer a, Pointer b) {
  Vector* first = (Vector*)a;
  Vector* second = (Vector*)b;
  float result = 0;

  if (first->getSize() != second->getSize())
    return -1.0;

  for (int i = 0; i < first->getSize(); i++) {
    float diff = *(float*)first->getAt(i) - *(float*)second->getAt(i);
    result += fabs(diff);
  }

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

  int value = 0;
  if (b > a) {
    value = -1;
  } else if (a > b) {
    value = 1;
  }
  return value;
}

int compareEdgesManhattan(Pointer first, Pointer second) {
  GraphVertexPair* pair1 = (GraphVertexPair*)first;
  GraphVertexPair* pair2 = (GraphVertexPair*)second;

  float a = manhattanDistance((Vector*)pair1->getVertex1(),
                              (Vector*)pair1->getVertex2());
  float b = manhattanDistance((Vector*)pair2->getVertex1(),
                              (Vector*)pair2->getVertex2());

  int value = 0;
  if (b > a) {
    value = -1;
  } else if (a > b) {
    value = 1;
  }
  return value;
}

int main(int argc, char* argv[]) {
  if (argc != 4)
    return -1;

  int K = atoi(argv[1]);
  int dimensions = atoi(argv[3]);

  Vector* vec = cppdescent::readBinData((char*)argv[2], dimensions);

  std::cout << "For K = " << K << "\n";
  std::cout << "Dataset: " << argv[2] << "\n";
  std::cout << "Dimensions: " << dimensions << "\n";
  std::cout << "----------------------------------------------------------\n";

  // Creation of graphs.

  auto start = std::chrono::high_resolution_clock::now();

  Graph* bfGraph = cppdescent::KNNBruteForceGraph(
      vec, K, (CompareFunc)compareEdgesManhattan, manhattanDistance);

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << "Brute force K-NN Graph created in " << duration.count()
            << " milliseconds\n";

  start = std::chrono::high_resolution_clock::now();

  Graph* nnGraph = cppdescent::NNDescent_KNNGraph(vec, K, manhattanDistance);

  stop = std::chrono::high_resolution_clock::now();
  duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << "NN-Descent K-NN Graph created in " << duration.count()
            << " milliseconds\n";

  // Graphs have been created.

  // The vertices will be the same for both graphs.
  List* vertices = bfGraph->getVertices();

  float recall = 0;

  for (ListNode* bfNode = vertices->getHead(); bfNode != nullptr;
       bfNode = bfNode->getNext()) {
    int trueNeighbors = 0;
    List* bfNodeAdjacent = bfGraph->getAdjacent(bfNode->getValue());

    for (ListNode* adjacent = bfNodeAdjacent->getHead(); adjacent != nullptr;
         adjacent = adjacent->getNext()) {
      List* nnAdjacent = nnGraph->getAdjacent(bfNode->getValue());
      if (nnAdjacent->find(adjacent->getValue(), cppdescent::compareVertices))
        trueNeighbors++;

      delete nnAdjacent;
    }

    recall += (float)trueNeighbors / (float)K;
    delete bfNodeAdjacent;
  }

  recall = recall / 20.0;

  std::cout << "Total recall is " << recall * 100 << "%\n\n\n";

  delete vertices;
  delete bfGraph;
  delete nnGraph;

  cppdescent::deleteDatapointVectors(vec);
}