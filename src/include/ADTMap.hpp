#pragma once

#include "common_types.hpp"

#define MAX_LOAD_FACTOR 0.5

typedef unsigned int (*HashFunc)(Pointer);
typedef enum { EMPTY, OCCUPIED, DELETED } State;

class MapNode {
 private:
  Pointer key;
  Pointer value;
  State state;

 public:
  MapNode(Pointer key, Pointer value, State state)
      : key(key), value(value), state(state){};
  ~MapNode(){};
  void setKey(Pointer key) { this->key = key; };
  void setValue(Pointer value) { this->value = value; };
  void setState(State state) { this->state = state; };
  Pointer getKey() { return this->key; };
  Pointer getValue() { return this->value; };
  State getState() { return this->state; };
};

class Map {
 private:
  MapNode** array;
  int capacity;
  int size;
  int deleted;
  CompareFunc compare;
  DestroyFunc destroyKey;
  DestroyFunc destroyValue;
  HashFunc hash;

 public:
  Map(CompareFunc compare, DestroyFunc destroyKey, DestroyFunc destroyValue);
  ~Map();
  void insert(Pointer key, Pointer value);
  bool remove(Pointer key);
  Pointer find(Pointer key) { return this->findNode(key); };
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
};