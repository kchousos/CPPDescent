#include <chrono>
#include <iostream>
#include "cppdescent/cppdescent.hpp"

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cout << "Wrong number of arguments. Please try again.\n";
    return -1;
  }

  int K = atoi(argv[1]);
  int dimensions = atoi(argv[3]);
  int metric = atoi(argv[4]);

  if (metric > 2 || metric < 1)
    return -1;

  Vector* vec = cppdescent::readBinData((char*)argv[2], dimensions);
  int N = vec->getSize();

  DistanceFunc distance = nullptr;
  CompareFunc compare = nullptr;

  if (metric == 1) {
    distance = cppdescent::euclideanDistance;
    compare = cppdescent::compareEdgesEuclidean;
  } else if (metric == 2) {
    distance = cppdescent::manhattanDistance;
    compare = cppdescent::compareEdgesManhattan;
  }

  // find filepath to computed brute force graph
  std::string bfPath("./datasets/computed/");
  bfPath.append("N-");
  bfPath.append(std::to_string(N));
  bfPath.append("_K-");
  bfPath.append(std::to_string(K));
  if (metric == 1)
    bfPath.append("_euclidean");
  else
    bfPath.append("_manhattan");
  bfPath.append(".bin");

  std::cout << "For K = " << K << "\n";
  std::cout << "Dataset: " << argv[2] << "\n";
  std::cout << "Dimensions: " << dimensions << "\n";
  std::cout << "----------------------------------------------------------\n";

  Graph* nnGraph;
  Graph* bfGraph;

  for (float delta = 0.001; delta < 0.3; delta *= 10) {
    auto start = std::chrono::high_resolution_clock::now();

    // NN-Descent
    nnGraph = cppdescent::NNDescent_KNNGraph(vec, K, delta, distance);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "NN-Descent K-NN Graph created in " << duration.count()
              << " milliseconds for δ = " << delta << "\n";

    // Read the graph files.
    bfGraph = cppdescent::readBinGraph(bfPath.c_str(), dimensions, distance);

    if (bfGraph == nullptr) {
      std::cout << "No pre-computed brute force graph. Computing now...\n";
      bfGraph = cppdescent::KNNBruteForceGraph(vec, K, compare, distance);
    }

    std::cout << "Total recall is "
              << cppdescent::recall(bfGraph, nnGraph, N, K) << "%\n\n";
  }

  delete bfGraph;
  delete nnGraph;

  cppdescent::deleteDatapointVectors(vec);
}