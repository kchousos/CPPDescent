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
#include "ADTPQueue.hpp"
#include "ADTVector.hpp"

class Graph {
 private:
  Vector* vec;
  Map* map;
  int size;
  int K;
  CompareFunc compare_vertices;
  CompareFunc compare_data;
  DestroyFunc destroy;
  DestroyFunc destroy_data;
  HashFunc hash;

 public:
  Graph(int K,
        CompareFunc compare,
        DestroyFunc destroy,
        DestroyFunc vecDestroy = nullptr);
  ~Graph();
  int getSize();
  void insertVertex(Pointer vertex);
  List* getVertices();
  void removeVertex(Pointer vertex);
  void insertEdge(Pointer vertex1, Pointer vertex2, float weight);
  void removeEdge(Pointer vertex1, Pointer vertex2);
  float getWeight(Pointer vertex1, Pointer vertex2);
  List* getAdjacent(Pointer vertex);
  PQueue* getAdjacentPQ(Pointer vertex);
  List* getReverseAdjacent(Pointer vertex);
  PQueue* getReverseAdjacentPQ(Pointer vertex);
  List* getGeneralNeighbors(Pointer vertex);
  PQueue* getGeneralNeighborsPQ(Pointer vertex);
  bool isNeighbor(Pointer v1, Pointer v2);
  // Map* shortestPathLengths();
  void setHashFunction(HashFunc hash);
  CompareFunc getCompareData() { return this->compare_data; };
  CompareFunc getCompareVertices() { return this->compare_vertices; };
  DestroyFunc getDestroy() { return this->destroy; };
  DestroyFunc getDestroyData() { return this->destroy_data; };
  HashFunc getHash() { return this->hash; };
  Vector* getVec() { return this->vec; };
  Map* getMap() { return this->map; };
};

class GraphVertex {
 private:
  Pointer data;
  PQueue* neighbors;
  PQueue* reverse;
  Graph* owner;

 public:
  GraphVertex(Pointer data, Graph* owner);
  ~GraphVertex();
  void addNeighbor(Pointer neighbor) { this->neighbors->insert(neighbor); };
  void addReverse(Pointer reverse) { this->reverse->insert(reverse); };
  void removeNeighbor(Pointer neighbor, CompareFunc compare) {
    this->neighbors->remove(neighbor, compare);
  };
  void removeReverse(Pointer reverse, CompareFunc compare) {
    this->reverse->remove(reverse, compare);
  };
  PQueue* getNeighbors() { return this->neighbors; };
  PQueue* getReverse() { return this->reverse; };
  Pointer getData() { return this->data; };
  Graph* getOwner() { return this->owner; };
};

class GraphVertexPair {
 public:
  GraphVertexPair(Graph* owner, GraphVertex* vertex1, GraphVertex* vertex2)
      : vertex1(vertex1), vertex2(vertex2), owner(owner){};
  GraphVertex* getVertex1() { return this->vertex1; };
  GraphVertex* getVertex2() { return this->vertex2; };
  Graph* getOwner() { return this->owner; };

 private:
  GraphVertex* vertex1;
  GraphVertex* vertex2;
  Graph* owner;
};