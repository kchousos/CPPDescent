/**
 * @file ADTVector.hpp
 * @author Konstantinos Chousos
 * @brief ADTVector implementation using a dynamic array.
 * @version 0.1
 * @date 2023-10-29
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "common.hpp"

#define VECTOR_BOF (vectorNode*)0
#define VECTOR_EOF (vectorNode*)0

/**
 * @brief Class for the vector Node object.
 *
 * A vector node is described only its value.
 *
 */
class vectorNode {
 public:
  /**
   * @brief Construct a new vector Node object.
   *
   * @param value A Pointer to the value.
   */
  vectorNode(Pointer value) : value(value){};
  /**
   * @brief Get the Value object.
   *
   * @return Pointer
   */
  Pointer getValue() const { return value; };
  /**
   * @brief Set the Value object.
   *
   * @param value
   */
  void setValue(Pointer value) { this->value = value; };

 private:
  Pointer value;
};

class Vector {
 public:
  /**
   * @brief Construct a new Vector object.
   *
   * The newly created Vector will be of size size and its elements
   * will be initialized as nullptr.
   *
   * If destroyValue is not nullptr, it will be called on each element
   * when it is removed from the Vector.
   *
   * @param size The initial size of the Vector.
   * @param destroyValue The function to be called on each element when it is
   * removed from the Vector.
   */
  Vector(int size, DestroyFunc destroyValue);
  /**
   * @brief Destroy the Vector object.
   *
   * Deletes all allocated memory of the Vector.
   *
   */
  ~Vector();
  /**
   * @brief Get the current size of the Vector.
   *
   * @return int The size of the Vector.
   */
  int getSize();
  /**
   * @brief Inserts a new element at the end of the vector.
   *
   * @param value The value of the new element.
   */
  void insertLast(Pointer value);
  /**
   * @brief Removes the last element of the vector.
   *
   */
  void removeLast();
  /**
   * @brief Get the value at the specified position of the vector.
   *
   * @param pos The position to look for.
   * @return Pointer A pointer to the value at the specified position.
   */
  Pointer getAt(int pos);
  /**
   * @brief Set the value at the specified position of the vector.
   *
   * @param pos The position to edit.
   * @param value The new value.
   */
  void setAt(int pos, Pointer value);
  /**
   * @brief Find the first element with value equal to value.
   *
   * @param value The value to look for.
   * @param compare The function to be used for comparison.
   * @return Pointer A pointer to the found value.
   */
  Pointer find(Pointer value, CompareFunc compare);
  /**
   * @brief Set the Destroy Value.
   *
   * @param destroyValue The new destroy value function.
   * @return DestroyFunc The old destroy value function.
   */
  DestroyFunc setDestroyValue(DestroyFunc destroyValue);
  /**
   * @brief Get the first node of the vector.
   *
   * @return vectorNode
   */
  vectorNode* first();
  /**
   * @brief Get the last node of the vector.
   *
   * @return vectorNode
   */
  vectorNode* last();
  /**
   * @brief Get the next node of the vector, after the one given.
   *
   * @param node The node to get the next of.
   * @return vectorNode
   */
  vectorNode* next(vectorNode* node);
  /**
   * @brief Get the previous node of the vector, before the one given.
   *
   * @param node The node to get the previous of.
   * @return vectorNode
   */
  vectorNode* previous(vectorNode* node);
  /**
   * @brief Get the value of the node.
   *
   * @param node
   * @return Pointer
   */
  Pointer nodeValue(vectorNode* node);
  /**
   * @brief Find the first node with value equal to value.
   *
   * @param value The value to look for.
   * @param compare The function to be used for comparison.
   * @return vectorNode The resulting node.
   */
  vectorNode* findNode(Pointer value, CompareFunc compare);

 private:
  vectorNode* array;
  int size;
  int capacity;
  DestroyFunc destroyValue;
};