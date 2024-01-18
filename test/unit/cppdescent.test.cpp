/**
 * @file cppdescent.test.cpp
 * @author Konstantinos Chousos, Phaedon Seitanidis
 * @brief Unit tests for the different uses of the cppdescent library.
 * @version 0.1
 * @date 2023-11-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cppdescent/cppdescent.hpp"
#include <gsl/gsl_vector.h>
#include <cmath>
#include "gtest/gtest.h"

#define delta 0.001
#define rho 0.5

TEST(HelperFunctions, compareGraphVertices) {
  gsl_vector* v1 = gsl_vector_alloc(10);
  gsl_vector* v2 = gsl_vector_alloc(10);

  Graph* graph = new Graph(nullptr, nullptr);

  graph->insertVertex(v1);
  graph->insertVertex(v2);

  Vector* vec = graph->getVec();
  GraphVertex* g1 = (GraphVertex*)vec->getAt(0);
  GraphVertex* g2 = (GraphVertex*)vec->getAt(1);

  ASSERT_EQ(cppdescent::compareGraphVertices(g1, g1), 0);
  ASSERT_EQ(cppdescent::compareGraphVertices(g1, g2), 1);

  delete graph;
}

TEST(IO, readData) {
  Vector* vec = cppdescent::readBinData("./datasets/00000020.bin", 100);
  ASSERT_NE(vec, nullptr);

  float lastValue = 0.0726192221;

  gsl_vector* v = (gsl_vector*)vec->getAt(vec->getSize() - 1);
  ASSERT_FLOAT_EQ(lastValue, gsl_vector_get(v, 99));

  delete vec;
}