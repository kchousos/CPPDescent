/**
 * @brief A KNN-Graph creation library.
 *
 */
#include "ADTGraph.hpp"
#include "ADTVector.hpp"

typedef float (*DistanceFunc)(Pointer a, Pointer b);

/**
 * @brief Functions for the creation of a K-NN graph.
 *
 */
namespace cppdescent {
void deleteFloat(Pointer value);

float* createFloat(float value);

float compareFloats(Pointer a, Pointer b);

int deleteDatapointVectors(Vector* vec);

int compareVertices(Pointer first, Pointer second);
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
Vector* readBinData(char* fp, int dimensions);

/**
 * @brief Writes a computed graph to a binary file.
 *
 * The structure of the binary file that will be created is the following:
 *
 * 1. <uint32_t> N: the number of the vertices
 *
 * 2. N * 100 floats: each vertex of 100 dimensions
 *
 * 3. N * K * 2 int: pairs of ints that describe the edges between the vertices.
 * Each int represents the index of the vertex in the order above. This will
 * be handy when we reconstruct the graph using the getAt() function of the
 * vector.
 *
 * @param fp The filepath to the created file.
 * @param graph
 */
void writeBinGraph(char* fp, Graph* graph);

/**
 * @brief Reads a graph from a binary file. To work correctly, the file needs to
 * be first created from the 'writeBinGraph' function.
 *
 * @param fp The filepath.
 * @param dimensions The dimensions of the datapoints.
 * @param distance A function to compute the distance between the vertices.
 * @return Graph*
 */
Graph* readBinGraph(char* fp, int dimensions, DistanceFunc distance);
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
 * @param distance The function that computes the distance between two points
 * (vectors).
 * @return Graph* A pointer to the optimal K-NN graph.
 */
Graph* KNNBruteForceGraph(Vector* data,
                          int K,
                          CompareFunc compare,
                          DistanceFunc distance);
/**
 * @brief Computes the K-NN graph for the given dataset using the NN-Descent
 * algorithm.
 *
 * The user is responsible deletion of the allocated memory of the returned
 * graph.
 *
 * @param data A Vector of the vertices of the graph.
 * @param K The number of nearest neighbors to compute.
 * @param distance The function to be used to compute the distances between
 * vertices.
 * @return Graph* The complete K-NN graph of the dataset.
 */
Graph* NNDescent_KNNGraph(Vector* data, int K, DistanceFunc distance);
/**
 * @brief Computes the K Nearest Neighbors of the query point in the graph.
 *
 * @param graph The K-NN Graph, in which we search for neighbors of the query.
 * @param K
 * @param compare The function used to compare edges.
 * @param query The query point, given as a Vector. Must be of the same
 * dimensions as the rest points of the graph.
 * @return List* A list of the K-NN of the query. If the query point is of wrong
 * dimensions, then nullptr is returned.
 */
List* NNDescent_Query(Graph* graph, int K, CompareFunc compare, Vector* query);
};  // namespace cppdescent