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
#include <cmath>
#include <cstdint>
#include <iostream>

//===================================
// Helper functions.
//===================================

int cppdescent::compareGraphVertices(Pointer vertex1, Pointer vertex2) {
  GraphVertex* gvertex1 = (GraphVertex*)vertex1;
  GraphVertex* gvertex2 = (GraphVertex*)vertex2;

  return cppdescent::compareVertices(gvertex1->getData(), gvertex2->getData());
}

void destroyEdges(GraphVertexPair* pair) {
  delete pair;
}

uint hashEdge(Pointer value) {
  GraphVertexPair* pair = (GraphVertexPair*)value;
  size_t hash = (size_t)pair->getVertex1() + (size_t)pair->getVertex2();
  return hash;
}

int cppdescent::compareNeighbors(Pointer neighbor1, Pointer neighbor2) {
  GraphVertexPair* pair1 = ((Neighbor*)neighbor1)->getPair();
  GraphVertexPair* pair2 = ((Neighbor*)neighbor2)->getPair();

  float first =
      euclideanDistance(((GraphVertex*)pair1->getVertex1())->getData(),
                        ((GraphVertex*)pair1->getVertex2())->getData());
  float second =
      euclideanDistance(((GraphVertex*)pair2->getVertex1())->getData(),
                        ((GraphVertex*)pair2->getVertex2())->getData());

  float result = first - second;

  if (result < 0)
    return -1;
  else if (result > 0)
    return 1;
  else
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
    Vector* datapoints = new Vector(dimensions, deleteFloat);

    for (int j = 0; j < dimensions; j++) {
      fread(&value, sizeof(float), 1, data);
      datapoints->setAt(j, createFloat(value));
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
    Vector* vertex = (Vector*)gvertex->getData();
    int dimensions = vertex->getSize();

    for (int j = 0; j < dimensions; j++)
      fwrite((float*)vertex->getAt(j), sizeof(float), 1, file);
  }

  Map* map = graph->getMap();

  // edges
  for (MapNode* node = map->getFirst(); node != MAP_EOF;
       node = map->getNext(node)) {
    GraphVertexPair* pair = (GraphVertexPair*)node->getKey();

    Pointer vertex1 = pair->getVertex1();
    Pointer vertex2 = pair->getVertex2();

    GraphVertex* gvertex1 = new GraphVertex(vertex1, graph);
    GraphVertex* gvertex2 = new GraphVertex(vertex2, graph);
    int pos1 = vec->findPos(gvertex1, compareGraphVertices);
    int pos2 = vec->findPos(gvertex2, compareGraphVertices);

    delete gvertex1;
    delete gvertex2;

    fwrite(&pos1, sizeof(int), 1, file);
    fwrite(&pos2, sizeof(int), 1, file);
  }

  fclose(file);
}

void deleteVectors(Pointer vec) {
  delete (Vector*)vec;
}

Graph* cppdescent::readBinGraph(const char* fp,
                                int dimensions,
                                DistanceFunc distance) {
  FILE* file = fopen(fp, "r");
  if (file == nullptr)
    return nullptr;  // LCOV_EXCL_LINE

  Graph* graph =
      new Graph((CompareFunc)compareVertices, nullptr, deleteVectors);
  graph->setHashFunction((HashFunc)hashEdge);

  uint32_t N;
  int K;

  fread(&N, sizeof(N), 1, file);
  fread(&K, sizeof(K), 1, file);

  float datapoint;
  // read the vertices
  for (int i = 0; i < (int)N; i++) {
    Vector* vertex = new Vector(dimensions, (DestroyFunc)deleteFloat);

    for (int j = 0; j < dimensions; j++) {
      fread(&datapoint, sizeof(float), 1, file);
      vertex->setAt(j, createFloat(datapoint));
    }

    graph->insertVertex(vertex);
  }

  // read the edges
  int pos1, pos2;

  for (int i = 0; i < (int)N * K; i++) {
    fread(&pos1, sizeof(int), 1, file);
    fread(&pos2, sizeof(int), 1, file);

    GraphVertex* gvertex1 = (GraphVertex*)graph->getVec()->getAt(pos1);
    GraphVertex* gvertex2 = (GraphVertex*)graph->getVec()->getAt(pos2);
    Pointer vertex1 = gvertex1->getData();
    Pointer vertex2 = gvertex2->getData();
    graph->insertEdge(vertex1, vertex2, distance(vertex1, vertex2));
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
                               cppdescent::compareNeighbors) != nullptr)
        trueNeighbors++;

    recall += (float)trueNeighbors / (float)K;
  }

  recall = recall / (float)N;
  recall *= 100;

  return recall;
}

void cppdescent::deleteFloat(Pointer value) {
  delete (float*)value;
}

float* cppdescent::createFloat(float value) {
  float* p = new float;
  *p = value;
  return p;
}

float cppdescent::compareFloats(Pointer a, Pointer b) {
  return (*(float*)a - *(float*)b);
}

int cppdescent::deleteDatapointVectors(Vector* vec) {
  if (vec == nullptr)
    return -1;  // LCOV_EXCL_LINE
  int dimensions = vec->getSize();
  for (int i = 0; i < dimensions; i++) {
    delete (Vector*)vec->getAt(i);
  }

  delete vec;
  return 0;
}

int cppdescent::compareVertices(Pointer first, Pointer second) {
  Vector* vec1 = (Vector*)first;
  Vector* vec2 = (Vector*)second;
  int dimensions = vec1->getSize();
  for (int i = 0; i < dimensions; i++)
    if (cppdescent::compareFloats(vec1->getAt(i), vec2->getAt(i)))
      return 1;

  return 0;
}

Graph* cppdescent::KNNBruteForceGraph(Vector* data,
                                      int K,
                                      CompareFunc compare,
                                      DistanceFunc distance) {
  Graph* graph = new Graph((CompareFunc)compareVertices, nullptr);
  graph->setHashFunction((HashFunc)hashEdge);

  // Insert all points as vertices.
  int N = data->getSize();
  for (int i = 0; i < N; i++)
    graph->insertVertex((Pointer)data->getAt(i));

  for (int i = 0; i < N; i++) {
    Pointer a = (Pointer)data->getAt(i);
    PQueue* neighbors = new PQueue(compare, nullptr, nullptr);

    for (int j = 0; j < N; j++) {
      if (i == j)
        continue;

      Pointer b = (Pointer)data->getAt(j);
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
      graph->insertEdge(a, vec, distance(a, vec));
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
Graph* sampleGraph(Vector* data,
                   int K,
                   CompareFunc compare,
                   DistanceFunc distance) {
  Graph* graph = new Graph((CompareFunc)compare, nullptr);
  graph->setHashFunction((HashFunc)hashEdge);

  int N = data->getSize();

  srand(time(0));

  // Insert all points as vertices.
  for (int i = 0; i < N; i++)
    graph->insertVertex(data->getAt(i));

  // Iterate all of the vertices.
  for (int i = 0; i < N; i++) {
    // For each vertex, add K random neighbors.
    for (int j = 0; j < K; j++) {
      int randPos = rand() % N;  // The position of the neighbor.

      // Avoid adding itself as a neighbor.
      while (randPos == i)
        randPos = rand() % N;

      // Get the two vertices and create an edge between them.
      Pointer v1 = data->getAt(i);
      Pointer v2 = data->getAt(randPos);
      while (graph->isNeighbor(v1, v2) == true) {
        randPos = rand() % N;
        v2 = (Pointer)data->getAt(randPos);
      }
      // float weight = distance(v1, v2);
      graph->insertEdge(v1, v2, 1);
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
  Pointer max = ((GraphVertexPair*)((Neighbor*)direct->getMax())->getPair())
                    ->getVertex2();
  float maxDist =
      distance(((GraphVertex*)u1)->getData(), ((GraphVertex*)max)->getData());

  if (dist < maxDist) {
    graph->removeEdge(((GraphVertex*)u1)->getData(),
                      ((GraphVertex*)max)->getData());
    graph->insertEdge(((GraphVertex*)u1)->getData(),
                      ((GraphVertex*)u2)->getData(), dist);
    return 1;
  }

  return 0;
}

GraphVertex* getOther(Neighbor* neighbor, int direct) {
  GraphVertex* other;

  if (direct)
    other = (GraphVertex*)((GraphVertexPair*)neighbor->getPair())->getVertex2();
  else
    other = (GraphVertex*)((GraphVertexPair*)neighbor->getPair())->getVertex1();

  return other;
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
    Neighbor* neighbor = (Neighbor*)neighbors->getAt(i);

    // neighbor has flag = true
    if (neighbor->getFlag()) {
      // in the `trues` array add this neighbor's position
      trueMetadata[trues][0] = i;
      neighbor->setFalse();
      if (i < K)
        trueMetadata[trues][2] = 1;
      trues++;
    } else if (i < K) {
      // neighbor has flag = false and is direct
      GraphVertex* v = getOther(neighbor, 1);
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
      GraphVertex* v =
          getOther((Neighbor*)neighbors->getAt(i), trueMetadata[i][2]);
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

      GraphVertex* v =
          getOther((Neighbor*)neighbors->getAt(trueMetadata[selected][0]),
                   trueMetadata[selected][2]);

      sets.new_v->insertLast(v);
    }
  }

  if (falses < rho * K) {
    for (int i = 0; i < falses; i++) {
      GraphVertex* v = getOther((Neighbor*)neighbors->getAt(i), 0);
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

      GraphVertex* v = getOther(
          (Neighbor*)neighbors->getAt(reverseFalseMetadata[selected][0]), 0);
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
  // B[v] <- Sample(V, K) for all v in V
  std::cout << "Computing starting graph...\n";
  Graph* graph = sampleGraph(data, K, (CompareFunc)compareVertices, distance);
  std::cout << "Starting graph has been created\n";
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

    for (int v = 0; v < N; v++) {
      Vector* vAll = graph->getGeneralNeighborsV(vertices->getAt(v));

      allSets[v] = getSets(vAll, K, rho);
    }

    for (int v = 0; v < N; v++) {
      // vAll = Bbar[v] = B[v] ⋃ R[v]
      Vector* vAll = graph->getGeneralNeighborsV(vertices->getAt(v));

      struct sets sets = allSets[v];

      Vector* new_v = sets.new_v;
      Vector* old_v = sets.old_v;

      for (int U1 = 0; U1 < new_v->getSize(); U1++) {
        for (int U2 = U1 + 1; U2 < new_v->getSize(); U2++) {
          GraphVertex* u1 = (GraphVertex*)new_v->getAt(U1);
          GraphVertex* u2 = (GraphVertex*)new_v->getAt(U2);

          dist = distance(u1->getData(), u2->getData());

          if (graph->isNeighbor(u1->getData(), u2->getData()) == false)
            c += updateNN(graph, u1, u2, dist, distance);

          if (graph->isNeighbor(u2->getData(), u1->getData()) == false)
            c += updateNN(graph, u2, u1, dist, distance);
        }

        for (int U2 = 0; U2 < old_v->getSize(); U2++) {
          GraphVertex* u1 = (GraphVertex*)new_v->getAt(U1);
          GraphVertex* u2 = (GraphVertex*)old_v->getAt(U2);

          dist = distance(u1->getData(), u2->getData());

          if (graph->isNeighbor(u1->getData(), u2->getData()) == false)
            c += updateNN(graph, u1, u2, dist, distance);

          if (graph->isNeighbor(u2->getData(), u1->getData()) == false)
            c += updateNN(graph, u2, u1, dist, distance);
        }
      }

      delete vAll;
      delete new_v;
      delete old_v;
    }

    std::cout << "Number of changes in the graph (c) = " << c << "\n";
  } while (c >= delta * N * K);

  delete[] allSets;

  std::cout << "NN-Descent iterations: " << iterations << "\n";

  return graph;
}

// LCOV_EXCL_START
PQueue* cppdescent::NNDescent_Query(Graph* graph,
                                    int K,
                                    CompareFunc compare,
                                    Vector* query) {
  List* vertices = graph->getVertices();

  srand(time(0));

  int dimensions =
      ((Vector*)((GraphVertex*)((Vector*)graph->getVec()->first()->getValue()))
           ->getData())
          ->getSize();
  if (query->getSize() != dimensions)
    return nullptr;

  // get random candidate from graph
  int pos = rand() % vertices->getSize();
  Pointer candidate = ((GraphVertex*)graph->getVec()->getAt(pos))->getData();

  PQueue* knn = new PQueue(compare, (DestroyFunc)destroyEdges, nullptr);

  List* candidates;
  bool candidatesRemain = true;

  GraphVertex* queryVertex = new GraphVertex(query, graph);

  while (candidatesRemain) {
    candidatesRemain = false;

    // get candidate's neighbors
    candidates = graph->getGeneralNeighborsVertices(candidate);

    // add best candidate's neighbors to the queue
    for (ListNode* node = candidates->getHead(); node != nullptr;
         node = node->getNext())
      if (!((GraphVertex*)node->getValue())->checked()) {
        ((GraphVertex*)node->getValue())->check();

        candidatesRemain = true;

        GraphVertexPair* pair =
            new GraphVertexPair(graph, queryVertex, node->getValue());

        knn->insert(pair);
      }

    // truncuate queue to K
    while (knn->getSize() > K)
      knn->removeMax();

    // get new best candidate
    candidate = ((GraphVertex*)((GraphVertexPair*)knn->getMin())->getVertex2())
                    ->getData();

    delete candidates;
  }

  delete vertices;
  delete queryVertex;
  return knn;
}
// LCOV_EXCL_STOP

// ============================ Metric Functions =============================

float cppdescent::euclideanDistance(Pointer a, Pointer b) {
  Vector* first = (Vector*)a;
  Vector* second = (Vector*)b;
  float result = 0;

  int dimensions = first->getSize();

  if (second->getSize() != dimensions)
    return -1.0;  // LCOV_EXCL_LINE

  for (int i = 0; i < dimensions; i++) {
    float diff = *(float*)first->getAt(i) - *(float*)second->getAt(i);
    result += diff * diff;
  }

  result = sqrtf(result);
  return result;
}

int cppdescent::compareEdgesEuclidean(Pointer first, Pointer second) {
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

int cppdescent::compareVertexPairsEuclidean(Pointer first, Pointer second) {
  GraphVertexPair* pair1 = (GraphVertexPair*)first;
  GraphVertexPair* pair2 = (GraphVertexPair*)second;

  Vector* vec11 = (Vector*)((GraphVertex*)pair1->getVertex1())->getData();
  Vector* vec12 = (Vector*)((GraphVertex*)pair1->getVertex2())->getData();
  Vector* vec21 = (Vector*)((GraphVertex*)pair2->getVertex1())->getData();
  Vector* vec22 = (Vector*)((GraphVertex*)pair2->getVertex2())->getData();

  float a = euclideanDistance(vec11, vec12);
  float b = euclideanDistance(vec21, vec22);

  int value = 0;
  if (b > a) {
    value = -1;
  } else if (a > b) {
    value = 1;
  }
  return value;
}

float cppdescent::manhattanDistance(Pointer a, Pointer b) {
  Vector* first = (Vector*)a;
  Vector* second = (Vector*)b;
  float result = 0;

  int dimensions = first->getSize();

  if (second->getSize() != dimensions)
    return -1.0;  // LCOV_EXCL_LINE

  for (int i = 0; i < dimensions; i++) {
    float diff = *(float*)first->getAt(i) - *(float*)second->getAt(i);
    result += fabs(diff);
  }

  return result;
}

int cppdescent::compareEdgesManhattan(Pointer first, Pointer second) {
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
