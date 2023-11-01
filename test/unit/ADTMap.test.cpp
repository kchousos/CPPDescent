/**
 * @file ADTMap.test.cpp
 * @author Phaedon Seitanidis
 * @brief Unit tests for ADT Map.
 * @version 0.1
 * @date 2023-10-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cppdescent/ADTMap.hpp"
#include "gtest/gtest.h"

/**
 * @brief A simple function to compare 2 integer pointers.
 *
 * @param a Int pointer 1
 * @param b Int pointer 2
 * @return int < 0 => a < b, > 0 => a > b, = 0 => a == b
 */
int compareInts(Pointer a, Pointer b) {
  return *(int*)a - *(int*)b;
}

/**
 * @brief A simple function to free the memory space of an int pointer.
 *
 * @param a The pointer to get deleted.
 */
void destroyInts(Pointer a) {
  delete (int*)a;
}

/**
 * @brief A simple hash function for int pointers.
 *
 * @param a The given pointer to be hashed.
 * @return unsigned int The int value of the given pointer.
 */
unsigned int hashInt(Pointer a) {
  return *(int*)a;
}

/**
 * @brief Create an int object in a new memory space with the given value.
 *
 * @param value The value of the created int object.
 * @return int* The created pointer p, where *p = value.
 */
int* createInt(int value) {
  int* p = new int;
  *p = value;
  return p;
}

/**
 * @brief Helper function to insert a key-value pair in a map and test.
 *
 * @param map
 * @param key
 * @param value
 */
void insertAndTest(Map* map, Pointer key, Pointer value) {
  map->insert(key, value);
  ASSERT_EQ(map->find(key), value);
}

/**
 * @brief Helper function for shuffling the array of values.
 *
 * @param array
 * @param n
 */
void shuffle(int* array[], int n) {
  for (int i = 0; i < n; i++) {
    int j = i + rand() / (RAND_MAX / (n - i) + 1);
    int* t = array[j];
    array[j] = array[i];
    array[i] = t;
  }
}

TEST(ADTMapTest, create) {
  Map* map = new Map;
  map->setHashFunction(hashInt);
  map->setDestroyKey(nullptr);
  map->setDestroyValue(nullptr);

  ASSERT_NE(map, nullptr);
  ASSERT_EQ(map->getSize(), 0) << map->getSize();

  delete map;
}

TEST(ADTMapTest, insert) {
  Map* map = new Map(compareInts, destroyInts, destroyInts);
  map->setHashFunction(hashInt);

  int N = 1000;
  int** keyArray = new int*[N];
  int** valueArray = new int*[N];

  for (int i = 0; i < N; i++) {
    keyArray[i] = createInt(i);
  }

  shuffle(keyArray, N);

  for (int i = 0; i < N; i++) {
    valueArray[i] = createInt(i);

    insertAndTest(map, keyArray[i], valueArray[i]);

    ASSERT_EQ(map->getSize(), (i + 1));
  }

  int* newKey = createInt(*keyArray[0]);
  int* newValue = createInt(99);

  insertAndTest(map, newKey, newValue);

  delete map;

  Map* map2 = new Map(compareInts, nullptr, nullptr);
  map2->setHashFunction(hashInt);

  int key1 = 0, key2 = 0;
  int value1 = 0, value2 = 0;

  insertAndTest(map2, &key1, &value1);
  insertAndTest(map2, &key1, &value2);
  insertAndTest(map2, &key2, &value2);

  delete map2;
  delete[] keyArray;
  delete[] valueArray;

  Map* map3 = new Map(compareInts, nullptr, nullptr);
  map3->setHashFunction(hashInt);

  key1 = 1;
  key2 = 54;

  map3->insert(&key1, &value1);

  map3->insert(&key2, &value1);
  ASSERT_EQ(map3->remove(&key1), true);
  map3->insert(&key2, &value2);
  ASSERT_EQ(map3->getSize(), 1);
  ASSERT_EQ(map3->remove(&key2), true);
  ASSERT_EQ(map3->find(&key2), nullptr);

  delete map3;
}

/**
 * @brief Construct a new TEST object
 *
 */
TEST(ADTMapTest, remove) {
  Map* map = new Map(compareInts, destroyInts, destroyInts);
  map->setHashFunction(hashInt);

  int N = 1000;
  int** keyArray = new int*[N];
  int** valueArray = new int*[N];

  for (int i = 0; i < N; i++) {
    keyArray[i] = createInt(i);
    valueArray[i] = createInt(i);

    map->insert(keyArray[i], valueArray[i]);
    if (i % (N / 20) == 0) {
      ASSERT_EQ(map->remove(keyArray[i]), true);
    }
  }

  int notExists = 2000;
  ASSERT_NE(map->remove(&notExists), true);

  for (int i = 0; i < N; i++)
    if (i % (N / 20) != 0) {
      ASSERT_EQ(map->remove(keyArray[i]), true);
    }

  int key1 = 100;
  ASSERT_NE(map->remove(&key1), true);
  delete map;

  map = new Map(compareInts, destroyInts, destroyInts);
  map->setHashFunction(hashInt);

  for (int i = 0; i < N; i++) {
    keyArray[i] = createInt(i);
    valueArray[i] = createInt(i);

    map->insert(keyArray[i], valueArray[i]);
    map->remove(keyArray[i]);

    ASSERT_EQ(map->getSize(), 0);
  }
  delete map;

  delete[] keyArray;
  delete[] valueArray;
}

TEST(ADTMapTest, find) {
  Map* map = new Map(compareInts, destroyInts, destroyInts);
  map->setHashFunction(hashInt);

  int N = 1000;
  int** keyArray = new int*[N];
  int** valueArray = new int*[N];

  for (int i = 0; i < N; i++) {
    keyArray[i] = createInt(i);
    valueArray[i] = createInt(i);

    map->insert(keyArray[i], valueArray[i]);
    MapNode* found = map->findNode(keyArray[i]);
    Pointer foundKey = found->getKey();
    Pointer foundVal = found->getValue();

    ASSERT_NE(found, MAP_EOF);
    ASSERT_EQ(foundKey, keyArray[i]);
    ASSERT_EQ(foundVal, valueArray[i]);
  }

  int notExists = 2000;
  ASSERT_EQ(map->findNode(&notExists), MAP_EOF);
  ASSERT_EQ(map->find(&notExists), nullptr);

  Map* map2 = new Map(compareInts, nullptr, nullptr);
  map2->setHashFunction(hashInt);

  int M = 53;
  for (int i = 0; i < M; i++) {
    map2->insert(keyArray[i], valueArray[i]);
    ASSERT_EQ(map2->remove(keyArray[i]), true);
  }
  ASSERT_EQ(map2->find(&keyArray[M]), nullptr);

  delete map;
  delete map2;

  delete[] keyArray;
  delete[] valueArray;
}

TEST(ADTMapTest, iterate) {
  Map* map = new Map(compareInts, destroyInts, destroyInts);
  map->setHashFunction(hashInt);
  ASSERT_EQ(map->getFirst(), MAP_EOF);
  int N = 1000;
  for (int i = 0; i < N; i++)
    map->insert(createInt(i), createInt(2 * i));

  bool* seen = new bool[N];
  for (int i = 0; i < N; i++)
    seen[i] = false;

  for (MapNode* node = map->getFirst(); node != MAP_EOF;
       node = map->getNext(node)) {
    int* key = (int*)node->getKey();
    int* value = (int*)node->getValue();

    ASSERT_TRUE(*key >= 0 && *key < N && seen[*key] == false);
    ASSERT_EQ(*value, 2 * *key);

    seen[*key] = true;
  }

  delete[] seen;
  delete map;
}
