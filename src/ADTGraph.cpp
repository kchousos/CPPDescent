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

int compareVertices(GraphVertex* v1, GraphVertex* v2) {
  return v1->getOwner()->getCompareData()(v1->getData(), v2->getData());
}

int compareVertexPair(GraphVertexPair* pair1, GraphVertexPair* pair2) {
  int first = compareVertices(pair1->getVertex1(), pair2->getVertex1());
  if (first)
    return first;

  int second = compareVertices(pair1->getVertex2(), pair2->getVertex2());
  if (second)
    return second;

  return 0;
}

void destroyVertexPair(GraphVertexPair* pair) {
  delete pair;
}

void destroyValue(Pointer value) {
  delete (int*)value;
}

void destroyVertex(GraphVertex* vertex) {
  DestroyFunc destroy = vertex->getOwner()->getDestroyData();
  if (destroy != nullptr)
    destroy(vertex->getData());

  delete vertex;
}

// Graph //

Graph::Graph(int K,
             CompareFunc compare_data,
             DestroyFunc destroy,
             DestroyFunc destroy_data)
    : size(0),
      K(K),
      compare_data(compare_data),
      destroy(destroy),
      destroy_data(destroy_data) {
  this->map = new Map((CompareFunc)compareVertexPair,
                      (DestroyFunc)destroyVertexPair, destroyValue);
  this->vec = new Vector(0, (DestroyFunc)destroyVertex);

  this->compare_vertices = (CompareFunc)compareVertices;
}

int Graph::getSize() {
  return this->size;
}

/**
 * @brief Insert a vertex to the graph (if it doesn't already exist).
 *
 * @param vertex A Pointer to the data to be inserted.
 */
void Graph::insertVertex(Pointer data) {
  GraphVertex* gvertex = new GraphVertex(data, this);

  if (this->vec->find(gvertex, this->compare_vertices) == nullptr) {
    this->vec->insertLast(gvertex);
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

void Graph::removeVertex(Pointer data) {
  GraphVertex* gvertex = new GraphVertex(data, this);
  int i = this->vec->findPos(gvertex, this->compare_vertices);

  if (i == -1) {
    std::cerr << "removeVertex: given vertex not found. Exiting...\n";
    return;
  }

  GraphVertex* newPos = new GraphVertex(
      ((GraphVertex*)this->vec->getAt(this->size - 1))->getData(), this);
  GraphVertex* newLast =
      new GraphVertex(((GraphVertex*)this->vec->getAt(i))->getData(), this);

  this->vec->setAt(i, newPos);
  this->vec->setAt(this->size - 1, newLast);
  this->vec->removeLast();

  this->size--;

  if (this->map->getFirst() != nullptr) {
    GraphVertexPair* pair = new GraphVertexPair(this, gvertex, gvertex);
    this->map->remove(pair);
    delete pair;
  }

  delete gvertex;
}

void Graph::insertEdge(Pointer data1, Pointer data2, float weight = 1) {
  GraphVertex* vertex1 = new GraphVertex(data1, this);
  GraphVertex* vertex2 = new GraphVertex(data2, this);

  GraphVertex* gvertex1 =
      (GraphVertex*)this->vec->find(vertex1, this->compare_vertices);
  GraphVertex* gvertex2 =
      (GraphVertex*)this->vec->find(vertex2, this->compare_vertices);

  if (gvertex1 == nullptr || gvertex2 == nullptr)
    return;

  bool alreadyMember = false;

  GraphVertexPair* pair = new GraphVertexPair(this, gvertex1, gvertex2);

  if (this->map->find(pair) != nullptr)
    alreadyMember = true;

  this->map->insert(pair, createFloat(weight));

  if (alreadyMember == false) {
    gvertex1->addNeighbor(pair);
    gvertex2->addReverse(pair);
  }

  delete vertex1;
  delete vertex2;
}

void Graph::removeEdge(Pointer data1, Pointer data2) {
  GraphVertex* vertex1 = new GraphVertex(data1, this);
  GraphVertex* vertex2 = new GraphVertex(data2, this);

  GraphVertex* gvertex1 =
      (GraphVertex*)this->vec->find(vertex1, this->compare_vertices);
  GraphVertex* gvertex2 =
      (GraphVertex*)this->vec->find(vertex2, this->compare_vertices);

  GraphVertexPair* pair = new GraphVertexPair(this, gvertex1, gvertex2);

  gvertex1->removeNeighbor(pair, (CompareFunc)compareVertexPair);
  gvertex2->removeReverse(pair, (CompareFunc)compareVertexPair);

  this->map->remove(pair);

  delete pair;
  delete vertex1;
  delete vertex2;
}

float Graph::getWeight(Pointer data1, Pointer data2) {
  GraphVertex* vertex1 = new GraphVertex(data1, this);
  GraphVertex* vertex2 = new GraphVertex(data2, this);
  GraphVertexPair* pair = new GraphVertexPair(this, vertex1, vertex2);
  Pointer p = this->map->find(pair);
  delete pair;
  delete vertex1;
  delete vertex2;

  if (p != nullptr)
    return *(float*)p;
  return INT_MAX;
}

List* Graph::getAdjacent(Pointer data) {
  List* list = new List;
  ListNode* node = LIST_BOF;
  GraphVertex* vertex = new GraphVertex(data, this);

  for (int i = 0; i < this->size; i++) {
    if (list->getSize() >= K)
      break;

    GraphVertexPair* pair =
        new GraphVertexPair(this, vertex, (GraphVertex*)this->vec->getAt(i));
    if (this->map->find(pair) != MAP_EOF) {
      list->insertNext(node, pair->getVertex2());
      if (node != nullptr)
        node = list->next(node);
      else
        node = list->getHead();
    }
    delete pair;
  }

  delete vertex;
  return list;
}

int compareEdgeWeights(Pointer a, Pointer b) {
  GraphVertexPair* pair1 = (GraphVertexPair*)a;
  GraphVertexPair* pair2 = (GraphVertexPair*)b;

  float first = pair1->getOwner()->getWeight(pair1->getVertex1()->getData(),
                                             pair1->getVertex2()->getData());
  float second = pair2->getOwner()->getWeight(pair2->getVertex1()->getData(),
                                              pair2->getVertex2()->getData());

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

PQueue* Graph::getAdjacentPQ(Pointer data) {
  GraphVertex* gvertex = new GraphVertex(data, this);
  GraphVertex* found =
      ((GraphVertex*)this->vec->find(gvertex, this->compare_vertices));

  if (found == nullptr) {
    std::cout << "Vertex not found" << std::endl;
    delete gvertex;
    return nullptr;
  }

  if (found->getNeighbors()->getSize() == 0) {
    std::cout << "No adjacents found." << std::endl;
    delete gvertex;
    return nullptr;
  }

  delete gvertex;
  return found->getNeighbors();
}

List* Graph::getReverseAdjacent(Pointer data) {
  List* list = new List;
  ListNode* node = LIST_BOF;
  GraphVertex* vertex = new GraphVertex(data, this);

  for (int i = 0; i < this->size; i++) {
    GraphVertexPair* pair =
        new GraphVertexPair(this, (GraphVertex*)this->vec->getAt(i), vertex);
    if (this->map->find(pair) != MAP_EOF) {
      list->insertNext(node, pair->getVertex1());
      if (node != nullptr)
        node = list->next(node);
      else
        node = list->getHead();
    }
    delete pair;
  }

  delete vertex;
  return list;
}

// The function itself leaves memory leaks. But, it is only used for
// getGeneralNeighors, in which the memory of the revAdj is freed by another
// PQueue.
PQueue* Graph::getReverseAdjacentPQ(Pointer data) {
  GraphVertex* gvertex = new GraphVertex(data, this);
  GraphVertex* found =
      ((GraphVertex*)this->vec->find(gvertex, this->compare_vertices));

  if (found == nullptr) {
    std::cout << "Vertex not found" << std::endl;
    delete gvertex;
    return nullptr;
  }
  if (found->getReverse()->getSize() == 0) {
    std::cout << "No reverse adjacents found." << std::endl;
    delete gvertex;
    return nullptr;
  }

  delete gvertex;
  return found->getReverse();
}

List* Graph::getGeneralNeighbors(Pointer data) {
  List* generalNeighbors = this->getAdjacent(data);
  List* reverseAdjList = this->getReverseAdjacent(data);

  generalNeighbors->mergeLists(reverseAdjList);
  // delete reverseAdjList;
  return generalNeighbors;
}

void swap(Pointer p, Pointer q) {
  Pointer tmp = p;
  p = q;
  q = tmp;
}

PQueue* Graph::getGeneralNeighborsPQ(Pointer data) {
  PQueue* adj = getAdjacentPQ(data);
  PQueue* revAdj = getReverseAdjacentPQ(data);

  if (adj->getSize() < revAdj->getSize())
    swap((Pointer)adj, (Pointer)revAdj);

  int size = revAdj->getSize();
  for (int i = 0; i < size; i++) {
    adj->insert(revAdj->getMax());
    revAdj->removeMax();
  }

  return adj;
}

bool Graph::isNeighbor(Pointer v1, Pointer v2) {
  GraphVertexPair* pair =
      new GraphVertexPair(this, (GraphVertex*)v1, (GraphVertex*)v2);
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

GraphVertex::GraphVertex(Pointer data, Graph* owner)
    : data(data), owner(owner) {
  neighbors = new PQueue(compareEdgeWeights, nullptr, nullptr);
  reverse = new PQueue(compareEdgeWeights, nullptr, nullptr);
}

GraphVertex::~GraphVertex() {
  delete neighbors;
  delete reverse;
}