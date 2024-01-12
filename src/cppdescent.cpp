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

  float first = pair1->getOwner()->getWeight(
      ((GraphVertex*)pair1->getVertex1())->getData(),
      ((GraphVertex*)pair1->getVertex2())->getData());
  float second = pair2->getOwner()->getWeight(
      ((GraphVertex*)pair2->getVertex1())->getData(),
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

  for (int bfNode = 0, nnNode = 0; bfNode < bfVertices->getSize();
       bfNode++, nnNode++) {
    int trueNeighbors = 0;
    Vector* bfNodeAdjacent = bfGraph->getAdjacentV(bfVertices->getAt(bfNode));

    for (int adjacent = 0; adjacent < bfNodeAdjacent->getSize(); adjacent++) {
      Vector* nnAdjacent = nnGraph->getAdjacentV(nnVertices->getAt(nnNode));

      if (nnAdjacent->find(bfNodeAdjacent->getAt(adjacent),
                           cppdescent::compareNeighbors))
        trueNeighbors++;
    }

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
      float weight = distance(v1, v2);
      graph->insertEdge(v1, v2, weight);
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

Graph* cppdescent::NNDescent_KNNGraph(Vector* data,
                                      int K,
                                      float delta,
                                      DistanceFunc distance) {
  // B[v] <- Sample(V, K) for all v in V
  Graph* graph = sampleGraph(data, K, (CompareFunc)compareVertices, distance);
  std::cout << "Starting graph has been created\n";
  // The vertices do not change, only the edges between them are modified. So we
  // only need to get them once and not in each iteration.
  Vector* vertices = graph->getVerticesV();
  int N = graph->getSize();
  int c;
  int iterations = 0;
  float dist;

  do {
    iterations++;

    c = 0;

    for (int v = 0; v < N; v++) {
      // vAll = Bbar[v] = B[v] ⋃ R[v]
      Vector* vAll = graph->getGeneralNeighborsV(vertices->getAt(v));
      int neighborsNum = vAll->getSize();

      for (int U1 = 0; U1 < neighborsNum; U1++) {
        Neighbor* neighbor1 = (Neighbor*)vAll->getAt(U1);

        // the direct neighbors are in the first K cells of vAll, the rest are
        // reverse.
        int direct = U1 < K ? 1 : 0;
        GraphVertex* u1 = getOther(neighbor1, direct);

        for (int U2 = U1 + 1; U2 < neighborsNum; U2++) {
          Neighbor* neighbor2 = (Neighbor*)vAll->getAt(U2);

          if (!neighbor1->getFlag())
            continue;

          // the direct neighbors are in the first K cells of vAll, the rest
          // are reverse.
          int direct2 = U2 < K ? 1 : 0;
          GraphVertex* u2 = getOther(neighbor2, direct2);

          dist = distance(u1->getData(), u2->getData());

          if (graph->isNeighbor(u1->getData(), u2->getData()) == false)
            c += updateNN(graph, u1, u2, dist, distance);

          if (graph->isNeighbor(u2->getData(), u1->getData()) == false)
            c += updateNN(graph, u2, u1, dist, distance);
        }

        neighbor1->setFalse();
      }

      delete vAll;
    }

    std::cout << "Number of changes in the graph (c) = " << c << "\n";
  } while (c >= delta * N * K);

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
