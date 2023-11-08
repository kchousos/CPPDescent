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

// Graph* cppdescent::KNNBruteForceGraph(Vector* data,
//                                       int K,
//                                       CompareFunc compare,
//                                       DistanceFunc distance) {
//   Graph* graph = new Graph((CompareFunc)compareVertices, nullptr);
//   graph->setHashFunction((HashFunc)hashEdge);

//   // Insert all points as vertices.
//   for (int i = 0; i < data->getSize(); i++)
//     graph->insertVertex((Pointer)data->getAt(i));

//   GraphVertexPair** neighborsEdges = new GraphVertexPair*[K];

//   // Neighbors for the element.
//   for (int i = 0; i < data->getSize(); i++) {
//     // The first K elements different to the vertex tested.
//     for (int k = 0; k < K; k++) {
//       GraphVertexPair* pair;

//       if (i != k)
//         pair = new GraphVertexPair(graph, (Pointer)data->getAt(i),
//                                    (Pointer)data->getAt(k));
//       else
//         // To ensure that K elements are inserted
//         pair = new GraphVertexPair(graph, (Pointer)data->getAt(i),
//                                    (Pointer)data->getAt(K));

//       neighborsEdges[k] = pair;
//     }

//     EdgesQuickSort(neighborsEdges, 0, K - 1, compare);

//     // Test the rest of the datapoints for closer neighbors.
//     for (int j = K; j < data->getSize(); j++) {
//       if (i != j) {
//         GraphVertexPair* pair = new GraphVertexPair(
//             graph, (Pointer)data->getAt(i), (Pointer)data->getAt(j));

//         // If the current neighbor is nearer than the Kth (furthest) neighbor
//         if (compare((Pointer)pair, (Pointer)neighborsEdges[K - 1]) > 0 &&
//             !EdgesBinarySearch(neighborsEdges, pair, 0, K - 1, compare)) {
//           // swap with last element
//           swapEdges(pair, neighborsEdges[K - 1]);
//           // sort the newly added element
//           EdgesBubbleSort(neighborsEdges, K, compare);
//         }

//         // destroy the old Kth neighbor
//         destroyEdges(pair);
//       }
//     }

//     for (int k = 0; k < K; k++) {
//       graph->insertEdge((Pointer)data->getAt(i),
//                         (Pointer)neighborsEdges[k]->getVertex2(),
//                         distance((Pointer)data->getAt(i),
//                                  (Pointer)neighborsEdges[k]->getVertex2()));
//       destroyEdges(neighborsEdges[k]);
//     }
//   }

//   delete[] neighborsEdges;

//   return graph;
// }

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

int updateNN(List* adjList, GraphVertexPair* pair, float weight) {

}

Graph* cppdescent::NNDescent(Vector* data, int K, CompareFunc compare, DistanceFunc distance) {
  Graph* graph = new Graph((CompareFunc)compareVertices, nullptr);
  graph->setHashFunction((HashFunc)hashEdge);

  // Insert all points as vertices.
  for (int i = 0; i < data->getSize(); i++)
    graph->insertVertex((Pointer)data->getAt(i));
  
  for (int i = 0; i < data->getSize(); i++) {
    for (int j = 0; j < K; j++) {
      int randPos = rand() % data->getSize();
      while (randPos == j)
        randPos = rand() % data->getSize();

      Pointer v1 = (Pointer)data->getAt(i);
      Pointer v2 = (Pointer)data->getAt(randPos);
      float weight = distance(v1, v2);
      graph->insertEdge(v1, v2, weight);
    }
  }

  for (int i = 0; i < data->getSize(); i++) {
    List* generalNeighbors = graph->getGeneralNeighbors(data->getAt(i));
    int counter = 0;

    ListNode* node = generalNeighbors->getHead();
    for (int j = 0; j < generalNeighbors->getSize(); j++) {
      List* secondGradeNeighbors = graph->getGeneralNeighbors(node->getValue());
      ListNode* node2 = secondGradeNeighbors->getHead();

      float dist = distance(data->getAt(i), node2->getValue());
      GraphVertexPair* pair = new GraphVertexPair();
      counter += updateNN(graph->getAdjacent(data->getAt(i)));
    }
  }
}
