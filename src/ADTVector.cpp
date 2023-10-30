/**
 * @file ADTVector.cpp
 * @author Konstantinos Chousos
 * @brief Implementation of ADTVector using a dynamic array.
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "cppdescent/ADTVector.hpp"
#include "stdlib.h"

Vector::Vector(int size, DestroyFunc destroyValue)
    : size(size), destroyValue(destroyValue) {
  this->capacity = size < VECTOR_MIN_CAPACITY ? VECTOR_MIN_CAPACITY : size;
  // `calloc` is used instead of `new` so we can later call `realloc`.
  this->array = (vectorNode*)calloc(this->capacity, sizeof(*this->array));
}

Vector::~Vector() {
  if (this->destroyValue != nullptr) {
    vectorNode* node = this->first();
    while (node != nullptr) {
      this->destroyValue(node->getValue());
      node = this->next(node);
    }
  }

  // `free` is used because the array was initialzed with `calloc`.
  free(this->array);
}

int Vector::getSize() {
  return this->size;
}

void Vector::insertLast(Pointer value) {
  /// If we are at the last element, we create a new array double the size.
  if (this->capacity == this->size) {
    this->capacity *= 2;
    this->array = (vectorNode*)realloc(this->array,
                                       this->capacity * sizeof(*this->array));
  }

  this->array[this->size].setValue(value);
  this->size++;
}

int Vector::removeLast() {
  if (this->size == 0)
    return -1;

  if (this->destroyValue != nullptr)
    this->destroyValue(this->array[this->size - 1].getValue());

  this->size--;

  // if the used part of the array is too small, decrease its size.
  if (this->capacity > this->size * 4 &&
      this->capacity > 2 * VECTOR_MIN_CAPACITY) {
    this->capacity /= 2;
    this->array = (vectorNode*)realloc(this->array,
                                       this->capacity * sizeof(*this->array));
  }
  return 0;
}

Pointer Vector::getAt(int index) {
  if (index < 0 || index >= this->size)
    return nullptr;

  return this->array[index].getValue();
}

int Vector::setAt(int index, Pointer value) {
  if (index < 0 || index >= this->size)
    return -1;

  if (value != this->array[index].getValue() && this->destroyValue != nullptr)
    this->destroyValue(this->array[index].getValue());

  this->array[index].setValue(value);
  return 0;
}

Pointer Vector::find(Pointer value, CompareFunc compare) {
  for (int i = 0; i < this->size; i++)
    if (compare(this->array[i].getValue(), value) == 0)
      return this->array[i].getValue();

  return nullptr;
}

DestroyFunc Vector::setDestroyValue(DestroyFunc destroyValue) {
  DestroyFunc old = this->destroyValue;
  this->destroyValue = destroyValue;
  return old;
}

vectorNode* Vector::first() {
  if (this->size == 0)
    return VECTOR_BOF;
  else
    return &this->array[0];
}

vectorNode* Vector::last() {
  if (this->size == 0)
    return VECTOR_BOF;
  else
    return &this->array[this->size - 1];
}

vectorNode* Vector::next(vectorNode* node) {
  if (node == &this->array[this->size - 1])
    return VECTOR_EOF;
  else
    return node + 1;
}

vectorNode* Vector::previous(vectorNode* node) {
  if (node == &this->array[0])
    return VECTOR_EOF;
  else
    return node - 1;
}

Pointer Vector::nodeValue(vectorNode* node) {
  return node->getValue();
}

vectorNode* Vector::findNode(Pointer value, CompareFunc compare) {
  for (int i = 0; i < this->size; i++)
    if (compare(this->array[i].getValue(), value) == 0)
      return &this->array[i];  // found

  return VECTOR_EOF;  // not found
}