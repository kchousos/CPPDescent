#pragma once
#include "common.hpp"

class ListNode {
   public:
    ListNode();
    ListNode(Pointer value) : value(value){};
    ~ListNode();
    int setNext(ListNode* next);
    ListNode* getNext();
    Pointer getValue();

   private:
    ListNode* next;
    Pointer value;
};

class List {
   public:
    List(DestroyFunc destroyValue);
    ~List();
    int getSize();
    void insertNext(ListNode* node, Pointer value);
    void removeNext(ListNode* node);
    ListNode find();
    int setDestroyValue(int value);
    ListNode next(ListNode node);
    Pointer nodeValue(ListNode node);
    ListNode findNode(ListNode node);
    int increaseSize();
    int decreaseSize();
    ListNode* getHead();
    ListNode* getTail();

   private:
    ListNode* dummy;
    ListNode* head;
    ListNode* tail;
    int size;
    DestroyFunc destroyValue;
};
