/**
 * @brief A KNN-Graph creation library.
 *
 */
#include "ADTGraph.hpp"
#include "ADTVector.hpp"

#define DEFAULT_POINTS_NUM 100

typedef float (*DistanceFunc)(Pointer a, Pointer b);

/**
 * @brief Functions for the creation of a K-NN graph.
 *
 */
namespace cppdescent {
/**
 * @brief Delete a heap-allocated float.
 *
 * @param value A Pointer to the float.
 */
void deleteFloat(Pointer value);
/**
 * @brief Create a heap-allocated float.
 *
 * @param value The float's value.
 * @return float* A float pointer to the newly created value.
 */
float* createFloat(float value);
/**
 * @brief Compares two floats.
 *
 * @param a First float pointer.
 * @param b Second float pointer.
 * @return int > 0 if a > b, 0 if a == b, < 0 if a < b.
 */
float compareFloats(Pointer a, Pointer b);
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
 * @brief Frees the memory allocated to storing the dataset.
 *
 * Deletes each 100-sized vector and the parent N-sized one.
 *
 * @param vec A pointer to the parent vector.
 * @return int 0 if the vectors were deleted succesfully, -1 if the given vec is
 * nullptr.
 */
int deleteDatapointVectors(Vector* vec);
/**
 * @brief Compares each dimension of the two points.
 *
 * @param first The first point.
 * @param second The second point.
 * @return int 0 if the points are the same on all dimensions, 1 otherwise.
 */
int compareVertices(Pointer first, Pointer second);

int hashEdge(Pointer edge);

void destroyEdges(GraphVertexPair* pair);

/**
 * @brief Sorts the edges array in O(n*logn) time.
 *
 * @param edges An array of edges.
 * @param low
 * @param high
 * @param compare The function to use for comparisons.
 */
void EdgesQuickSort(GraphVertexPair** edges,
                    int low,
                    int high,
                    CompareFunc compare);
/**
 * @brief Sorts the edges array in O(n^2) time.
 *
 * @param edges An array of edges.
 * @param size The size of the array.
 * @param compare The function to use for comparisons.
 */
void EdgesBubbleSort(GraphVertexPair** edges, int size, CompareFunc compare);
/**
 * @brief Searches for an edge in the edges array.
 *
 * @param edges The array.
 * @param edge The edge to search.
 * @param low
 * @param high
 * @param compare The function to use for comparisons.
 * @return true If the edge was found.
 * @return false If the edge was not found.
 */
bool EdgesBinarySearch(GraphVertexPair** edges,
                       GraphVertexPair* edge,
                       int low,
                       int high,
                       CompareFunc compare);
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

Graph* NNDescent(Vector* data, int K, CompareFunc compare, DistanceFunc distance);
};  // namespace cppdescent