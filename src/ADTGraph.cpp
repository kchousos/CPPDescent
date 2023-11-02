/**
 * @file ADTGraph.cpp
 * @author Konstantinos Chousos
 * @brief
 * @version 0.1
 * @date 2023-11-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cppdescent/ADTGraph.hpp"
#include <climits>

int* createInt(int value) {
  int* p = new int;
  *p = value;
  return p;
}

int compareVertexPair(GraphVertexPair* pair1, GraphVertexPair* pair2) {
  if (pair1->getOwner()->getCompare()(pair1->getVertex1(), pair2->getVertex1()))
    return pair1->getOwner()->getCompare()(pair1->getVertex1(),
                                           pair2->getVertex1());
  else if (pair1->getOwner()->getCompare()(pair1->getVertex2(),
                                           pair2->getVertex2()))
    return pair1->getOwner()->getCompare()(pair1->getVertex2(),
                                           pair2->getVertex2());

  return 0;
}

void destroyVertexPair(GraphVertexPair* pair) {
  pair->getOwner()->getDestroy()(pair->getVertex1());
  pair->getOwner()->getDestroy()(pair->getVertex2());
}

void destroyValue(Pointer value) {
  delete (int*)value;
}

Graph::Graph(CompareFunc compare, DestroyFunc destroy)
    : size(0), compare(compare), destroy(destroy) {
  this->map = new Map((CompareFunc)compareVertexPair,
                      (DestroyFunc)destroyVertexPair, destroyValue);
  this->vec = new Vector(0, nullptr);
}

int Graph::getSize() {
  return this->size;
}

void Graph::insertVertex(Pointer vertex) {
  this->vec->insertLast(vertex);
  this->size++;
}

List* Graph::getVertices() {
  // FIXME: free me
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

void Graph::insertEdge(Pointer vertex1, Pointer vertex2, int weight) {
  GraphVertexPair* pair = new GraphVertexPair(this, vertex1, vertex2);
  this->map->setHashFunction(this->hash);
  this->map->insert(pair, createInt(weight));
}

void Graph::removeEdge(Pointer vertex1, Pointer vertex2) {
  GraphVertexPair* pair = new GraphVertexPair(this, vertex1, vertex2);
  this->map->remove(pair);
  delete pair;
}

int Graph::getWeight(Pointer vertex1, Pointer vertex2) {
  GraphVertexPair* pair = new GraphVertexPair(this, vertex1, vertex2);
  Pointer p = this->map->find(pair);

  if (p != nullptr)
    return *(int*)p;
  return INT_MAX;
}

List* Graph::getAdjacent(Pointer vertex) {
  List* list = new List;
  ListNode* node = LIST_BOF;
  for (int i = 0; i < this->size; i++) {
    GraphVertexPair* pair =
        new GraphVertexPair(this, vertex, this->vec->getAt(i));

    if (this->map->find(pair) != nullptr) {
      list->insertNext(node, pair->getVertex2());
      if (node != nullptr)
        node = list->next(node);
      else
        node = list->getHead();
    }
  }

  return list;
}

Graph::~Graph() {
  delete this->vec;
  delete this->map;
}

// TODO
Map* Graph::shortestPathLengths() {
  return nullptr;
}

void Graph::setHashFunction(HashFunc hash) {
  this->hash = hash;
}