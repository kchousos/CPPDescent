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
#include "cppdescent/cppdescent.hpp"

float* createFloat(float value) {
  float* p = new float;
  *p = value;
  return p;
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

  return v1->getOwner()->getCompareData()(v1->getData(), v2->getData());
}

int compareNeighbors(Pointer neighbor1, Pointer neighbor2) {
  GraphVertexPair* pair1 = ((Neighbor*)neighbor1)->getPair();
  GraphVertexPair* pair2 = ((Neighbor*)neighbor2)->getPair();

  float first = cppdescent::euclideanDistance(
      ((GraphVertex*)pair1->getVertex1())->getData(),
      ((GraphVertex*)pair1->getVertex2())->getData());
  float second = cppdescent::euclideanDistance(
      ((GraphVertex*)pair2->getVertex1())->getData(),
      ((GraphVertex*)pair2->getVertex2())->getData());

  // float first = pair1->getOwner()->getWeight(
  //     ((GraphVertex*)pair1->getVertex1())->getData(),
  //     ((GraphVertex*)pair1->getVertex2())->getData());
  // float second = pair2->getOwner()->getWeight(
  //     ((GraphVertex*)pair2->getVertex1())->getData(),
  //     ((GraphVertex*)pair2->getVertex2())->getData());

  float result = first - second;

  if (result < 0)
    return -1;
  else if (result > 0)
    return 1;
  else
    return 0;
}

int compareNeighborsBin(Pointer neighbor1, Pointer neighbor2) {
  GraphVertexPair* pair1 = ((Neighbor*)neighbor1)->getPair();
  GraphVertexPair* pair2 = ((Neighbor*)neighbor2)->getPair();

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
  GraphVertex* vertex1 = new GraphVertex(data1, this);
  GraphVertex* vertex2 = new GraphVertex(data2, this);

  GraphVertex* gvertex1 =
      (GraphVertex*)this->vec->find(vertex1, this->compare_vertices);
  GraphVertex* gvertex2 =
      (GraphVertex*)this->vec->find(vertex2, this->compare_vertices);

  if (gvertex1 == nullptr || gvertex2 == nullptr)
    return;  // LCOV_EXCL_LINE

  bool alreadyMember = false;

  GraphVertexPair* neighborPair = new GraphVertexPair(this, gvertex1, gvertex2);
  Neighbor* neighbor = new Neighbor(neighborPair);

  if (gvertex1->getNeighbors()->find(neighbor,
                                     (CompareFunc)compareNeighborsBin) != -1 &&
      gvertex2->getReverse()->find(neighbor,
                                   (CompareFunc)compareNeighborsBin) != -1)
    alreadyMember = true;

  if (alreadyMember == false) {
    gvertex1->addNeighbor(neighbor);
    gvertex2->addReverse(neighbor);
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

  GraphVertexPair* neighborPair = new GraphVertexPair(this, gvertex1, gvertex2);
  Neighbor* neighbor = new Neighbor(neighborPair);

  gvertex1->removeNeighbor(neighbor, (CompareFunc)compareNeighborsBin);
  gvertex2->removeReverse(neighbor, (CompareFunc)compareNeighborsBin);

  delete neighborPair;
  delete neighbor;
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
    Neighbor* neighbor = (Neighbor*)direct->getAt(i);
    neighbors->insertLast(neighbor);
  }
  for (int i = 0; i < reverse->getSize(); i++) {
    Neighbor* neighbor = (Neighbor*)reverse->getAt(i);
    neighbors->insertLast(neighbor);
  }

  return neighbors;
}

void swap(Pointer p, Pointer q) {
  Pointer tmp = p;
  p = q;
  q = tmp;
}

bool Graph::isNeighbor(Pointer v1, Pointer v2) {
  GraphVertex* vertex1 = new GraphVertex(v1, this);
  GraphVertex* vertex2 = new GraphVertex(v2, this);

  GraphVertex* gvertex1 =
      (GraphVertex*)this->vec->find(vertex1, this->compare_vertices);
  GraphVertex* gvertex2 =
      (GraphVertex*)this->vec->find(vertex2, this->compare_vertices);

  bool alreadyMember = false;

  GraphVertexPair* neighborPair = new GraphVertexPair(this, gvertex1, gvertex2);
  Neighbor* neighbor = new Neighbor(neighborPair);

  if (gvertex1->getNeighbors()->find(neighbor,
                                     (CompareFunc)compareNeighborsBin) != -1 ||
      gvertex2->getReverse()->find(neighbor,
                                   (CompareFunc)compareNeighborsBin) != -1)
    alreadyMember = true;

  delete neighborPair;
  delete neighbor;
  delete vertex1;
  delete vertex2;

  return alreadyMember;
}

Graph::~Graph() {
  delete this->vec;
}

// TODO
void destroyNeighbor(Pointer neighbor) {}

GraphVertex::GraphVertex(Pointer data, Graph* owner)
    : data(data), owner(owner), hasBeenChecked(false) {
  neighbors = new PQueue(compareNeighbors, destroyNeighbor, nullptr);
  reverse = new PQueue(compareNeighbors, destroyNeighbor, nullptr);
}

GraphVertex::~GraphVertex() {
  delete neighbors;
  delete reverse;
}