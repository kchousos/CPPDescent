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
