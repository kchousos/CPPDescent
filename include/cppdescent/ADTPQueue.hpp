/**
 * @file ADTPQueue.hpp
 * @author Konstantinos Chousos
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "ADTVector.hpp"

/**
 * @brief ADT Priority Queue.
 *
 */
class PQueue {
 public:
  /**
   * @brief Construct a new PQueue object.
   *
   * Uses the `compare` function to compare its elements.
   *
   * If `destroyValue` != nullptr, the `destroyValue` function is called
   * upon removal of an element.
   *
   * If `values` != nullptr, then the priority queue will be initialized
   * with the values of the vector `values`.
   *
   * @param compare
   * @param destroyValue
   * @param values
   */
  PQueue(CompareFunc compare, DestroyFunc destroyValue, Vector* values);
  /**
   * @brief Destroy the PQueue object.
   *
   */
  ~PQueue();
  /**
   * @brief Get the size of the priority queue.
   *
   * @return int
   */
  int getSize();
  /**
   * @brief Get the max element of the queue.
   *
   * @return Pointer A generic pointer to the max element.
   */
  Pointer getMax();
  /**
   * @brief Get the min element of the queue.
   *
   * @return Pointer A generic pointer to the min element.
   */
  Pointer getMin();
  /**
   * @brief Insert a new element to the queue.
   *
   * @param value The value of the new element to be inserted.
   */
  void insert(Pointer value);
  /**
   * @brief Remove the max of the queue.
   *
   */
  void removeMax();
  /**
   * @brief Set the Destroy Value object.
   *
   * @param destroyValue
   * @return DestroyFunc
   */
  DestroyFunc setDestroyValue(DestroyFunc destroyValue);

  // Helper functions
  // These are used because the node IDs are 1-based, where as the
  // vector is 0-based.
  Pointer nodeValue(int nodeId);
  void nodeSwap(int nodeId1, int nodeId2);
  void bubbleUp(int nodeId);
  void bubbleDown(int nodeId);
  void naiveHeapify(Vector* values);

 private:
  Vector* vector;
  CompareFunc compare;
  DestroyFunc destroyValue;
};