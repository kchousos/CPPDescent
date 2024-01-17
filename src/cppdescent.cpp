/**
 * @file cppdescent.cpp
 * @author Konstantinos Chousos
 * @brief Implementation of the cppdescent library.
 * @version 0.1
 * @date 2023-10-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "cppdescent/cppdescent.hpp"
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <omp.h>
#include <cmath>
#include <cstdint>
#include <iostream>

bool verbose = true;

//===================================
// Helper functions.
//===================================

int cppdescent::compareGraphVertices(Pointer vertex1, Pointer vertex2) {
  GraphVertex* gvertex1 = (GraphVertex*)vertex1;
  GraphVertex* gvertex2 = (GraphVertex*)vertex2;

  if (gsl_vector_equal((gsl_vector*)gvertex1->getData(),
                       (gsl_vector*)gvertex2->getData()))
    return 0;

  return 1;
}

void destroyEdges(GraphVertexPair* pair) {
  delete pair;
}

int cppdescent::compareNeighbors(Pointer neighbor1, Pointer neighbor2) {
  GraphVertexPair* pair1 = (GraphVertexPair*)neighbor1;
  GraphVertexPair* pair2 = (GraphVertexPair*)neighbor2;

  float first = euclideanDistance(pair1->getVertex1(), pair1->getVertex2());
  float second = euclideanDistance(pair2->getVertex1(), pair2->getVertex2());

  float result = first - second;

  if (result < 0)
    return -1;
  else if (result > 0)
    return 1;
  else
    return 0;
}

int cppdescent::compareGraphVertexPairs(Pointer p1, Pointer p2) {
  GraphVertexPair* pair1 = (GraphVertexPair*)p1;
  GraphVertexPair* pair2 = (GraphVertexPair*)p2;

  if (!gsl_vector_equal(
          (gsl_vector*)((GraphVertex*)pair1->getVertex1())->getData(),
          (gsl_vector*)((GraphVertex*)pair2->getVertex1())->getData()) ||
      !gsl_vector_equal(
          (gsl_vector*)((GraphVertex*)pair1->getVertex2())->getData(),
          (gsl_vector*)((GraphVertex*)pair2->getVertex2())->getData()))
    return 1;

  return 0;
}

//===================================
// CPPDescent functions.
//===================================

Vector* cppdescent::readBinData(const char* fp, int dimensions) {
  FILE* data = fopen(fp, "rb");

  uint32_t N;

  fread(&N, sizeof(uint32_t), 1, data);

  Vector* elements = new Vector(N, nullptr);

  float value;

  for (int i = 0; i < (int)N; i++) {
    gsl_vector* datapoints = gsl_vector_alloc(dimensions);

    for (int j = 0; j < dimensions; j++) {
      fread(&value, sizeof(float), 1, data);
      gsl_vector_set(datapoints, j, value);
    }

    elements->setAt(i, datapoints);
  }

  fclose(data);

  return elements;
}

void cppdescent::writeBinGraph(const char* fp, Graph* graph, int K) {
  FILE* file = fopen(fp, "w+");

  Vector* vec = graph->getVec();
  uint32_t N = vec->getSize();

  // number of vertices
  fwrite(&N, sizeof(N), 1, file);

  // number of neighbors
  fwrite(&K, sizeof(K), 1, file);

  // vertices
  for (int i = 0; i < (int)N; i++) {
    GraphVertex* gvertex = (GraphVertex*)vec->getAt(i);
    gsl_vector* vertex = (gsl_vector*)gvertex->getData();
    int dimensions = vertex->size;

    for (int j = 0; j < dimensions; j++) {
      float datapoint = gsl_vector_get(vertex, j);
      fwrite(&datapoint, sizeof(float), 1, file);
    }
  }

  // edges
  for (int i = 0; i < (int)N; i++) {
    GraphVertex* gvertex = (GraphVertex*)vec->getAt(i);
    // neighbors
    Vector* neighbors = gvertex->getNeighbors()->toVector();
    for (int k = 0; k < K; k++) {
      // get the kth neigbor from the vector
      Pointer neighbor =
          (((GraphVertexPair*)neighbors->getAt(k)))->getVertex2();
      // find its position in the graph's vector
      int pos = vec->findPos(neighbor, compareGraphVertices);
      fwrite(&pos, sizeof(int), 1, file);
    }
  }

  fclose(file);
}

Graph* cppdescent::readBinGraph(const char* fp, int dimensions) {
  FILE* file = fopen(fp, "r");
  if (file == nullptr)
    return nullptr;  // LCOV_EXCL_LINE

  Graph* graph = new Graph(nullptr, nullptr);

  uint32_t N;
  int K;

  fread(&N, sizeof(N), 1, file);
  fread(&K, sizeof(K), 1, file);

  float datapoint;
  // read the vertices
  for (int i = 0; i < (int)N; i++) {
    gsl_vector* vertex = gsl_vector_alloc(dimensions);

    for (int j = 0; j < dimensions; j++) {
      fread(&datapoint, sizeof(float), 1, file);
      gsl_vector_set(vertex, j, datapoint);
    }

    graph->insertVertex(vertex);
  }

  for (int i = 0; i < (int)N; i++) {
    GraphVertex* v1 = (GraphVertex*)graph->getVec()->getAt(i);
    int pos;
    for (int k = 0; k < K; k++) {
      fread(&pos, sizeof(int), 1, file);
      GraphVertex* v2 = (GraphVertex*)graph->getVec()->getAt(pos);
      graph->insertEdge(v1, v2);
    }
  }

  fclose(file);
  return graph;
}

float cppdescent::recall(Graph* bfGraph, Graph* nnGraph, int N, int K) {
  Vector* bfVertices = bfGraph->getVerticesV();
  Vector* nnVertices = nnGraph->getVerticesV();

  float recall = 0;

  for (int node = 0; node < bfVertices->getSize(); node++) {
    int trueNeighbors = 0;
    Vector* bfNodeAdjacent = bfGraph->getAdjacentV(bfVertices->getAt(node));

    Vector* nnAdjacent = nnGraph->getAdjacentV(nnVertices->getAt(node));

    for (int adjacent = 0; adjacent < nnAdjacent->getSize(); adjacent++)
      if (bfNodeAdjacent->find(nnAdjacent->getAt(adjacent),
                               cppdescent::compareGraphVertexPairs) != nullptr)
        trueNeighbors++;

    recall += (float)trueNeighbors / (float)K;
  }

  recall = recall / (float)N;
  recall *= 100;

  return recall;
}

Graph* cppdescent::KNNBruteForceGraph(Vector* data,
                                      int K,
                                      CompareFunc compare) {
  Graph* graph = new Graph(nullptr, nullptr);

  // Insert all points as vertices.
  int N = data->getSize();
  for (int i = 0; i < N; i++)
    graph->insertVertex(data->getAt(i));

  for (int i = 0; i < N; i++) {
    Pointer a = graph->getVec()->getAt(i);
    PQueue* neighbors = new PQueue(compare, nullptr, nullptr);

    for (int j = 0; j < N; j++) {
      if (i == j)
        continue;

      Pointer b = graph->getVec()->getAt(j);
      GraphVertexPair* pair = new GraphVertexPair(graph, a, b);

      if (neighbors->getSize() < K) {
        neighbors->insert(pair);
      } else if (compare(pair, neighbors->getMax()) < 0) {
        GraphVertexPair* max = (GraphVertexPair*)neighbors->getMax();
        neighbors->removeMax();
        delete max;
        neighbors->insert(pair);
      } else {
        destroyEdges(pair);
      }
    }

    for (int k = 0; k < K; k++) {
      GraphVertexPair* neighbor = (GraphVertexPair*)neighbors->getMax();
      neighbors->removeMax();
      Pointer vec = ((GraphVertexPair*)neighbor)->getVertex2();
      delete neighbor;
      graph->insertEdge(a, vec);
    }

    delete neighbors;
  }

  return graph;
}

/**
 * @brief Creates a random graph, where each vertex has K random neighbors.
 *
 * The user is responsible for deallocating the graph.
 *
 * @param data The Vector with all the points.
 * @param K
 * @param compare
 * @param distance
 * @return Graph* The created graph.
 */
Graph* sampleGraph(Vector* data, int K) {
  Graph* graph = new Graph(nullptr, nullptr);

  int N = data->getSize();

  srand(time(0));

  // Insert all points as vertices.
  for (int i = 0; i < N; i++)
    graph->insertVertex(data->getAt(i));

  // Iterate all of the vertices.
  for (int i = 0; i < N; i++) {
    // For each vertex, add K random neighbors.
    Pointer v1 = graph->getVec()->getAt(i);
    for (int j = 0; j < K; j++) {
      int randPos = rand() % N;  // The position of the neighbor.

      // Avoid adding itself as a neighbor.
      while (randPos == i)
        randPos = rand() % N;

      // Get the two vertices and create an edge between them.
      Pointer v2 = graph->getVec()->getAt(randPos);
      while (graph->isNeighborVertex(v1, v2) == true) {
        randPos = rand() % N;
        v2 = (Pointer)graph->getVec()->getAt(randPos);
      }
      graph->insertEdge(v1, v2);
    }
  }

  return graph;
}

int updateNN(Graph* graph,
             Pointer u1,
             Pointer u2,
             float dist,
             DistanceFunc distance) {
  PQueue* direct = ((GraphVertex*)u1)->getNeighbors();
  Pointer max = ((GraphVertexPair*)direct->getMax())->getVertex2();
  float maxDist = distance(u1, max);

  if (dist < maxDist) {
    graph->removeEdge(u1, max);
    graph->insertEdge(u1, u2);
    return 1;
  }

  return 0;
}

struct sets {
  Vector* new_v;
  Vector* old_v;
};

/**
 * @brief Get the Sets object
 *
 * Returns a `sets` struct containing a vector pointer to the new[v] set and
 * another to the old[v] set.
 *
 * The first contains rho*K of direct neighbors with
 * their flag equal to true and rho*K reverse neighbors with true. In other
 * words, it contains 2*rho*K neighbors with flag = true.
 *
 * The second contains all of the direct neighbors with flag = false, which in
 * the worst case will be K, and rho*K of the reverse neighbors with flag =
 * false. In other words, K + rho*K neighbors with flag = false.
 *
 * @param neighbors
 * @param K
 * @param rho
 * @return struct sets
 */
struct sets getSets(Vector* neighbors, int K, float rho) {
  int** trueMetadata = new int*[neighbors->getSize()];
  for (int i = 0; i < neighbors->getSize(); ++i)
    trueMetadata[i] = new int[3];

  int** reverseFalseMetadata = new int*[neighbors->getSize() - K];
  for (int i = 0; i < neighbors->getSize() - K; ++i)
    reverseFalseMetadata[i] = new int[2];

  for (int i = 0; i < neighbors->getSize(); i++) {
    // has been added?
    trueMetadata[i][1] = 0;
    // is it a direct neighbor?
    trueMetadata[i][2] = 0;
  }

  for (int i = 0; i < neighbors->getSize() - K; i++)
    // has been added?
    reverseFalseMetadata[i][1] = 0;

  struct sets sets;
  // rhoK of direct true and rhoK of reverse true
  sets.new_v = new Vector(0, nullptr);
  // K of direct false and rhoK of reverse false
  sets.old_v = new Vector(0, nullptr);

  int trues = 0;
  int falses = 0;

  for (int i = 0; i < neighbors->getSize(); i++) {
    GraphVertexPair* pair = (GraphVertexPair*)neighbors->getAt(i);

    // neighbor has flag = true
    if (pair->getFlag()) {
      // in the `trues` array add this neighbor's position
      trueMetadata[trues][0] = i;
      pair->setFalse();
      if (i < K)
        trueMetadata[trues][2] = 1;
      trues++;
    } else if (i < K) {
      // neighbor has flag = false and is direct
      Pointer v = pair->getVertex2();
      sets.old_v->insertLast(v);
    } else {
      // neighbor has flag = false and is reverse, so needs sampling
      reverseFalseMetadata[falses][0] = i;
      falses++;
    }
  }

  if (trues < 2 * rho * K) {
    // if there are less trues than 2ρK, simply put them all
    for (int i = 0; i < trues; i++) {
      Pointer v;
      if (trueMetadata[i][2])
        v = ((GraphVertexPair*)neighbors->getAt(i))->getVertex2();
      else
        v = ((GraphVertexPair*)neighbors->getAt(i))->getVertex1();
      sets.new_v->insertLast(v);
    }
  } else {
    // new[v] sampling
    for (int i = 0; i < 2 * rho * K; i++) {
      int selected = rand() % trues;

      // if it already has been selected, choose another
      while (trueMetadata[selected][1])
        selected = rand() % trues;

      // has now been selected, do not select again
      trueMetadata[selected][1] = 1;

      Pointer v;
      if (trueMetadata[selected][2])
        v = ((GraphVertexPair*)neighbors->getAt(trueMetadata[selected][0]))
                ->getVertex2();
      else
        v = ((GraphVertexPair*)neighbors->getAt(trueMetadata[selected][0]))
                ->getVertex1();

      sets.new_v->insertLast(v);
    }
  }

  if (falses < rho * K) {
    for (int i = 0; i < falses; i++) {
      Pointer v = ((GraphVertexPair*)neighbors->getAt(i))->getVertex1();
      sets.old_v->insertLast(v);
    }
  } else {
    // old[v] sampling
    for (int i = 0; i < rho * K; i++) {
      int selected = rand() % falses;

      // if it already has been selected, choose another
      while (reverseFalseMetadata[selected][1])
        selected = rand() % falses;

      // has now been selected, do not select again
      reverseFalseMetadata[selected][1] = 1;

      Pointer v = ((GraphVertexPair*)neighbors->getAt(
                       reverseFalseMetadata[selected][0]))
                      ->getVertex1();
      sets.old_v->insertLast(v);
    }
  }

  for (int i = 0; i < neighbors->getSize(); ++i)
    delete[] trueMetadata[i];
  delete[] trueMetadata;

  for (int i = 0; i < neighbors->getSize() - K; ++i)
    delete[] reverseFalseMetadata[i];
  delete[] reverseFalseMetadata;

  return sets;
}

Graph* cppdescent::NNDescent_KNNGraph(Vector* data,
                                      int K,
                                      float delta,
                                      float rho,
                                      DistanceFunc distance) {
  if (verbose)
    std::cout << "\tInitializing starting graph...\n";
  // B[v] <- Sample(V, K) for all v in V
  Graph* graph = sampleGraph(data, K);
  if (verbose)
    std::cout << "\tStarting graph has been created\n";
  // The vertices do not change, only the edges between them are modified. So we
  // only need to get them once and not in each iteration.
  Vector* vertices = graph->getVerticesV();
  int N = graph->getSize();
  int c;
  int iterations = 0;
  float dist;

  struct sets* allSets = new struct sets[N];

  do {
    iterations++;

    c = 0;

#pragma omp parallel for
    for (int v = 0; v < N; v++) {
      // vAll = Bbar[v] = B[v] ⋃ R[v]
      Vector* vAll = graph->getGeneralNeighborsV(vertices->getAt(v));

      allSets[v] = getSets(vAll, K, rho);

      delete vAll;
    }

    for (int v = 0; v < N; v++) {
      struct sets sets = allSets[v];

      Vector* new_v = sets.new_v;
      Vector* old_v = sets.old_v;

      for (int U1 = 0; U1 < new_v->getSize(); U1++) {
        for (int U2 = U1 + 1; U2 < new_v->getSize(); U2++) {
          GraphVertex* u1 = (GraphVertex*)new_v->getAt(U1);
          GraphVertex* u2 = (GraphVertex*)new_v->getAt(U2);

          dist = distance(u1, u2);

          if (graph->isNeighborVertex(u1, u2) == false)
            c += updateNN(graph, u1, u2, dist, distance);

          if (graph->isNeighborVertex(u2, u1) == false)
            c += updateNN(graph, u2, u1, dist, distance);
        }

        for (int U2 = 0; U2 < old_v->getSize(); U2++) {
          GraphVertex* u1 = (GraphVertex*)new_v->getAt(U1);
          GraphVertex* u2 = (GraphVertex*)old_v->getAt(U2);

          dist = distance(u1, u2);

          if (graph->isNeighborVertex(u1, u2) == false)
            c += updateNN(graph, u1, u2, dist, distance);

          if (graph->isNeighborVertex(u2, u1) == false)
            c += updateNN(graph, u2, u1, dist, distance);
        }
      }

      delete new_v;
      delete old_v;
    }

    if (verbose)
      std::cout << "\tNumber of changes in the graph (c) = " << c << "\n";
  } while (c >= delta * N * K);

  delete[] allSets;

  if (verbose)
    std::cout << "\tNN-Descent iterations: " << iterations << "\n";

  return graph;
}

// LCOV_EXCL_START
// PQueue* cppdescent::NNDescent_Query(Graph* graph,
//                                     int K,
//                                     CompareFunc compare,
//                                     Vector* query) {
//   List* vertices = graph->getVertices();

//   srand(time(0));

//   int dimensions =
//       ((Vector*)((GraphVertex*)((Vector*)graph->getVec()->first()->getValue()))
//            ->getData())
//           ->getSize();
//   if (query->getSize() != dimensions)
//     return nullptr;

//   // get random candidate from graph
//   int pos = rand() % vertices->getSize();
//   Pointer candidate = ((GraphVertex*)graph->getVec()->getAt(pos))->getData();

//   PQueue* knn = new PQueue(compare, (DestroyFunc)destroyEdges, nullptr);

//   List* candidates;
//   bool candidatesRemain = true;

//   GraphVertex* queryVertex = new GraphVertex(query, graph);

//   while (candidatesRemain) {
//     candidatesRemain = false;

//     // get candidate's neighbors
//     candidates = graph->getGeneralNeighborsVertices(candidate);

//     // add best candidate's neighbors to the queue
//     for (ListNode* node = candidates->getHead(); node != nullptr;
//          node = node->getNext())
//       if (!((GraphVertex*)node->getValue())->checked()) {
//         ((GraphVertex*)node->getValue())->check();

//         candidatesRemain = true;

//         GraphVertexPair* pair =
//             new GraphVertexPair(graph, queryVertex, node->getValue());

//         knn->insert(pair);
//       }

//     // truncuate queue to K
//     while (knn->getSize() > K)
//       knn->removeMax();

//     // get new best candidate
//     candidate =
//     ((GraphVertex*)((GraphVertexPair*)knn->getMin())->getVertex2())
//                     ->getData();

//     delete candidates;
//   }

//   delete vertices;
//   delete queryVertex;
//   return knn;
// }
// LCOV_EXCL_STOP

// ============================ Metric Functions =============================

float cppdescent::euclideanDistance(Pointer a, Pointer b) {
  GraphVertex* first = (GraphVertex*)a;
  GraphVertex* second = (GraphVertex*)b;
  float result = 0;

  double x2 = first->getNorm();
  double y2 = second->getNorm();

  double xy;
  gsl_blas_ddot((gsl_vector*)first->getData(), (gsl_vector*)second->getData(),
                &xy);

  result = x2 + y2 - 2 * xy;

  return result;
}

int cppdescent::compareEdgesEuclidean(Pointer first, Pointer second) {
  GraphVertexPair* pair1 = (GraphVertexPair*)first;
  GraphVertexPair* pair2 = (GraphVertexPair*)second;

  float a = euclideanDistance(pair1->getVertex1(), pair1->getVertex2());
  float b = euclideanDistance(pair2->getVertex1(), pair2->getVertex2());

  int value = 0;
  if (b > a) {
    value = -1;
  } else if (a > b) {
    value = 1;
  }
  return value;
}