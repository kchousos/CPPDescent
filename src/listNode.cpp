/**
 * @file listNode.cpp
 * @author Konstantinos Chousos
 * @brief Implementation of the listNode to be used with linkedList.
 * @version 0.1
 * @date 2023-10-29
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "cppdescent/ADTList.hpp"

void ListNode::setNext(ListNode* next) {
  this->next = next;
}

ListNode* ListNode::getNext() {
  return this->next;
}

Pointer ListNode::getValue() {
  return this->value;
}