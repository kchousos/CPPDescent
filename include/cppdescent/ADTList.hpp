/**
 * @file linkedList.hpp
 * @author Konstantinos Chousos
 * @brief Header file for an ADT List implementation.
 * @version 0.1
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "common.hpp"

#define LIST_BOF (ListNode*)0  ///< Virtual node before the first node.
#define LIST_EOF (ListNode*)0  ///< Virtual node after the last node.

/**
 * @brief Node of a list.
 *
 * A list node is represented by its value. It also holds a pointer
 * to the next node in the list.
 *
 */
class ListNode {
 public:
  /**
   * @brief Construct a new List Node object that is empty.
   *
   */
  ListNode() : next(nullptr), value(nullptr){};
  /**
   * @brief Construct a new List Node object with value 'value'.
   *
   * @param value a generic pointer to the value of the node.
   */
  ListNode(Pointer value) : next(nullptr), value(value){};
  /**
   * @brief Destroy the List Node object
   *
   */
  // ~ListNode();
  /**
   * @brief Setter for the next node.
   *
   * @param next The pointer to the next node.
   */
  void setNext(ListNode* next);
  /**
   * @brief Getter for the next node.
   *
   * @return ListNode* The pointer to the next node.
   */
  ListNode* getNext();
  /**
   * @brief Getter for the value of the node.
   *
   * @return Pointer A generic pointer to the value.
   */
  Pointer getValue();

 private:
  ListNode* next;
  Pointer value;
};

/**
 * @brief ADT List
 *
 * An Abstract Data Type List with linear access to data, and addition
 * and removal to/from arbitrary positions.
 *
 */
class List {
 public:
  /**
   * @brief Construct a new List object.
   *
   * @param destroyValue If destroyValue != nullptr, the destroyValue
   * function will be called each time a node is removed.
   */
  List(DestroyFunc destroyValue = nullptr);
  /**
   * @brief Destroy the List object.
   *
   */
  ~List();
  /**
   * @brief Adds a node *after* the given node.
   *
   * @param node The node that the new node will be placed after.
   * @param value The value of the new node.
   */
  void insertNext(ListNode* node, Pointer value);
  /**
   * @brief Removes the node *after* the given node.
   *
   * @param node The node before the node to be removed.
   */
  void removeNext(ListNode* node);
  /**
   * @brief Finds the first value equal to the value parameter.
   *
   * @param value The value based on which the search is done.
   * @param compare A pointer to the function that will compare the node values.
   * @return Pointer A pointer to the resulting node.
   */
  Pointer find(Pointer value, CompareFunc compare);
  /**
   * @brief Set the Destroy Value object to a new value.
   *
   * @param value A pointer to the new destroy funciton.
   * @return DestroyFunc A pointer to the old destroy function.
   */
  DestroyFunc setDestroyValue(DestroyFunc value);
  /**
   * @brief Get the size of the List.
   *
   * The size is updated upon entry/removal of nodes, so the retrieval
   * of the size is O(1).
   *
   * @return int
   */
  int getSize();
  int increaseSize();
  int decreaseSize();
  /**
   * @brief Getter for the head of the List.
   *
   * @return ListNode* The head.
   */
  ListNode* getHead();
  /**
   * @brief Getter for the tail of the List.
   *
   * @return ListNode* The tail.
   */
  ListNode* getTail();
  /**
   * @brief Returns the node after the given one.
   *
   * @param node
   * @return ListNode* A pointer to the next node.
   */
  ListNode* next(ListNode* node);
  /**
   * @brief Returns the value of the node.
   *
   * @param node
   * @return Pointer A generic pointer to the value.
   */
  Pointer nodeValue(ListNode* node);
  /**
   * @brief Finds the first node that has value equal to the value parameter.
   *
   * @param value The value to search.
   * @param compare The compare function to use.
   * @return ListNode* A pointer to the resulting node.
   */
  ListNode* findNode(Pointer value, CompareFunc compare);

 private:
  ListNode* head;
  ListNode* tail;
  int size;
  DestroyFunc destroyValue;
};
