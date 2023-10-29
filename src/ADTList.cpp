/**
 * @file ADTList.cpp
 * @author Konstantinos Chousos
 * @brief Implementation of an ADT linked list.
 * @version 0.1
 * @date 2023-10-29
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "cppdescent/ADTList.hpp"

/**
 * @brief Construct a new List object.
 *
 * head is a virtual node that is always present in the list.
 *
 * @param destroyValue The destroy function for the values of the list.
 * Defaults to nullptr.
 *
 */
List::List(DestroyFunc destroyValue)
    : destroyValue(destroyValue), size(0), tail(head) {
  this->head = new ListNode();
}

/**
 * @brief Destroy the List object.
 *
 */
List::~List() {
  ListNode* node = this->head;
  while (node != nullptr) {
    ListNode* next = node->getNext();
    if (destroyValue != nullptr)
      destroyValue(node->getValue());
    delete node;
    node = next;
  }
}

int List::increaseSize() {
  return this->size++;
}
int List::decreaseSize() {
  return this->size--;
}

ListNode* List::getHead() {
  return this->head->getNext();
}
ListNode* List::getTail() {
  return this->tail;
}

int List::getSize() {
  return this->size;
}

void List::insertNext(ListNode* node, Pointer value) {
  if (node == nullptr)
    node = this->head;

  ListNode* newNode = new ListNode(value);

  newNode->setNext(node->getNext());
  node->setNext(newNode);

  this->increaseSize();

  if (this->tail == node)
    this->tail = newNode;
}

void List::removeNext(ListNode* node) {
  if (node == nullptr)
    node = this->head;

  ListNode* removed = node->getNext();

  if (this->destroyValue != nullptr)
    this->destroyValue(removed->getValue());

  node->setNext(removed->getNext());

  delete removed;

  this->decreaseSize();
  if (this->tail == removed)
    this->tail = node;
}

Pointer List::nodeValue(ListNode* node) {
  return node->getValue();
}

ListNode* List::next(ListNode* node) {
  return node->getNext();
}

Pointer List::find(Pointer value, CompareFunc compare) {
  ListNode* node = this->findNode(value, compare);
  return node == nullptr ? nullptr : node->getValue();
}

ListNode* List::findNode(Pointer value, CompareFunc compare) {
  for (ListNode* node = this->head->getNext(); node != nullptr;
       node = node->getNext()) {
    if (compare(node->getValue(), value) == 0)
      return node;
  }

  return nullptr;
}

DestroyFunc List::setDestroyValue(DestroyFunc value) {
  DestroyFunc oldValue = this->destroyValue;
  this->destroyValue = value;
  return oldValue;
}