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

int cppdescent::compareFloats(Pointer a, Pointer b) {
  return (int)((float*)a - (float*)b);
  ;
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
      float* datapoint = createFloat(value);
      datapoints->setAt(j, datapoint);
    }

    elements->setAt(i, datapoints);
  }

  fclose(data);

  return elements;
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

  GraphVertexPair** neighborsEdges = new GraphVertexPair*[K];

  // Insert all points as vertices.
  for (int i = 0; i < data->getSize(); i++)
    graph->insertVertex((Pointer)data->getAt(i));

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

    // Test the rest of the datapoints for closer neighbors.
    for (int j = K; j < data->getSize(); j++) {
      if (i != j) {
        GraphVertexPair* pair = new GraphVertexPair(
            graph, (Pointer)data->getAt(i), (Pointer)data->getAt(j));

        for (int k = 0; k < K; k++)
          if (compare((Pointer)pair, (Pointer)neighborsEdges[k]) > 0) {
            delete neighborsEdges[k];
            neighborsEdges[k] = new GraphVertexPair(
                graph, (Pointer)data->getAt(i), (Pointer)data->getAt(j));
          }
        delete pair;
      }
    }

    for (int k = 0; k < K; k++) {
      graph->insertEdge((Pointer)data->getAt(i),
                        (Pointer)neighborsEdges[k]->getVertex2(),
                        distance((Pointer)data->getAt(i),
                                 (Pointer)neighborsEdges[k]->getVertex2()));
      delete neighborsEdges[k];
    }
  }

  delete[] neighborsEdges;

  return graph;
}