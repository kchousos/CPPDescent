/**
 * @file ADTMap.cpp
 * @author Phaedon Seitanidis
 * @brief Implementation of ADTMap.
 * @version 0.1
 * @date 2023-10-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cppdescent/ADTMap.hpp"
#include <iostream>

/**
 * We want the capacity of the Hash Table to be a prime number, according
 * to the theory. So the following list consists of prime numbers that are
 * proven good capacity values for a Hash Table. When a rehash is needed, the
 * capacity of the new table will be chosen from this table. If more than
 * 1610612741 elements are needed, we double the capacity of the old table at
 * rehash.
 */
int prime_sizes[] = {
    53,        97,        193,       389,       769,       1543,     3079,
    6151,      12289,     24593,     49157,     98317,     196613,   393241,
    786433,    1572869,   3145739,   6291469,   12582917,  25165843, 50331653,
    100663319, 201326611, 402653189, 805306457, 1610612741};

Map::Map()
    : compare(nullptr),
      destroyKey(nullptr),
      destroyValue(nullptr),
      size(0),
      deleted(0) {
  this->capacity = prime_sizes[0];

  this->array = new MapNode*[this->capacity];
  for (int i = 0; i < this->capacity; i++)
    this->array[i] = new MapNode;
}

/**
 * @brief Construct a new Map:: Map object
 *
 * @param compare A compare function to keep the map sorted
 * @param destroyKey A destroy function for the key of every node
 * @param destroyValue A destroy function fro the balue of every node
 */
Map::Map(CompareFunc compare, DestroyFunc destroyKey, DestroyFunc destroyValue)
    : compare(compare),
      destroyKey(destroyKey),
      destroyValue(destroyValue),
      size(0),
      deleted(0) {
  this->capacity = prime_sizes[0];

  this->array = new MapNode*[this->capacity];
  for (int i = 0; i < this->capacity; i++)
    this->array[i] = new MapNode;
}

/**
 * @brief Destroy the Map:: Map object
 *
 */
Map::~Map() {
  for (int i = 0; i < this->capacity; i++) {
    if (this->array[i]->getState() == OCCUPIED) {
      if (this->destroyKey != nullptr)
        this->destroyKey(this->array[i]->getKey());
      if (this->destroyValue != nullptr)
        this->destroyValue(this->array[i]->getValue());
    }
  }

  for (int i = 0; i < this->capacity; i++)
    delete this->array[i];

  delete[] this->array;
}

/**
 * @brief Extend the hash table in case the load factor exceeds the predefined
 * threshold
 *
 * @param map The map whose hash table needs to be rehashed
 */
void Map::rehash() {
  // Keep the previous data
  int oldCapacity = this->capacity;
  MapNode** oldArray = this->array;

  // Find the new capacity from the prime numbers list
  int prime = sizeof(prime_sizes) / sizeof(int);  // array size
  for (int i = 0; i < prime; i++) {
    if (prime_sizes[i] > oldCapacity) {
      this->capacity = prime_sizes[i];
      break;
    }
  }
  // If we run out of primes, we double the capacity
  if (this->capacity == oldCapacity)
    this->capacity *= 2;  // LCOV_EXCL_LINE

  // Create a bigger hash table
  this->array = new MapNode*[this->capacity];
  for (int i = 0; i < this->capacity; i++)
    this->array[i] = new MapNode;

  // Place in the new array only the entries that have a value (get rid of the
  // deleted)
  this->size = 0;
  for (int i = 0; i < oldCapacity; i++)
    if (oldArray[i]->getState() == OCCUPIED)
      this->insert(oldArray[i]->getKey(), oldArray[i]->getValue());

  for (int i = 0; i < oldCapacity; i++)
    delete oldArray[i];
  // Delete old array so we do not have leaks
  delete[] oldArray;
}

/**
 * @brief Add a new pair (key --> value) in the map
 *
 * @param key
 * @param value
 */
void Map::insert(Pointer key, Pointer value) {
  bool alreadyMember = false;

  MapNode* node = nullptr;

  uint pos;
  for (pos = this->hash(key) %
             this->capacity;  // Starting from the position of the x hash value
       this->array[pos]->getState() !=
       EMPTY;  // If an empty cell is found, stopping the process
       pos = (pos + 1) %
             this->capacity) {  // linear probing (if the end of the array is
                                // met, we start from the begining again)

    if (this->array[pos]->getState() == DELETED) {
      // A DELETED position is found. A new pair can be added here only if the
      // key is new. We keep the position and keep searching, because the key
      // might be in one of the next cells
      if (node == nullptr)
        node = this->array[pos];

    } else if (this->compare(this->array[pos]->getKey(), key) == 0) {
      alreadyMember = true;
      node =
          this->array[pos];  // Key is found, the new pair is added here (even
                             // if a DELETED position was found previously)
      break;                 // There is no need to continue searching
    }
  }
  if (node ==
      nullptr)  // If an EMPTY cell is found, node will not have a value yet
    node = this->array[pos];

  if (alreadyMember == true) {
    // If a node gets replaced, it also get destroyed
    if (node->getKey() != key && this->destroyKey != nullptr)
      this->destroyKey(node->getKey());

    if (node->getValue() != value && this->destroyValue != nullptr)
      this->destroyValue(node->getValue());

  } else {
    // If the node is new, incresing the map size
    this->size++;

    // If we found a DELETED state, we change it to OCCUPIED
    if (node->getState() == DELETED)
      this->deleted--;
  }

  // Update the new node with the given values
  node->setState(OCCUPIED);
  node->setKey(key);
  node->setValue(value);

  // If after the insertion, the load factor threshold is surpassed we have to
  // rehash We take into account the DELETED nodes too, because they affect our
  // computations
  float loadFactor = (float)(this->size + this->deleted) / this->capacity;
  if (loadFactor > MAX_LOAD_FACTOR)
    this->rehash();
}

/**
 * @brief Remove from the map the pair (key --> value) that corresponds to the
 * given key
 *
 * @return true Τhe node with the given key was removed successfully
 * @return false Τhe node with the given key was not removed
 */
bool Map::remove(Pointer key) {
  MapNode* node = findNode(key);
  if (node == MAP_EOF)
    return false;

  // destroy
  if (this->destroyKey != nullptr)
    this->destroyKey(node->getKey());
  if (this->destroyValue != nullptr)
    this->destroyValue(node->getValue());

  // Set the state as DELETED, so not to stop the search but also make the
  // insert easier
  node->setState(DELETED);
  this->deleted++;
  this->size--;

  return true;
}

Pointer Map::find(Pointer key) {
  MapNode* node = this->findNode(key);
  if (node != nullptr)
    return node->getValue();

  return nullptr;
}

/**
 * @brief
 *
 * @return MapNode*
 */
MapNode* Map::getFirst() {
  // Begin the iteration from the first cell, until we find an OCCUPIED
  for (int i = 0; i < this->capacity; i++)
    if (this->array[i]->getState() == OCCUPIED)
      return this->array[i];

  return MAP_EOF;
}

/**
 * @brief
 *
 * @param node
 * @return MapNode*
 */
MapNode* Map::getNext(MapNode* node) {
  int pos;
  for (int i = 0; i < this->capacity; i++)
    if (this->array[i] == node)
      pos = i;

  for (int i = pos + 1; i < this->capacity; i++)
    if (this->array[i]->getState() == OCCUPIED)
      return this->array[i];

  return MAP_EOF;
}

/**
 * @brief
 *
 * @param key
 * @return MapNode*
 */
MapNode* Map::findNode(Pointer key) {
  // We traverse the array, starting from the position where the key hashes,
  // until we find a non EMPTY
  int count = 0;

  for (int pos = this->hash(key) % this->capacity;
       this->array[pos]->getState() != EMPTY;
       pos = (pos + 1) % this->capacity) {
    if (this->array[pos]->getState() == OCCUPIED &&
        this->compare(this->array[pos]->getKey(), key) == 0)
      return this->array[pos];

    count++;
    if (count == this->capacity)
      break;
  }

  return MAP_EOF;
}