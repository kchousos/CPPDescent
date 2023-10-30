/**
 * @file ADTPQueue.cpp
 * @author Konstantinos Chousos
 * @version 0.1
 * @date 2023-10-30
 *
 * @brief An ADT Priority Queue implemented using a heap.
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "cppdescent/ADTPQueue.hpp"
#include "cppdescent/ADTVector.hpp"

// Helper functions for abstraction when accessing nodes.

Pointer PQueue::nodeValue(int nodeId) {
  return this->vector->getAt(nodeId - 1);
}

void PQueue::nodeSwap(int nodeId1, int nodeId2) {
  Pointer v1 = nodeValue(nodeId1);
  Pointer v2 = nodeValue(nodeId2);

  this->vector->setAt(nodeId1 - 1, v2);
  this->vector->setAt(nodeId2 - 1, v1);
}

// Keeps the heap property.
void PQueue::bubbleUp(int nodeId) {
  if (nodeId == 1)
    return;

  int parent = nodeId / 2;

  if (this->compare(nodeValue(parent), nodeValue(nodeId)) < 0) {
    nodeSwap(parent, nodeId);
    bubbleUp(parent);
  }
}

// Keeps the heap property.
void PQueue::bubbleDown(int nodeId) {
  int lchild = 2 * nodeId;
  int rchild = lchild + 1;

  int size = this->getSize();
  if (lchild > size)
    return;

  int maxChild = lchild;
  if (rchild <= size && this->compare(nodeValue(lchild), nodeValue(rchild)) < 0)
    maxChild = rchild;

  if (this->compare(nodeValue(nodeId), nodeValue(maxChild)) < 0) {
    nodeSwap(nodeId, maxChild);
    bubbleDown(maxChild);
  }
}

// Initializes the queue with the vector values.
void PQueue::naiveHeapify(Vector* values) {
  // TODO: there is a more optimal way
  int size = values->getSize();
  for (int i = 0; i < size; i++)
    this->insert(values->getAt(i));
}