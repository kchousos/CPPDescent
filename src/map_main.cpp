#include <iostream>
#include "./include/ADTMap.hpp"

int main(void) {
    class Map* map = new Map(compareInts, destroyInts, destroyInts);
    map->setHashFunction(hashInts);
    
    // Check if the map is created
    std::cout << "Map: " << map << std::endl;
    std::cout << "Array: " << map->getArray() << std::endl;
    std::cout << "Size: " << map->getSize() << std::endl;
    std::cout << "Capacity: " << map->getCapacity() << std::endl;

    std::cout << "----------------------" <<std::endl;

    // Check insert
    int x = 5, y = 7;
    map->insert(&x, &y);
    
    std::cout << "Map: " << map << std::endl;
    std::cout << "Array: " << map->getArray() << std::endl;
    std::cout << "Size: " << map->getSize() << std::endl;
    std::cout << "Capacity: " << map->getCapacity() << std::endl;

    std::cout << "----------------------" <<std::endl;

    for (int i = 0; i < 55; i++) {
        x = i+1;
        map->insert(&i,&x);

        /**
         * todo Βλέπω αν τα sizes μετά το rehash είναι σωστά ή όχι
         */
        // std::cout << "Size in loop: " << map->getSize() << std::endl;
        // std::cout << "Capacity in loop: " << map->getCapacity() << std::endl;
        // std::cout << std::endl << std::endl;
    }

    std::cout << "Map: " << map << std::endl;
    std::cout << "Array: " << map->getArray() << std::endl;
    std::cout << "Size: " << map->getSize() << std::endl;
    std::cout << "Capacity: " << map->getCapacity() << std::endl;

    std::cout << "----------------------" <<std::endl;

    // Check remove and findNode
    map->remove(&x);

    std::cout << "Map: " << map << std::endl;
    std::cout << "Array: " << map->getArray() << std::endl;
    std::cout << "Size: " << map->getSize() << std::endl;
    std::cout << "Capacity: " << map->getCapacity() << std::endl;

    std::cout << "----------------------" <<std::endl;
    
    // Check getFirst and getNext
    std::cout << "Map: " << map << std::endl;
    std::cout << "Array: " << map->getArray() << std::endl;
    std::cout << "First node: " << map->getFirst() << std::endl;
    std::cout << "Next node: " << map->getNext(map->getFirst()) << std::endl;

    std::cout << "----------------------" <<std::endl;

    // Check destructor
    delete map;

    return 0;
}
