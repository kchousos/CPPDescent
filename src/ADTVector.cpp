#include "cppdescent/ADTVector.hpp"

Vector::Vector(int size, DestroyFunc destroyValue) {}

Vector::~Vector() {}

int Vector::getSize() {
  return 0;
}

void Vector::insertLast(Pointer value) {}

void Vector::removeLast() {}

Pointer Vector::getAt(int index) {
  return nullptr;
}

void Vector::setAt(int index, Pointer value) {}

Pointer Vector::find(Pointer value, CompareFunc compare) {
  return nullptr;
}

DestroyFunc Vector::setDestroyValue(DestroyFunc destroyValue) {
  return nullptr;
}

vectorNode* Vector::first() {
  return nullptr;
}

vectorNode* Vector::last() {
  return nullptr;
}

vectorNode* Vector::next(vectorNode* node) {
  return nullptr;
}

vectorNode* Vector::previous(vectorNode* node) {
  return nullptr;
}

Pointer Vector::nodeValue(vectorNode* node) {
  return nullptr;
}

vectorNode* Vector::findNode(Pointer value, CompareFunc compare) {
  return nullptr;
}