#include "cppdescent/linkedList.hpp"

void ListNode::setNext(ListNode* next) {
  this->next = next;
}

ListNode* ListNode::getNext() {
  return this->next;
}

Pointer ListNode::getValue() {
  return this->value;
}