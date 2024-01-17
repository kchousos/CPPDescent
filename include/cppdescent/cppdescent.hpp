/**
 * @brief A KNN-Graph creation library.
 *
 */
#include "ADTGraph.hpp"
#include "ADTVector.hpp"

typedef float (*DistanceFunc)(Pointer a, Pointer b);
extern bool verbose;

/**
 * @brief Functions for the creation of a K-NN graph.
 *
 */
namespace cppdescent {
int compareGraphVertices(Pointer vertex1, Pointer vertex2);

int compareGraphVertexPairs(Pointer p1, Pointer p2);

// ================================ I/O ======================================
/**
 * @brief Reads the data from a binary file.
 *
 * The data read are stored in a N-sized Vector, where N is the <uint32_t>
 * at the start of the file. Each node of this vector is a pointer to another
 * 100-sized vector, of which the values are pointers to floats.
 *
 * All of the vectors (the parent N-sized and each of the 100-sized) must
 * be freed by the user.
 *
 * @param fp A string (char *) of the filepath to the binary dataset.
 * @param dimensions The dimension of each point.
 * @return Vector* An N-sized vector with vectors for each element.
 */
Vector* readBinData(const char* fp, int dimensions);

/**
 * @brief Writes a computed graph to a binary file.
 *
 * The structure of the binary file that will be created is the following:
 *
 * 1. <uint32_t> N: the number of the vertices
 *
 * 2. N * 100 floats: each vertex of 100 dimensions
 *
 * 3. N * K * 1 int: For each vertex sequentially, the positions of its K
 * neighbors.
 *
 * @param fp The filepath to the created file.
 * @param K
 * @param graph
 */
void writeBinGraph(const char* fp, Graph* graph, int K);

/**
 * @brief Reads a graph from a binary file. To work correctly, the file needs
 * to be first created from the 'writeBinGraph' function.
 *
 * @param fp The filepath.
 * @param dimensions The dimensions of the datapoints.
 * @return Graph*
 */
Graph* readBinGraph(const char* fp, int dimensions);

// ============================ Helper Functions =============================
/**
 * @brief Returns the recall of the graph computed by NN-Descent, compared to
 * the brute force graph.
 *
 * @param bfGraph
 * @param nnGraph
 * @param N
 * @param K
 * @return float
 */
float recall(Graph* bfGraph, Graph* nnGraph, int N, int K);

// ============================ Metric Functions =============================
/**
 * @brief Returns the Euclidean distance between two points of arbitrary
 * dimension.
 *
 * @param first A pointer to the first point.
 * @param second A pointer to the second point.
 * @return long double The Euclidean distance.
 */
float euclideanDistance(Pointer a, Pointer b);
/**
 * @brief Returns the Manhattan distance between two points of arbitrary
 * dimension.
 *
 * @param first A pointer to the first point.
 * @param second A pointer to the second point.
 * @return long double The Manhattan distance.
 */
float manhattanDistance(Pointer a, Pointer b);
/**
 * @brief Compare edges using the euclideanDistance function.
 *
 * @param first A Pointer to the first element.
 * @param second A Pointer to the second element.
 * @return int
 */
int compareEdgesEuclidean(Pointer first, Pointer second);

int compareVertexPairsEuclidean(Pointer first, Pointer second);
/**
 * @brief Compare edges using the manhattanDistance function.
 *
 * @param first A Pointer to the first element.
 * @param second A Pointer to the second element.
 * @return int
 */
int compareEdgesManhattan(Pointer first, Pointer second);

// ============================= KNN computation =============================
/**
 * @brief Computes the K-NN graph using brute force.
 *
 * Useful for testing the recall and the performance of our NN-Descent
 * algorithm.
 *
 * The user is responsible deletion of the allocated memory of the graph.
 *
 * @param data A pointer to the parent N-sized vector.
 * @param K The number of Nearest Neigbors to find.
 * @param compare The function to use to compare the distances.
 * @return Graph* A pointer to the optimal K-NN graph.
 */
Graph* KNNBruteForceGraph(Vector* data, int K, CompareFunc compare);
/**
 * @brief Computes the K-NN graph for the given dataset using the NN-Descent
 * algorithm.
 *
 * The user is responsible deletion of the allocated memory of the returned
 * graph.
 *
 * @param data A Vector of the vertices of the graph.
 * @param K The number of nearest neighbors to compute.
 * @param delta The iterations will stop when the number of edges that were
 * updated is less than delta*N*K.
 * @param rho The sampling rate
 * @param distance The function to be used to compute the distances between
 * vertices.
 * @return Graph* The complete K-NN graph of the dataset.
 */
Graph* NNDescent_KNNGraph(Vector* data,
                          int K,
                          float delta = 0.01,
                          float rho = 0.5,
                          DistanceFunc distance = euclideanDistance);
/**
 * @brief Computes the K Nearest Neighbors of the query point in the graph.
 *
 * @param graph The K-NN Graph, in which we search for neighbors of the query.
 * @param K
 * @param compare The function used to compare edges.
 * @param query The query point, given as a Vector. Must be of the same
 * dimensions as the rest points of the graph.
 * @return PQueue* A priority queue of the K-NN of the query, with 'max' being
 * the most distant point relative to the query. It contains GraphVertexPairs
 * from the query point directed to the other nodes. If the query point is of
 * wrong dimensions, then nullptr is returned.
 */
PQueue* NNDescent_Query(Graph* graph,
                        int K,
                        CompareFunc compare,
                        Vector* query);
};  // namespace cppdescent