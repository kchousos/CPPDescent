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

  DistanceFunc distance;

  if (metric == 1)
    distance = cppdescent::euclideanDistance;
  else if (metric == 2)
    distance = cppdescent::manhattanDistance;

  std::string bfPath("./datasets/computed/");
  bfPath.append("N-");
  bfPath.append(std::to_string(N));
  bfPath.append("_K-");
  bfPath.append(std::to_string(K));
  bfPath.append("_euclidean");
  bfPath.append(".bin");

  std::cout << "For K = " << K << "\n";
  std::cout << "Dataset: " << argv[2] << "\n";
  std::cout << "Dimensions: " << dimensions << "\n";
  std::cout << "----------------------------------------------------------\n";

  auto start = std::chrono::high_resolution_clock::now();

  // NN-Descent
  Graph* nnGraph = cppdescent::NNDescent_KNNGraph(vec, K, distance);

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << "NN-Descent K-NN Graph created in " << duration.count()
            << " milliseconds\n";

  // Read the graph files.
  Graph* bfGraph =
      cppdescent::readBinGraph(bfPath.c_str(), dimensions, distance);

  std::cout << "Total recall is " << cppdescent::recall(bfGraph, nnGraph, N, K)
            << "%\n\n";

  delete bfGraph;
  delete nnGraph;

  cppdescent::deleteDatapointVectors(vec);
}