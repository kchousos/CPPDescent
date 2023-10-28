#include "cppdescent/linkedList.hpp"

ListNode::ListNode() {
  this->next = nullptr;
  this->value = nullptr;
}

ListNode::ListNode(void* value) {
  this->next = nullptr;
  this->value = value;
}

// ListNode::~ListNode() {
//   this->next = nullptr;
//   this->value = nullptr;
// }

void ListNode::setNext(ListNode* next) {
  this->next = next;
}

ListNode* ListNode::getNext() {
  return this->next;
}

Pointer ListNode::getValue() {
  return this->value;
}