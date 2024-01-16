#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include "cppdescent/cppdescent.hpp"

int main(int argc, char* argv[]) {
  if (argc != 6) {
    std::cout << "Wrong number of arguments. Please try again.\n";
    return -1;
  }

  int K = atoi(argv[1]);
  int dimensions = atoi(argv[3]);
  float delta = atof(argv[4]);
  float rho = atof(argv[5]);

  if (rho <= 0 || rho > 1) {
    std::cout << "rho must be in (0,1]. Please try again.\n";
    return -1;
  }

  Vector* vec = cppdescent::readBinData((char*)argv[2], dimensions);
  int N = vec->getSize();

  DistanceFunc distance = nullptr;
  CompareFunc compare = nullptr;

  distance = cppdescent::euclideanDistance;
  compare = cppdescent::compareEdgesEuclidean;

  // find filepath to computed brute force graph
  std::string fullFilePath = argv[2];
  size_t lastSeparator = fullFilePath.find_last_of("/");
  std::string filenameWithExtension = fullFilePath.substr(lastSeparator + 1);
  size_t lastDot = filenameWithExtension.find_last_of(".");
  std::string filenameWithoutExtension =
      filenameWithExtension.substr(0, lastDot);
  std::string bfPath("./datasets/computed/");
  bfPath.append(filenameWithoutExtension);
  bfPath.append("_K-");
  bfPath.append(std::to_string(K));
  bfPath.append("_euclidean");
  bfPath.append(".bin");

  std::cout << "For K = " << K << "\n";
  std::cout << "For δ = " << delta << "\n";
  std::cout << "For ρ = " << rho << "\n";
  std::cout << "Dataset: " << argv[2] << "\n";
  std::cout << "Dimensions: " << dimensions << "\n";
  std::cout << "----------------------------------------------------------\n";

  Graph* nnGraph;
  Graph* bfGraph;

  auto start = std::chrono::high_resolution_clock::now();

  // NN-Descent
  nnGraph = cppdescent::NNDescent_KNNGraph(vec, K, delta, rho, distance);

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << "NN-Descent K-NN Graph created in " << duration.count()
            << " milliseconds for δ = " << delta << ", ρ = " << rho << "\n";

  std::cout << "Computing recall...\n";

  // Read the graph files.
  bfGraph = cppdescent::readBinGraph(bfPath.c_str(), dimensions);

  bool computed = false;
  Vector* vec2;

  if (bfGraph == nullptr) {
    std::cout << "\tNo pre-computed brute force graph. Computing now...\n";
    vec2 = cppdescent::readBinData((char*)argv[2], dimensions);
    bfGraph = cppdescent::KNNBruteForceGraph(vec2, K, compare);
    std::cout << "\tSaving...\n";
    cppdescent::writeBinGraph(bfPath.c_str(), bfGraph, K);
    computed = true;
  }

  std::cout << "Total recall is " << cppdescent::recall(bfGraph, nnGraph, N, K)
            << "%\n\n";

  delete bfGraph;
  delete nnGraph;

  /*   // Query point

    std::cout << "Query point example\n";
    std::cout << "----------------------------------------------------------\n";

    nnGraph = cppdescent::NNDescent_KNNGraph(vec, K, 0.01, distance);
    srand(time(0));

    Vector* query = new Vector(dimensions, cppdescent::deleteFloat);

    for (int i = 0; i < dimensions; i++) {
      float num = (float)(rand()) / (float)(rand());
      query->setAt(i, cppdescent::createFloat(num));
    }

    PQueue* results = cppdescent::NNDescent_Query(
        nnGraph, K, cppdescent::compareVertexPairsEuclidean, query);

    int size = results->getSize();
    for (int i = 0; i < size; i++) {
      GraphVertexPair* max = (GraphVertexPair*)results->getMax();
      results->removeMax();
      Vector* neighbor = (Vector*)((GraphVertex*)max->getVertex2())->getData();

      std::cout << "Neighbor " << i + 1 << " : [";

      int dimensions = neighbor->getSize() > 8 ? 8 : neighbor->getSize();

      for (int i = 0; i < dimensions; i++) {
        std::cout << std::fixed;
        std::cout << *(float*)neighbor->getAt(i) << ", ";
      }

      std::cout << "...]\n";
    } */

  // delete results;
  // delete query;
  // delete nnGraph;

  if (computed)
    delete vec2;

  delete vec;
}