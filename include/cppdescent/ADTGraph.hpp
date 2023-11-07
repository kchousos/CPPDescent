/**
 * @file ADTGraph.hpp
 * @author Phaedon Seitanidis
 * @brief Abstract undirected graph with weighted edges.
 * @version 0.1
 * @date 2023-11-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "ADTList.hpp"
#include "ADTMap.hpp"
#include "ADTVector.hpp"

class Graph {
 private:
  Vector* vec;
  Map* map;
  int size;
  CompareFunc compare;
  DestroyFunc destroy;
  HashFunc hash;

 public:
  Graph(CompareFunc compare, DestroyFunc destroy);
  ~Graph();
  int getSize();
  void insertVertex(Pointer vertex);
  List* getVertices();
  void removeVertex(Pointer vertex);
  void insertEdge(Pointer vertex1, Pointer vertex2, float weight);
  void removeEdge(Pointer vertex1, Pointer vertex2);
  float getWeight(Pointer vertex1, Pointer vertex2);
  List* getAdjacent(Pointer vertex);
  // Map* shortestPathLengths();
  void setHashFunction(HashFunc hash);
  CompareFunc getCompare() { return this->compare; };
  DestroyFunc getDestroy() { return this->destroy; };
  HashFunc getHash() { return this->hash; };
};

class GraphVertexPair {
 public:
  GraphVertexPair(Graph* owner, Pointer vertex1, Pointer vertex2)
      : vertex1(vertex1), vertex2(vertex2), owner(owner){};
  Pointer getVertex1() { return this->vertex1; };
  Pointer getVertex2() { return this->vertex2; };
  Graph* getOwner() { return this->owner; };

 private:
  Pointer vertex1;
  Pointer vertex2;
  Graph* owner;
};