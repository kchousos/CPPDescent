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
#include <gsl/gsl_vector.h>
#include <climits>
#include <iostream>
#include "cppdescent/cppdescent.hpp"

float* createFloat(float value) {
  float* p = new float;
  *p = value;
  return p;
}

int compareGraphVertexPairs(Pointer p1, Pointer p2) {
  GraphVertexPair* pair1 = (GraphVertexPair*)p1;
  GraphVertexPair* pair2 = (GraphVertexPair*)p2;

  if (!gsl_vector_equal(
          (gsl_vector*)((GraphVertex*)pair1->getVertex1())->getData(),
          (gsl_vector*)((GraphVertex*)pair2->getVertex1())->getData()) ||
      !gsl_vector_equal(
          (gsl_vector*)((GraphVertex*)pair2->getVertex1())->getData(),
          (gsl_vector*)((GraphVertex*)pair2->getVertex2())->getData()))
    return 1;

  return 0;
}

int compareVertexPair(GraphVertexPair* pair1, GraphVertexPair* pair2) {
  int first = pair1->getOwner()->getCompareData()(pair1->getVertex1(),
                                                  pair2->getVertex1());
  if (first)
    return first;

  int second = pair1->getOwner()->getCompareData()(pair1->getVertex2(),
                                                   pair2->getVertex2());
  if (second)
    return second;

  return 0;
}

int compareVertices(Pointer vertex1, Pointer vertex2) {
  GraphVertex* v1 = (GraphVertex*)vertex1;
  GraphVertex* v2 = (GraphVertex*)vertex2;

  if (gsl_vector_equal((gsl_vector*)v1->getData(), (gsl_vector*)v2->getData()))
    return 0;
  else
    return 1;
}

int compareNeighbors(Pointer neighbor1, Pointer neighbor2) {
  GraphVertexPair* pair1 = (GraphVertexPair*)neighbor1;
  GraphVertexPair* pair2 = (GraphVertexPair*)neighbor2;

  float first = cppdescent::euclideanDistance(
      ((GraphVertex*)pair1->getVertex1())->getData(),
      ((GraphVertex*)pair1->getVertex2())->getData());
  float second = cppdescent::euclideanDistance(
      ((GraphVertex*)pair2->getVertex1())->getData(),
      ((GraphVertex*)pair2->getVertex2())->getData());

  float result = first - second;

  if (result < 0)
    return -1;
  else if (result > 0)
    return 1;
  else
    return 0;
}

int compareNeighborsBin(Pointer neighbor1, Pointer neighbor2) {
  GraphVertexPair* pair1 = (GraphVertexPair*)neighbor1;
  GraphVertexPair* pair2 = (GraphVertexPair*)neighbor2;

  int first = pair1->getOwner()->getCompareData()(
      ((GraphVertex*)pair1->getVertex1())->getData(),
      ((GraphVertex*)pair2->getVertex1())->getData());
  if (first)
    return first;

  int second = pair1->getOwner()->getCompareData()(
      ((GraphVertex*)pair1->getVertex2())->getData(),
      ((GraphVertex*)pair2->getVertex2())->getData());
  if (second)
    return second;

  return 0;
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

Graph::Graph(CompareFunc compare_data,
             DestroyFunc destroy,
             DestroyFunc destroy_data)
    : size(0),
      compare_data(compare_data),
      destroy(destroy),
      destroy_data(destroy_data) {
  this->vec = new Vector(0, (DestroyFunc)destroyVertex);

  this->compare_vertices = compareVertices;
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
  GraphVertex* gvertex = new GraphVertex(vertex, this);

  if (this->vec->find(gvertex, this->compare_vertices) == nullptr) {
    this->vec->insertLast(gvertex);
    this->size++;
  }
}

/**
 * The returned list needs to be deleted, but the list's elements are pointers
 * the the vector's elements, so the list shouldn't have a `destroyValue`.
 *
 * @return List*
 */
List* Graph::getVertices() {
  List* list = new List;
  ListNode* node = LIST_BOF;
  for (int i = 0; i < this->size; i++) {
    list->insertNext(node, ((GraphVertex*)this->vec->getAt(i))->getData());
    if (node != LIST_BOF)
      node = list->next(node);
    else
      node = list->getHead();
  }

  return list;
}

Vector* Graph::getVerticesV() {
  return this->vec;
}

void Graph::removeVertex(Pointer vertex) {
  GraphVertex* gvertex = new GraphVertex(vertex, this);
  int i = this->vec->findPos(gvertex, this->compare_vertices);

  // LCOV_EXCL_START
  if (i == -1) {
    std::cerr << "removeVertex: given vertex not found. Exiting...\n";
    return;
  }
  // LCOV_EXCL_STOP

  GraphVertex* newPos = new GraphVertex(
      ((GraphVertex*)this->vec->getAt(this->size - 1))->getData(), this);
  GraphVertex* newLast =
      new GraphVertex(((GraphVertex*)this->vec->getAt(i))->getData(), this);

  this->vec->setAt(i, newPos);
  this->vec->setAt(this->size - 1, newLast);
  this->vec->removeLast();

  this->size--;

  delete gvertex;
}

void Graph::insertEdge(Pointer data1, Pointer data2) {
  // GraphVertex* vertex1 = new GraphVertex(data1, this);
  // GraphVertex* vertex2 = new GraphVertex(data2, this);

  // GraphVertex* gvertex1 =
  //     (GraphVertex*)this->vec->find(vertex1, this->compare_vertices);
  // GraphVertex* gvertex2 =
  //     (GraphVertex*)this->vec->find(vertex2, this->compare_vertices);

  // if (gvertex1 == nullptr || gvertex2 == nullptr)
  //   return;  // LCOV_EXCL_LINE

  GraphVertex* gvertex1 = (GraphVertex*)data1;
  GraphVertex* gvertex2 = (GraphVertex*)data2;

  bool alreadyMember = false;

  GraphVertexPair* pair = new GraphVertexPair(this, gvertex1, gvertex2);

  if (gvertex1->getNeighbors()->find(
          pair, (CompareFunc)compareGraphVertexPairs) != -1) {
    alreadyMember = true;
    delete pair;
  }

  if (alreadyMember == false) {
    gvertex1->addNeighbor(pair);
    gvertex2->addReverse(pair);
  }

  // delete vertex1;
  // delete vertex2;
}

void Graph::removeEdge(Pointer data1, Pointer data2) {
  GraphVertex* vertex1 = new GraphVertex(data1, this);
  GraphVertex* vertex2 = new GraphVertex(data2, this);

  GraphVertex* gvertex1 =
      (GraphVertex*)this->vec->find(vertex1, this->compare_vertices);
  GraphVertex* gvertex2 =
      (GraphVertex*)this->vec->find(vertex2, this->compare_vertices);

  GraphVertexPair* pair = new GraphVertexPair(this, gvertex1, gvertex2);

  gvertex1->removeNeighbor(pair, (CompareFunc)compareGraphVertexPairs);
  gvertex2->removeReverse(pair, (CompareFunc)compareGraphVertexPairs);

  delete pair;
  delete vertex1;
  delete vertex2;
}

Vector* Graph::getAdjacentV(Pointer vertex) {
  GraphVertex* gvertex = (GraphVertex*)vertex;

  PQueue* pqueue = gvertex->getNeighbors();

  return pqueue->toVector();
}

Vector* Graph::getReverseAdjacentV(Pointer vertex) {
  GraphVertex* gvertex = (GraphVertex*)vertex;

  PQueue* pqueue = gvertex->getReverse();

  return pqueue->toVector();
}

void destroyEdgePair(GraphVertexPair* pair) {
  delete pair;
}

/**
 * @brief Returns a vector where the K first elements are the direct neighbors
 * of the vertex and the rest are the reverse ones.
 *
 * @param vertex
 * @return Vector*
 */
Vector* Graph::getGeneralNeighborsV(Pointer vertex) {
  Vector* direct = getAdjacentV(vertex);
  Vector* reverse = getReverseAdjacentV(vertex);

  Vector* neighbors = new Vector(0, nullptr);

  for (int i = 0; i < direct->getSize(); i++) {
    GraphVertexPair* pair = (GraphVertexPair*)direct->getAt(i);
    neighbors->insertLast(pair);
  }
  for (int i = 0; i < reverse->getSize(); i++) {
    GraphVertexPair* pair = (GraphVertexPair*)reverse->getAt(i);
    neighbors->insertLast(pair);
  }

  return neighbors;
}

void swap(Pointer p, Pointer q) {
  Pointer tmp = p;
  p = q;
  q = tmp;
}

bool Graph::isNeighbor(Pointer v1, Pointer v2) {
  // GraphVertex* vertex1 = new GraphVertex(v1, this);
  // GraphVertex* vertex2 = new GraphVertex(v2, this);

  // GraphVertex* gvertex1 =
  //     (GraphVertex*)this->vec->find(vertex1, this->compare_vertices);
  // GraphVertex* gvertex2 =
  //     (GraphVertex*)this->vec->find(vertex2, this->compare_vertices);

  GraphVertex* gvertex1 = (GraphVertex*)v1;
  GraphVertex* gvertex2 = (GraphVertex*)v2;

  bool alreadyMember = false;

  GraphVertexPair* pair = new GraphVertexPair(this, gvertex1, gvertex2);

  if (gvertex1->getNeighbors()->find(pair, (CompareFunc)compareNeighbors) != -1)
    alreadyMember = true;

  delete pair;
  // delete vertex1;
  // delete vertex2;

  return alreadyMember;
}

bool Graph::isNeighborVertex(Pointer v1, Pointer v2) {
  GraphVertex* gvertex1 = (GraphVertex*)v1;
  GraphVertex* gvertex2 = (GraphVertex*)v2;

  bool alreadyMember = false;

  GraphVertexPair* pair = new GraphVertexPair(this, gvertex1, gvertex2);

  if (gvertex1->getNeighbors()->toVector()->binaryFind(
          pair, (CompareFunc)compareGraphVertexPairs) != nullptr)
    alreadyMember = true;

  delete pair;

  return alreadyMember;
}

Graph::~Graph() {
  delete this->vec;
}

GraphVertex::GraphVertex(Pointer data, Graph* owner)
    : data(data), owner(owner), hasBeenChecked(false) {
  neighbors = new PQueue(compareNeighbors, nullptr, nullptr);
  reverse = new PQueue(compareNeighbors, (DestroyFunc)destroyEdgePair, nullptr);
}

GraphVertex::~GraphVertex() {
  delete neighbors;
  delete reverse;
}