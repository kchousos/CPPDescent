/**
 * @file ADTGraph.hpp
 * @author Phaedon Seitanidis
 * @brief Abstract undirected graph with weighted edges
 * @version 0.1
 * @date 2023-11-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "cppdescent/common.hpp"
#include "cppdescent/ADTList.hpp"
#include "cppdescent/ADTVector.hpp"
#include "cppdescent/ADTMap.hpp"

class Graph {
    private: 
        Vector vec;
        Map map;
        int size;
        CompareFunc compare;
        DestroyoFunc destroy;
        HashFunc hash;
    public:
        Graph();
        ~Graph();
        int getSize();
        void insertVertex();
        List getVertices();
        void removeVertex(Pointer vertex);
        void insertEdge(Pointer vertex1, Pointer vertex2, int weight);
        void removeEdge(Pointer vertex1, Pointer vertex2);
        int getWeight(Pointer vertex1, Pointer vertex2);
        List getAdjacent(Pointer vertex);
        Map shortestPathLengths();
        void setHashFunction(HashFunc hash);
}

class GraphVertexPair {
    private: 
        Pointer vertex1;
        Pointer vertex2;
        Graph* owner;
}


