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
#include <cstdint>
#include <iostream>
#include "cppdescent/ADTPQueue.hpp"

//===================================
// Helper functions.
//===================================

void swapEdges(GraphVertexPair* first, GraphVertexPair* second) {
  GraphVertexPair temp = *first;
  *first = *second;
  *second = temp;
}

void EdgesBubbleSort(GraphVertexPair** edges, int size, CompareFunc compare) {
  for (int j = size - 1; j > 0; j--)
    if (compare((Pointer)edges[j], (Pointer)edges[j - 1]) > 0)
      swapEdges(edges[j], edges[j - 1]);
}

int partition(GraphVertexPair** edges, int low, int high, CompareFunc compare) {
  // choose the pivot

  GraphVertexPair* pivot = edges[high];
  // Index of smaller element and Indicate
  // the right position of pivot found so far
  int i = (low - 1);

  for (int j = low; j <= high; j++) {
    // If current element is smaller than the pivot
    if (compare((Pointer)edges[j], (Pointer)pivot) > 0) {
      // Increment index of smaller element
      i++;
      swapEdges(edges[i], edges[j]);
    }
  }
  swapEdges(edges[i + 1], edges[high]);
  return (i + 1);
}

void EdgesQuickSort(GraphVertexPair** edges,
                    int low,
                    int high,
                    CompareFunc compare) {
  if (low < high) {
    int pi = partition(edges, low, high, compare);
    EdgesQuickSort(edges, low, pi - 1, compare);
    EdgesQuickSort(edges, pi + 1, high, compare);
  }
}

bool EdgesBinarySearch(GraphVertexPair** edges,
                       GraphVertexPair* edge,
                       int low,
                       int high,
                       CompareFunc compare) {
  while (low <= high) {
    int mid = low + (high - low) / 2;

    if (compare(edges[mid], edge) == 0)
      return true;

    if (compare(edges[mid], edge) > 0)
      low = mid + 1;
    else
      high = mid - 1;
  }
  return false;
}

void destroyEdges(GraphVertexPair* pair) {
  delete pair;
}

uint hashEdge(Pointer value) {
  GraphVertexPair* pair = (GraphVertexPair*)value;
  size_t hash = (size_t)pair->getVertex1() + (size_t)pair->getVertex2();
  return hash;
}

//===================================
// CPPDescent functions.
//===================================

Vector* cppdescent::readBinData(char* fp, int dimensions) {
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
    return -1;
  for (int i = 0; i < vec->getSize(); i++) {
    delete (Vector*)vec->getAt(i);
  }

  delete vec;
  return 0;
}

int cppdescent::compareVertices(Pointer first, Pointer second) {
  Vector* vec1 = (Vector*)first;
  Vector* vec2 = (Vector*)second;

  for (int i = 0; i < vec1->getSize(); i++)
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
  for (int i = 0; i < data->getSize(); i++)
    graph->insertVertex((Pointer)data->getAt(i));

  int N = data->getSize();

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
    graph->insertVertex((Pointer)data->getAt(i));

  // Iterate all of the vertices.
  for (int i = 0; i < N; i++) {
    // For each vertex, add K random neighbors.
    for (int j = 0; j < K; j++) {
      int randPos = rand() % N;  // The position of the neighbor.

      // Avoid adding itself as a neighbor.
      while (randPos == i)
        randPos = rand() % N;

      // Get the two vertices and create an edge between them.
      Pointer v1 = (Pointer)data->getAt(i);
      Pointer v2 = (Pointer)data->getAt(randPos);
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
             Pointer v,
             Pointer u2,
             float dist,
             DistanceFunc distance) {
  PQueue* vAll = graph->getAdjacentPQ(v);
  Pointer max = ((GraphVertexPair*)vAll->getMax())->getVertex2();
  float maxDist = distance(v, max);

  if (dist < maxDist) {
    graph->removeEdge(v, max);
    graph->insertEdge(v, u2, dist);
    return 1;
  }

  // delete vAll;
  return 0;
}

Graph* cppdescent::NNDescent_KNNGraph(Vector* data,
                                      int K,
                                      DistanceFunc distance) {
  // B[v] <- Sample(V, K) for all v in V
  Graph* graph = sampleGraph(data, K, (CompareFunc)compareVertices, distance);
  // The vertices do not change, only the edges between them are modified. So we
  // only need to get them once and not in each iteration.
  List* vertices = graph->getVertices();
  int c;

  int iterations = 0;

  do {
    iterations++;

    c = 0;

    for (ListNode* v = vertices->getHead(); v != nullptr; v = v->getNext()) {
      // vAll = Bbar[v] = B[v] U R[v]
      List* vAll = graph->getGeneralNeighbors(v->getValue());

      for (ListNode* u1 = vAll->getHead(); u1 != nullptr; u1 = u1->getNext()) {
        // u1All = Bbar[u1] = B[u1] U R[u1]
        List* u1All = graph->getGeneralNeighbors(u1->getValue());

        for (ListNode* u2 = u1All->getHead(); u2 != nullptr;
             u2 = u2->getNext()) {
          // Otherwise the same edge could be added twice.
          if (graph->isNeighbor(v->getValue(), u2->getValue()) == true)
            continue;

          float dist = distance(v->getValue(), u2->getValue());

          c += updateNN(graph, v->getValue(), u2->getValue(), dist, distance);
        }

        delete u1All;
      }

      delete vAll;
    }
  } while (c != 0);

  delete vertices;

  std::cout << "Iterations: " << iterations << "\n";

  return graph;
}

List* NNDescent_Query(Graph* graph, int K, CompareFunc compare, Vector* query) {
  List* vertices = graph->getVertices();

  srand(time(0));

  int pos = rand() % vertices->getSize();

  ListNode* node = vertices->getHead();
  for (int i = 0; i <= pos; i++)
    node = node->getNext();

  PQueue* knn = new PQueue(compare, (DestroyFunc)destroyEdges, nullptr);
  Pointer candidate = node->getValue();

  GraphVertexPair* pair = new GraphVertexPair(graph, (Pointer)query, candidate);
  knn->insert(pair);

  List* candidates = graph->getAdjacent(candidate);

  while (true) {
    for (ListNode* node = candidates->getHead(); node != nullptr;
         node = node->getNext()) {
      pair = new GraphVertexPair(graph, (Pointer)query, node->getValue());
      if (knn->getSize() < K || compare(knn->getMax(), pair) > 0) {
        knn->removeMax();
        knn->insert(pair);
      } else {
        delete pair;
      }

      List* candidateNeighbors = graph->getAdjacent(node->getValue());
      candidateNeighbors->decreaseSize();
    }
  }

  delete vertices;
}