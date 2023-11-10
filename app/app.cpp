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

int main(void) {
  Vector* vec = cppdescent::readBinData((char*)"./datasets/00000200.bin", 100);

  int K[] = {10, 20, 30};

  for (int k = 0; k < 3; k++) {
    std::cout << "For K = " << K[k] << "\n";
    std::cout << "----------------------------------------------------------\n";

    // Creation of graphs.

    auto start = std::chrono::high_resolution_clock::now();

    Graph* bfGraph = cppdescent::KNNBruteForceGraph(
        vec, K[k], (CompareFunc)compareEdgesEuclidean, euclideanDistance);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "Brute force K-NN Graph created in " << duration.count()
              << " milliseconds\n";

    start = std::chrono::high_resolution_clock::now();

    Graph* nnGraph =
        cppdescent::NNDescent_KNNGraph(vec, K[k], euclideanDistance);

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

      recall += (float)trueNeighbors / (float)K[k];
      delete bfNodeAdjacent;
    }

    recall = recall / 200.0;

    std::cout << "Total recall is " << recall * 100 << "%\n\n\n";

    delete vertices;
    delete bfGraph;
    delete nnGraph;
  }

  cppdescent::deleteDatapointVectors(vec);
}