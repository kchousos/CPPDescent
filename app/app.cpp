#include <unistd.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include "cppdescent/cppdescent.hpp"

#define dimensions 100

int main(int argc, char* argv[]) {
  int K = -1;
  int D = 0;
  int T = 4;
  float delta = 0.01;
  float rho = 0.5;
  char* path = nullptr;

  int opt;

  while ((opt = getopt(argc, argv, ":d:r:K:qD:T:")) != -1) {
    switch (opt) {
      case 'd':
        delta = atof(optarg);
        break;
      case 'r':
        rho = atof(optarg);
        break;
      case 'K':
        K = atoi(optarg);
        break;
      case 'T':
        T = atoi(optarg);
        break;
      case 'D':
        D = atoi(optarg);
        break;
      case 'q':
        verbose = false;
        break;
      case ':':
        printf("option %c needs a value. Please try again.\n", optopt);
        return -1;
      case '?':
        printf("unknown option: %c. Please try again.\n", optopt);
        return -1;
    }
  }

  path = argv[optind];

  if (rho <= 0 || rho > 1) {
    std::cout << "rho must be in (0,1]. Please try again.\n";
    return -1;
  }

  if (delta <= 0) {
    std::cout << "delta must be greater than 0. Please try again.\n";
    return -1;
  }

  if (D >= K) {
    std::cout << "D must be lesser than K. Please try again.\n";
    return -1;
  }

  if (T < 2) {
    std::cout << "T (RPTrees) must be at least 2. Please try again.\n";
    return -1;
  }

  if (!path) {
    std::cout << "Dataset must be specified. Please try again.\n";
    return -1;
  }

  Vector* vec = cppdescent::readBinData(path, dimensions);
  int N = vec->getSize();

  DistanceFunc distance = cppdescent::euclideanDistance;
  CompareFunc compare = cppdescent::compareEdgesEuclidean;

  // find filepath to computed brute force graph
  std::string fullFilePath = path;
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

  if (verbose) {
    std::cout << "For K = " << K << "\n";
    std::cout << "For δ = " << delta << "\n";
    std::cout << "For ρ = " << rho << "\n";
    if (D != 0)
      std::cout << T << " random projection trees are used, for D = " << D
                << "\n";
    std::cout << "Dataset: " << path << "\n";
    std::cout << "Dimensions: " << dimensions << "\n";
    std::cout << "----------------------------------------------------------\n";
  }

  Graph* nnGraph;
  Graph* bfGraph;

  auto start = std::chrono::high_resolution_clock::now();

  // NN-Descent
  if (verbose)
    std::cout << "NN-Descent\n";

  nnGraph = cppdescent::NNDescent_KNNGraph(vec, K, D, T, delta, rho, distance);

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  if (verbose)
    std::cout << "NN-Descent K-NN Graph created in " << duration.count()
              << " milliseconds for δ = " << delta << ", ρ = " << rho << "\n";

  if (verbose)
    std::cout << "Computing recall...\n";

  // Read the graph files.
  bfGraph = cppdescent::readBinGraph(bfPath.c_str(), dimensions);

  bool computed = false;
  Vector* vec2 = nullptr;

  if (bfGraph == nullptr) {
    if (verbose)
      std::cout << "\tNo pre-computed brute force graph. Computing now...\n";
    vec2 = cppdescent::readBinData(path, dimensions);
    bfGraph = cppdescent::KNNBruteForceGraph(vec2, K, compare);
    if (verbose)
      std::cout << "\tSaving...\n";
    cppdescent::writeBinGraph(bfPath.c_str(), bfGraph, K);
    computed = true;
  }

  float recall = cppdescent::recall(bfGraph, nnGraph, N, K);

  if (verbose)
    std::cout << "Total recall is " << recall << "%\n\n";

  delete bfGraph;
  delete nnGraph;

  if (!verbose)
    std::cout << N << "," << K << "," << delta << "," << rho << ","
              << duration.count() << "," << recall << "\n";

  /*   // Query point

    std::cout << "Query point example\n";
    std::cout <<
    "----------------------------------------------------------\n";

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
      Vector* neighbor =
    (Vector*)((GraphVertex*)max->getVertex2())->getData();

      std::cout << "Neighbor " << i + 1 << " : [";

      int dimensions = neighbor->getSize() > 8 ? 8 :
    neighbor->getSize();

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