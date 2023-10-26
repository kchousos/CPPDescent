#include "linkedList.hpp"

List::List(DestroyFunc destroyValue)
    : destroyValue(destroyValue), size(0), tail(dummy) {
    this->dummy = new ListNode();
    this->dummy->setNext(nullptr);
}

// TODO: Πώς θα χρησιμοποιείται το destroyValue;
List::~List() {
    ListNode* node = this->head;
    while (node != nullptr) {
        ListNode* next = node->getNext();
        delete node;
        node = next;
    }
}

int List::increaseSize() { return this->size++; }
int List::decreaseSize() { return this->size--; }

ListNode* List::getHead() { return this->head; }
ListNode* List::getTail() { return this->tail; }

int List::getSize() { return this->size; }

void List::insertNext(ListNode* node, Pointer value) {
    if (node == nullptr) node = this->dummy;

    ListNode* newNode = new ListNode(value);

    newNode->setNext(node->getNext());
    node->setNext(newNode);

    this->decreaseSize();

    // FIXME: Αυτό το σημείο μάλλον είναι λάθος, kανονιkά θα πρέπει να παίζει
    // με getters kαι setters.
    if (this->tail == node) this->tail = newNode;
}

void List::removeNext(ListNode* node) {
    if (node == nullptr) node = this->dummy;

    ListNode* removed = node->getNext();

    // FIXME
    // Αυτήν τη στιγμή δεν παίζει να το ψάξω, αλλά αυτό θέλει kοίταγμα.
    if (this->destroyValue != nullptr) this->destroyValue(removed);

    node->setNext(removed->getNext());

    this->decreaseSize();
    // FIXME: Αυτό το σημείο μάλλον είναι λάθος, kανονιkά θα πρέπει να παίζει
    // με getters kαι setters.
    if (this->tail == removed) this->tail = node;

    // delete removed;
}