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
#include "gtest/gtest.h"

TEST(IO, readData) {
  Vector* vec = cppdescent::readBinData("./datasets/00000020.bin");
  ASSERT_NE(vec, nullptr);

  float lastValue = 0.0726192221;

  Vector* lastElement = (Vector*)vec->getAt(vec->getSize() - 1);
  ASSERT_EQ(lastValue, *(float*)lastElement->getAt(lastElement->getSize() - 1));

  int result = cppdescent::deleteDatapointVectors(vec);
  ASSERT_EQ(result, 0);
}