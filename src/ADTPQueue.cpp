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
#include <iostream>
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

// Priority Queue functions

PQueue::PQueue(CompareFunc compare, DestroyFunc destroyValue, Vector* values)
    : compare(compare), destroyValue(destroyValue) {
  if (compare == nullptr) {
    std::cerr << "`compare` function cannot be NULL. Exiting...\n";
    return;
  }

  this->vector = new Vector(0, nullptr);

  if (values != nullptr)
    naiveHeapify(values);
}

PQueue::~PQueue() {
  this->vector->setDestroyValue(this->destroyValue);
}

int PQueue::getSize() {
  return this->vector->getSize();
}

Pointer PQueue::getMax() {
  return nodeValue(1);
}

void PQueue::insert(Pointer value) {
  this->vector->insertLast(value);
  // The newly added element might not adhere to the heap property, when
  // it is placed at the last position. So we call `bubbleUp` to reestablish
  // this property on the queue.
  bubbleUp(this->getSize());
}

void PQueue::removeMax() {
  int lastNode = this->getSize();
  if (lastNode == 0) {
    std::cerr << "removeMax: Queue is empty. Exiting...\n";
    return;
  }

  if (this->destroyValue != nullptr)
    this->destroyValue(this->getMax());

  nodeSwap(1, lastNode);
  this->vector->removeLast();

  // Reestablish the heap property.
  bubbleDown(1);
}

DestroyFunc PQueue::setDestroyValue(DestroyFunc destroyValue) {
  DestroyFunc old = this->destroyValue;
  this->destroyValue = destroyValue;
  return old;
}