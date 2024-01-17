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

#include "ADTPQueue.hpp"
#include "ADTVector.hpp"

class Graph {
 private:
  Vector* vec;
  int size;
  CompareFunc compare_vertices;
  CompareFunc compare_data;
  DestroyFunc destroy_data;

 public:
  Graph(CompareFunc compare, DestroyFunc vecDestroy);
  ~Graph();
  int getSize();
  void insertVertex(Pointer vertex);
  Vector* getVerticesV();
  void removeVertex(Pointer vertex);
  void insertEdge(Pointer vertex1, Pointer vertex2);
  void removeEdge(Pointer vertex1, Pointer vertex2);
  Vector* getAdjacentV(Pointer vertex);
  Vector* getReverseAdjacentV(Pointer vertex);
  Vector* getGeneralNeighborsV(Pointer vertex);
  bool isNeighborVertex(Pointer v1, Pointer v2);
  CompareFunc getCompareData() { return this->compare_data; };
  CompareFunc getCompareVertices() { return this->compare_vertices; };
  DestroyFunc getDestroyData() { return this->destroy_data; };
  Vector* getVec() { return this->vec; };
};

class GraphVertex {
 private:
  Pointer data;
  PQueue* neighbors;
  PQueue* reverse;
  Graph* owner;
  bool hasBeenChecked;
  double norm;

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
  void check() { this->hasBeenChecked = true; };
  bool checked() { return this->hasBeenChecked; };
  void setNorm(double norm) { this->norm = norm; };
  double getNorm() { return this->norm; };
};

class GraphVertexPair {
 public:
  GraphVertexPair(Graph* owner, Pointer vertex1, Pointer vertex2)
      : flag(true), vertex1(vertex1), vertex2(vertex2), owner(owner){};
  Pointer getVertex1() { return this->vertex1; };
  Pointer getVertex2() { return this->vertex2; };
  Graph* getOwner() { return this->owner; };
  void setFalse() { this->flag = false; };
  bool getFlag() { return this->flag; };

 private:
  bool flag;
  Pointer vertex1;
  Pointer vertex2;
  Graph* owner;
};