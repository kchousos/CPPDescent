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
#include <stdio.h>
#include <cmath>
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

Vector* cppdescent::readBinData(char* fp) {
  FILE* data = fopen(fp, "rb");

  uint32_t N;

  fread(&N, sizeof(uint32_t), 1, data);

  Vector* elements = new Vector(N, nullptr);

  float value;

  for (int i = 0; i < (int)N; i++) {
    Vector* datapoints = new Vector(100, deleteFloat);

    for (int j = 0; j < 100; j++) {
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

float cppdescent::euclideanDistance(Vector* first, Vector* second) {
  float result = 0;

  if (first->getSize() != second->getSize())
    return -1.0;

  for (int i = 0; i < first->getSize(); i++) {
    float diff = *(float*)first->getAt(i) - *(float*)second->getAt(i);
    result += pow(diff, 2);
  }

  result = sqrtf(result);
  return result;
}

int cppdescent::compareVertexPair(Pointer first, Pointer second) {
  GraphVertexPair* pair1 = (GraphVertexPair*)first;
  GraphVertexPair* pair2 = (GraphVertexPair*)second;

  float a = euclideanDistance((Vector*)pair1->getVertex1(),
                              (Vector*)pair1->getVertex2());
  float b = euclideanDistance((Vector*)pair2->getVertex1(),
                              (Vector*)pair2->getVertex2());

  // a < b and not a > b because we want the PQueue to return the *minimum*.
  if (a < b)
    return 1;
  else if (a > b)
    return -1;
  else
    return 0;
}

Graph* cppdescent::KNNBruteForceGraph(Vector* data, int K) {
  Graph* graph = new Graph(nullptr, nullptr);

  for (int i = 0; i < data->getSize(); i++) {
    // neighbors for the element
    PQueue* neighbors = new PQueue(compareVertexPair, nullptr, nullptr);
    // add the element to the graph as a vertex
    // FIXME: the neighbors must contain graph vertex pairs for the comparison
    // to work.
    graph->insertVertex((Pointer)data->getAt(i));

    for (int j = 0; j < data->getSize(); j++) {
      graph->insertVertex((Pointer)data->getAt(j));

      if (j != i)
        neighbors->insert((Pointer)data->getAt(j));
    }
    // Pull K elements
    // ! We need to pull the *minimum* K elements, not the maximum
    // For each one: add an edge from element to other element
    // in the graph
    for (int k = 0; k < K; k++) {
      Vector* vec = (Vector*)neighbors->getMax();
      neighbors->removeMax();
      graph->insertEdge((Pointer)data->getAt(i), (Pointer)vec, 1);
    }
    delete neighbors;
  }

  return graph;
}