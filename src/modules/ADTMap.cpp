#include "../include/ADTMap.hpp"
#include <iostream>

// Το μέγεθος του Hash Table ιδανικά θέλουμε να είναι πρώτος αριθμός σύμφωνα με
// την θεωρία. Η παρακάτω λίστα περιέχει πρώτους οι οποίοι έχουν αποδεδιγμένα
// καλή συμπεριφορά ως μεγέθη. Κάθε re-hash θα γίνεται βάσει αυτής της λίστας.
// Αν χρειάζονται παραπάνω απο 1610612741 στοχεία, τότε σε καθε rehash
// διπλασιάζουμε το μέγεθος.
int prime_sizes[] = {
    53,        97,        193,       389,       769,       1543,     3079,
    6151,      12289,     24593,     49157,     98317,     196613,   393241,
    786433,    1572869,   3145739,   6291469,   12582917,  25165843, 50331653,
    100663319, 201326611, 402653189, 805306457, 1610612741};

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
    this->array[i] = new MapNode(nullptr, nullptr, EMPTY);
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
  int oldCapacity = this->getCapacity();
  MapNode** oldArray = this->getArray();

  // Find the new capacity from the prime numbers list
  int prime = sizeof(prime_sizes) / sizeof(int);  // array size
  for (int i = 0; i < prime; i++) {
    if (prime_sizes[i] > oldCapacity) {
      this->setCapacity(prime_sizes[i]);
      break;
    }
  }
  // If we run out of primes, we double the capacity
  if (this->getCapacity() == oldCapacity)
    this->setCapacity(this->getCapacity() * 2);

  // Create a bigger hash table
  this->array = new MapNode*[this->capacity];
  for (int i = 0; i < this->capacity; i++)
    this->array[i] = new MapNode(nullptr, nullptr, EMPTY);

  // Place in the new array only the entries that have a value (get rid of the
  // deleted)
  this->size = 0;
  for (int i = 0; i < oldCapacity; i++)
    if (oldArray[i]->getState() == OCCUPIED)
      this->insert(oldArray[i]->getKey(), oldArray[i]->getValue());

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

  int pos;
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

  // Σε αυτό το σημείο, το node είναι ο κόμβος στον οποίο θα γίνει εισαγωγή.
  if (alreadyMember == true) {
    // If a node gets replaced, it also get destroyed
    if (node->getKey() != key && this->getDestroyKey() != nullptr)
      this->destroyKey(node->getKey());

    if (node->getValue() != value && this->getDestroyValue() != nullptr)
      this->destroyValue(node->getValue());

  } else {
    // If the node is new, incresing the map size
    this->size++;

    if (node->getState() ==
        DELETED)  // If we found a DELETED state, we change it to OCCUPIED
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
  if (node == nullptr)
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

///////////////// Διάσχιση μέσω κόμβων //////////////////

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

  return nullptr;
}

/**
 * @brief
 *
 * @param node
 * @return MapNode*
 */
MapNode* Map::getNext(MapNode* node) {
  // node is a pointer to the i-th element of the array, so node - array[0] == i
  // (pointer arithmetic!)
  for (int i = node - this->array[0] + 1; i < this->capacity; i++)
    if (this->array[i]->getState() == OCCUPIED)
      return this->array[i];

  return nullptr;
}

/**
 * @brief
 *
 * @param key
 * @return MapNode*
 */
MapNode* Map::findNode(Pointer key) {
  // Διασχίζουμε τον πίνακα, ξεκινώντας από τη θέση που κάνει hash το key, και
  // για όσο δε βρίσκουμε EMPTY
  int count = 0;
  for (int pos =
           this->hash(key) %
           this->capacity;  // ξεκινώντας από τη θέση που κάνει hash το key
       this->array[pos]->getState() != EMPTY;  // αν φτάσουμε σε EMPTY σταματάμε
       pos =
           (pos + 1) % this->capacity) {  // linear probing, γυρνώντας στην αρχή
                                          // όταν φτάσουμε στη τέλος του πίνακα

    // Μόνο σε OCCUPIED θέσεις (όχι DELETED), ελέγχουμε αν το key είναι εδώ
    if (this->array[pos]->getState() == OCCUPIED &&
        this->compare(this->array[pos]->getKey(), key) == 0)
      return this->array[pos];

    // Αν διασχίσουμε ολόκληρο τον πίνακα σταματάμε. Εφόσον ο πίνακας δεν μπορεί
    // να είναι όλος OCCUPIED, αυτό μπορεί να συμβεί μόνο στην ακραία περίπτωση
    // που ο πίνακας έχει γεμίσει DELETED τιμές!
    count++;
    if (count == this->capacity)
      break;
  }

  return nullptr;
}
