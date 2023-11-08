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

void swapEdges(GraphVertexPair* first, GraphVertexPair* second) {
  GraphVertexPair* temp = first;
  first = second;
  second = temp;
}

GraphVertexPair** cppdescent::EdgesBubbleSort(GraphVertexPair** edges,
                                              int size,
                                              CompareFunc compare) {
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size - 1; j++)
      if (compare((Pointer)edges[j], (Pointer)edges[j + 1]) < 0)
        swapEdges(edges[j], edges[j + 1]);

  return edges;
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

void cppdescent::EdgesQuickSort(GraphVertexPair** edges,
                                int low,
                                int high,
                                CompareFunc compare) {
  if (low < high) {
    int pi = partition(edges, low, high, compare);
    cppdescent::EdgesQuickSort(edges, low, pi - 1, compare);
    cppdescent::EdgesQuickSort(edges, pi + 1, high, compare);
  }
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
    if (compareFloats(vec1->getAt(i), vec2->getAt(i)))
      return 1;

  return 0;
}

void cppdescent::destroyEdges(GraphVertexPair* pair) {
  delete pair;
}

// FIXME
int cppdescent::hashEdge(Pointer edge) {
  GraphVertexPair* pair1 = (GraphVertexPair*)edge;
  pair1->getVertex1();
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

  GraphVertexPair** neighborsEdges = new GraphVertexPair*[K];

  // Neighbors for the element.
  for (int i = 0; i < data->getSize(); i++) {
    // The first K elements different to the vertex tested.
    for (int k = 0; k < K; k++) {
      GraphVertexPair* pair;

      if (i != k)
        pair = new GraphVertexPair(graph, (Pointer)data->getAt(i),
                                   (Pointer)data->getAt(k));
      else
        pair = new GraphVertexPair(graph, (Pointer)data->getAt(i),
                                   (Pointer)data->getAt(K));

      neighborsEdges[k] = pair;
    }

    EdgesQuickSort(neighborsEdges, 0, K - 1, compare);

    // Test the rest of the datapoints for closer neighbors.
    for (int j = K; j < data->getSize(); j++) {
      if (i != j) {
        GraphVertexPair* pair = new GraphVertexPair(
            graph, (Pointer)data->getAt(i), (Pointer)data->getAt(j));

        // further than the Kth neighbor
        if (compare((Pointer)pair, (Pointer)neighborsEdges[K - 1]) <= 0) {
          destroyEdges(pair);
          continue;
        }

        for (int k = K - 1; k >= 0; k--) {
          if (compare((Pointer)pair, (Pointer)neighborsEdges[k]) > 0) {
            swapEdges(pair, neighborsEdges[k]);
            destroyEdges(neighborsEdges[k]);
            break;
          }
        }
      }
    }

    for (int k = 0; k < K; k++) {
      graph->insertEdge((Pointer)data->getAt(i),
                        (Pointer)neighborsEdges[k]->getVertex2(),
                        distance((Pointer)data->getAt(i),
                                 (Pointer)neighborsEdges[k]->getVertex2()));
      destroyEdges(neighborsEdges[k]);
    }
  }

  delete[] neighborsEdges;

  return graph;
}