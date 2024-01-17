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
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <climits>
#include <iostream>
#include "cppdescent/cppdescent.hpp"

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

  float first =
      cppdescent::euclideanDistance(pair1->getVertex1(), pair1->getVertex2());
  float second =
      cppdescent::euclideanDistance(pair2->getVertex1(), pair2->getVertex2());

  float result = first - second;

  if (result < 0)
    return -1;
  else
    return 1;
}

void destroyVertex(GraphVertex* vertex) {
  gsl_vector_free((gsl_vector*)vertex->getData());

  delete vertex;
}

// Graph //

Graph::Graph(CompareFunc compare_data, DestroyFunc destroy_data)
    : size(0), compare_data(compare_data), destroy_data(destroy_data) {
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
  gsl_vector* x = (gsl_vector*)vertex;

  if (this->vec->find(gvertex, this->compare_vertices) != nullptr) {
    delete gvertex;
    return;
  }

  this->vec->insertLast(gvertex);
  this->size++;
  double norm;
  gsl_blas_ddot(x, x, &norm);
  gvertex->setNorm(norm);
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

  this->vec->swap(i, this->size - 1);
  this->vec->removeLast();

  this->size--;

  delete gvertex;
}

void Graph::insertEdge(Pointer data1, Pointer data2) {
  GraphVertex* gvertex1 = (GraphVertex*)data1;
  GraphVertex* gvertex2 = (GraphVertex*)data2;

  bool alreadyMember = false;

  GraphVertexPair* pair = new GraphVertexPair(this, gvertex1, gvertex2);

  if (gvertex1->getNeighbors()->find(
          pair, (CompareFunc)cppdescent::compareGraphVertexPairs) != -1) {
    alreadyMember = true;
    delete pair;
  }

  if (alreadyMember == false) {
    gvertex1->addNeighbor(pair);
    gvertex2->addReverse(pair);
  }
}

void Graph::removeEdge(Pointer data1, Pointer data2) {
  GraphVertex* vertex1 = (GraphVertex*)data1;
  GraphVertex* vertex2 = (GraphVertex*)data2;

  GraphVertexPair* pair = new GraphVertexPair(this, vertex1, vertex2);

  vertex1->removeNeighbor(pair,
                          (CompareFunc)cppdescent::compareGraphVertexPairs);
  vertex2->removeReverse(pair,
                         (CompareFunc)cppdescent::compareGraphVertexPairs);

  delete pair;
  // delete vertex1;
  // delete vertex2;
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

bool Graph::isNeighborVertex(Pointer v1, Pointer v2) {
  GraphVertex* gvertex1 = (GraphVertex*)v1;
  GraphVertex* gvertex2 = (GraphVertex*)v2;

  bool alreadyMember = false;

  GraphVertexPair* pair = new GraphVertexPair(this, gvertex1, gvertex2);

  if (gvertex1->getNeighbors()->toVector()->binaryFind(
          pair, (CompareFunc)cppdescent::compareGraphVertexPairs) != nullptr)
    alreadyMember = true;

  delete pair;

  return alreadyMember;
}

Graph::~Graph() {
  delete this->vec;
}

// GraphVertex

GraphVertex::GraphVertex(Pointer data, Graph* owner)
    : data(data), owner(owner), hasBeenChecked(false) {
  neighbors = new PQueue(compareNeighbors, nullptr, nullptr);
  reverse = new PQueue(compareNeighbors, (DestroyFunc)destroyEdgePair, nullptr);
}

GraphVertex::~GraphVertex() {
  delete neighbors;
  delete reverse;
}