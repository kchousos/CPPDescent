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
  return *(float*)a - *(float*)b;
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

int cppdescent::compareEdges(Pointer first, Pointer second) {
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

Graph* cppdescent::KNNBruteForceGraph(Vector* data, int K) {
  Graph* graph = new Graph((CompareFunc)compareVertices, nullptr);
  graph->setHashFunction((HashFunc)hashEdge);

  for (int i = 0; i < data->getSize(); i++) {
    // Neighbors for the element.
    PQueue* neighborsEdges =
        new PQueue((CompareFunc)compareEdges, nullptr, nullptr);
    // Add the element to the graph as a vertex.
    // The neighbors must contain graph vertex pairs for the comparison
    // to work.
    graph->insertVertex((Pointer)data->getAt(i));

    for (int j = 0; j < data->getSize(); j++) {
      graph->insertVertex((Pointer)data->getAt(j));

      if (j != i) {
        // The pairs will (supposedly) be deleted by the pqueue destructor.
        GraphVertexPair* pair = new GraphVertexPair(
            graph, (Pointer)data->getAt(i), (Pointer)data->getAt(j));
        neighborsEdges->insert((Pointer)pair);
      }
    }
    // Pull K elements.
    // ! We need to pull the *minimum* K elements, not the maximum
    // For each one: add an edge from element to other element
    // in the graph.
    for (int k = 0; k < K; k++) {
      GraphVertexPair* min = (GraphVertexPair*)neighborsEdges->getMax();
      neighborsEdges->removeMax();
      Vector* vec = (Vector*)min->getVertex2();
      graph->insertEdge((Pointer)data->getAt(i), (Pointer)vec, 2);
    }
    delete neighborsEdges;
  }

  return graph;
}