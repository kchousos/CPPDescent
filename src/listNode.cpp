#include "linkedList.hpp"

ListNode::ListNode() {
  this->next = nullptr;
  this->value = nullptr;
}

ListNode::ListNode(void* value) {
  this->next = nullptr;
}

ListNode::~ListNode() {
  this->next = nullptr;
  this->value = nullptr;
}

int ListNode::setNext(ListNode* next) {
  this->next = next;
  return 0;
}

ListNode* ListNode::getNext() {
  return this->next;
}

Pointer ListNode::getValue() {
  return this->value;
}