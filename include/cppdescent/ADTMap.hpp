/**
 * @file ADTMap.hpp
 * @author Phaedon Seitanidis
 * @brief
 * @version 0.1
 * @date 2023-11-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "common.hpp"

#define MAX_LOAD_FACTOR 0.5
#define MAP_EOF (MapNode*)0

typedef enum { EMPTY, OCCUPIED, DELETED } State;

class MapNode {
 public:
  MapNode() : key(nullptr), value(nullptr), state(EMPTY){};
  MapNode(Pointer key, Pointer value, State state)
      : key(key), value(value), state(state){};
  ~MapNode(){};
  void setKey(Pointer key) { this->key = key; };
  void setValue(Pointer value) { this->value = value; };
  void setState(State state) { this->state = state; };
  Pointer getKey() { return this->key; };
  Pointer getValue() { return this->value; };
  State getState() { return this->state; };

 private:
  Pointer key;
  Pointer value;
  State state;
};

class Map {
 public:
  Map();
  Map(CompareFunc compare, DestroyFunc destroyKey, DestroyFunc destroyValue);
  ~Map();
  void insert(Pointer key, Pointer value);
  bool remove(Pointer key);
  Pointer find(Pointer key);
  void rehash();
  void setCapacity(int capacity) { this->capacity = capacity; };
  void setDestroyKey(DestroyFunc destroyKey) { this->destroyKey = destroyKey; };
  void setDestroyValue(DestroyFunc destroyValue) {
    this->destroyValue = destroyValue;
  };
  void setHashFunction(HashFunc hash) { this->hash = hash; };
  MapNode** getArray() { return this->array; };
  int getCapacity() { return this->capacity; };
  int getSize() { return this->size; };
  int getDeleted() { return this->deleted; };
  DestroyFunc getDestroyKey() { return this->destroyKey; };
  DestroyFunc getDestroyValue() { return this->destroyValue; };
  HashFunc getHashFunction() { return this->hash; };
  MapNode* getFirst();
  MapNode* getNext(MapNode* node);
  MapNode* findNode(Pointer key);

 private:
  MapNode** array;
  CompareFunc compare;
  DestroyFunc destroyKey;
  DestroyFunc destroyValue;
  HashFunc hash;
  int capacity;
  int size;
  int deleted;
};