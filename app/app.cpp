#include <chrono>
#include <iostream>
#include "cppdescent/cppdescent.hpp"

int main(int argc, char* argv[]) {
  if (argc != 5)
    return -1;

  int K = atoi(argv[1]);
  int dimensions = atoi(argv[3]);
  int metric = atoi(argv[4]);

  if (metric > 2 || metric < 1)
    return -1;

  Vector* vec = cppdescent::readBinData((char*)argv[2], dimensions);
  int N = vec->getSize();

  DistanceFunc distance;
  CompareFunc compare;

  if (metric == 1) {
    distance = cppdescent::euclideanDistance;
    compare = cppdescent::compareEdgesEuclidean;
  } else if (metric == 2) {
    distance = cppdescent::manhattanDistance;
    compare = cppdescent::compareEdgesManhattan;
  }

  std::cout << "For K = " << K << "\n";
  std::cout << "Dataset: " << argv[2] << "\n";
  std::cout << "Dimensions: " << dimensions << "\n";
  std::cout << "----------------------------------------------------------\n";

  // Creation of graphs //

  auto start = std::chrono::high_resolution_clock::now();

  // Brute force
  Graph* bfGraph = cppdescent::KNNBruteForceGraph(vec, K, compare, distance);

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << "Brute force K-NN Graph created in " << duration.count()
            << " milliseconds\n";

  start = std::chrono::high_resolution_clock::now();

  // NN-Descent
  Graph* nnGraph = cppdescent::NNDescent_KNNGraph(vec, K, distance);

  stop = std::chrono::high_resolution_clock::now();
  duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << "NN-Descent K-NN Graph created in " << duration.count()
            << " milliseconds\n";

  // Graphs have been created.

  // Save brute force graph to file.
  cppdescent::writeBinGraph("./build/cache/bfgraph.bin", bfGraph);
  delete bfGraph;

  // save NN-Descent graph to file.
  cppdescent::writeBinGraph("./build/cache/nngraph.bin", nnGraph);
  delete nnGraph;

  // Read the graph files.
  bfGraph = cppdescent::readBinGraph("./build/cache/bfgraph.bin", dimensions,
                                     distance);
  nnGraph = cppdescent::readBinGraph("./build/cache/nngraph.bin", dimensions,
                                     distance);

  std::cout << "Total recall is " << cppdescent::recall(bfGraph, nnGraph, N, K)
            << "%\n\n";

  delete bfGraph;
  delete nnGraph;

  cppdescent::deleteDatapointVectors(vec);
}