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
List::List(DestroyFunc value) : destroyValue(value), size(0) {
  this->head = new ListNode();
  this->tail = head;
}

/**
 * @brief Destroy the List object.
 *
 * If the destroyValue function is not nullptr, it is called for every value.
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

/**
 * @brief Increase the list size.
 *
 * @return int The updated size.
 */
int List::increaseSize() {
  return this->size++;
}

/**
 * @brief Decrease the list size.
 *
 * @return int The updated size.
 */
int List::decreaseSize() {
  return this->size--;
}

/**
 * @brief Get the first element of the List.
 *
 * @return ListNode* Pointer to the first element.
 */
ListNode* List::getHead() {
  return this->head->getNext();
}

/**
 * @brief Get the last element of the list.
 *
 * If the list is empty, the virtual head is the tail, so we return nullptr.
 *
 * @return ListNode* Pointer to the last element.
 */
ListNode* List::getTail() {
  if (this->size == 0)
    return nullptr;
  return this->tail;
}

/**
 * @brief Get the size of the list.
 *
 * @return int The size.
 */
int List::getSize() {
  return this->size;
}

/**
 * @brief Insert a new element after the node.
 *
 * @param node The node after which the element is inserted.
 * @param value The value of the new element.
 */
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

/**
 * @brief Removes the node after  the given node.
 *
 * @param node The node after which the element is removed.
 */
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

/**
 * @brief Get the node's value.
 *
 * @param node
 * @return Pointer
 */
Pointer List::nodeValue(ListNode* node) {
  return node->getValue();
}

/**
 * @brief Get the next node.
 *
 * @param node
 * @return ListNode*
 */
ListNode* List::next(ListNode* node) {
  return node->getNext();
}

/**
 * @brief Find the first value equal to the value parameter.
 *
 * @param value The value to search for.
 * @param compare A pointer to the compare function.
 * @return Pointer A pointer to found value.
 */
Pointer List::find(Pointer value, CompareFunc compare) {
  ListNode* node = this->findNode(value, compare);
  return node == nullptr ? nullptr : node->getValue();
}

/**
 * @brief Find the first node with value equal to the value parameter.
 *
 * @param value The  value to search for.
 * @param compare A pointer to the compare function.
 * @return ListNode* A pointer to the found node.
 */
ListNode* List::findNode(Pointer value, CompareFunc compare) {
  for (ListNode* node = this->head->getNext(); node != nullptr;
       node = node->getNext()) {
    if (compare(node->getValue(), value) == 0)
      return node;
  }

  return nullptr;
}

/**
 * @brief Set the destroy value function.
 *
 * @param value The new destroy function.
 * @return DestroyFunc The old destroy function.
 */
DestroyFunc List::setDestroyValue(DestroyFunc value) {
  DestroyFunc oldValue = this->destroyValue;
  this->destroyValue = value;
  return oldValue;
}

/**
 * @brief Set the given node as the next one.
 *
 * @param next The node to be next.
 */
void ListNode::setNext(ListNode* next) {
  this->next = next;
}

/**
 * @brief Get the next node of the list.
 *
 * @return ListNode* The next node.
 */
ListNode* ListNode::getNext() {
  return this->next;
}

/**
 * @brief Get the value of the current node.
 *
 * @return Pointer The value of the current node.
 */
Pointer ListNode::getValue() {
  return this->value;
}

/**
 * @brief Concatenation of 2 lists (Result: list1->list2).
 *
 * @param list List to be merged with this.
 * @return int Returns 0 in case of success, -1 in any other case.
 */
int List::mergeLists(List* list) {
  if (list == nullptr || list->getHead() == LIST_BOF)
    return -1;

  ListNode* lnode = list->getHead();
  ListNode* node = this->tail;
  for (int i = 0; i < list->getSize(); i++) {
    this->insertNext(node, lnode->getValue());
    node = this->next(node);
    lnode = list->next(lnode);
  }

  return 0;
}