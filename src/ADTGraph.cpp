/**
 * @file ADTGraph.cpp
 * @author Pheadon Seitanidis
 * @brief
 * @version 0.1
 * @date 2023-11-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cppdescent/ADTGraph.hpp"
#include <climits>
#include <iostream>

int* createInt(int value) {
  int* p = new int;
  *p = value;
  return p;
}

float* createFloat(float value) {
  float* p = new float;
  *p = value;
  return p;
}

int compareVertexPair(GraphVertexPair* pair1, GraphVertexPair* pair2) {
  int first =
      pair1->getOwner()->getCompare()(pair1->getVertex1(), pair2->getVertex1());
  int second =
      pair1->getOwner()->getCompare()(pair1->getVertex2(), pair2->getVertex2());
  if (first)
    return first;
  else if (second)
    return second;

  return 0;
}

void destroyVertexPair(GraphVertexPair* pair) {
  delete pair;
}

void destroyValue(Pointer value) {
  delete (int*)value;
}

// Graph //

Graph::Graph(CompareFunc compare, DestroyFunc destroy)
    : size(0), compare(compare), destroy(destroy) {
  this->map = new Map((CompareFunc)compareVertexPair,
                      (DestroyFunc)destroyVertexPair, destroyValue);
  this->vec = new Vector(0, nullptr);
}

int Graph::getSize() {
  return this->size;
}

/**
 * @brief Insert a vertex to the graph (if it doesn't already exist).
 *
 * @param vertex A Pointer to the vertex to be unserted.
 */
void Graph::insertVertex(Pointer vertex) {
  if (this->vec->find(vertex, this->compare) == nullptr) {
    this->vec->insertLast(vertex);
    this->size++;
  }
}

/**
 * @brief
 *
 * The returned list needs to be deleted, but the list's elements are pointers
 * the the vector's elements, so the list shouldn't have a `destroyValue`.
 *
 * @return List*
 */
List* Graph::getVertices() {
  List* list = new List;
  ListNode* node = LIST_BOF;
  for (int i = 0; i < this->size; i++) {
    list->insertNext(node, this->vec->getAt(i));
    if (node != LIST_BOF)
      node = list->next(node);
    else
      node = list->getHead();
  }

  return list;
}

void Graph::removeVertex(Pointer vertex) {
  Pointer last = this->vec->nodeValue(this->vec->last());
  int i;
  for (i = 0; i < this->size; i++)
    if (this->vec->getAt(i) == vertex)
      break;

  this->vec->setAt(i, last);
  this->vec->setAt(this->size - 1, this->vec->getAt(i));
  this->vec->removeLast();

  this->size--;

  if (this->map->getFirst() != nullptr) {
    GraphVertexPair* pair = new GraphVertexPair(this, vertex, vertex);
    this->map->remove(pair);
  }
}

void Graph::insertEdge(Pointer vertex1, Pointer vertex2, float weight = 1) {
  if (this->vec->find(vertex1, this->compare) == nullptr ||
      this->vec->find(vertex2, this->compare) == nullptr)
    return;
  GraphVertexPair* pair = new GraphVertexPair(this, vertex1, vertex2);
  this->map->insert(pair, createFloat(weight));
}

void Graph::removeEdge(Pointer vertex1, Pointer vertex2) {
  GraphVertexPair* pair = new GraphVertexPair(this, vertex1, vertex2);
  this->map->remove(pair);
  delete pair;
}

float Graph::getWeight(Pointer vertex1, Pointer vertex2) {
  GraphVertexPair* pair = new GraphVertexPair(this, vertex1, vertex2);
  Pointer p = this->map->find(pair);
  delete pair;

  if (p != nullptr)
    return *(float*)p;
  return INT_MAX;
}

List* Graph::getAdjacent(Pointer vertex) {
  List* list = new List;
  ListNode* node = LIST_BOF;

  for (int i = 0; i < this->size; i++) {
    GraphVertexPair* pair =
        new GraphVertexPair(this, vertex, this->vec->getAt(i));
    if (this->map->find(pair) != MAP_EOF) {
      list->insertNext(node, pair->getVertex2());
      if (node != nullptr)
        node = list->next(node);
      else
        node = list->getHead();
    }
    delete pair;
  }

  return list;
}

int compareEdgeWeights(Pointer a, Pointer b) {
  GraphVertexPair* pair1 = (GraphVertexPair*)a;
  GraphVertexPair* pair2 = (GraphVertexPair*)b;

  float first =
      pair1->getOwner()->getWeight(pair1->getVertex1(), pair1->getVertex2());
  float second =
      pair2->getOwner()->getWeight(pair2->getVertex1(), pair2->getVertex2());

  float result = first - second;

  if (result < 0)
    return -1;
  else if (result > 0)
    return 1;
  else
    return 0;
}

void destroyEdgePair(GraphVertexPair* pair) {
  delete pair;
}

PQueue* Graph::getAdjacentPQ(Pointer vertex) {
  PQueue* pqueue =
      new PQueue(compareEdgeWeights, (DestroyFunc)destroyEdgePair, nullptr);

  for (int i = 0; i < this->size; i++) {
    GraphVertexPair* pair =
        new GraphVertexPair(this, vertex, this->vec->getAt(i));
    if (this->map->find(pair) != MAP_EOF)
      pqueue->insert(pair);
    else
      delete pair;
  }

  return pqueue;
}

List* Graph::getReverseAdjacent(Pointer vertex) {
  List* list = new List;
  ListNode* node = LIST_BOF;

  for (int i = 0; i < this->size; i++) {
    GraphVertexPair* pair =
        new GraphVertexPair(this, this->vec->getAt(i), vertex);
    if (this->map->find(pair) != MAP_EOF) {
      list->insertNext(node, pair->getVertex1());
      if (node != nullptr)
        node = list->next(node);
      else
        node = list->getHead();
    }
    delete pair;
  }

  return list;
}

// The function itself leaves memory leaks. But, it is only used for
// getGeneralNeighors, in which the memory of the revAdj is freed by another
// PQueue.
PQueue* Graph::getReverseAdjacentPQ(Pointer vertex) {
  PQueue* pqueue = new PQueue(compareEdgeWeights, nullptr, nullptr);

  for (int i = 0; i < this->size; i++) {
    GraphVertexPair* pair =
        new GraphVertexPair(this, this->vec->getAt(i), vertex);
    if (this->map->find(pair) != MAP_EOF)
      pqueue->insert(pair);
    else
      delete pair;
  }

  return pqueue;
}

List* Graph::getGeneralNeighbors(Pointer vertex) {
  List* generalNeighbors = this->getAdjacent(vertex);
  List* reverseAdjList = this->getReverseAdjacent(vertex);

  generalNeighbors->mergeLists(reverseAdjList);
  // delete reverseAdjList;
  return generalNeighbors;
}

void swap(Pointer p, Pointer q) {
  Pointer tmp = p;
  p = q;
  q = tmp;
}

PQueue* Graph::getGeneralNeighborsPQ(Pointer vertex) {
  PQueue* adj = getAdjacentPQ(vertex);
  PQueue* revAdj = getReverseAdjacentPQ(vertex);

  if (adj->getSize() < revAdj->getSize())
    swap((Pointer)adj, (Pointer)revAdj);

  int size = revAdj->getSize();
  for (int i = 0; i < size; i++) {
    adj->insert(revAdj->getMax());
    revAdj->removeMax();
  }

  delete revAdj;
  return adj;
}

bool Graph::isNeighbor(Pointer v1, Pointer v2) {
  GraphVertexPair* pair = new GraphVertexPair(this, v1, v2);
  Pointer p = this->map->find(pair);
  delete pair;

  if (p != nullptr)
    return true;
  return false;
}

Graph::~Graph() {
  delete this->vec;
  delete this->map;
}

// // TODO
// Map* Graph::shortestPathLengths() {
//   return nullptr;
// }

void Graph::setHashFunction(HashFunc hash) {
  this->hash = hash;
  this->map->setHashFunction(this->hash);
}