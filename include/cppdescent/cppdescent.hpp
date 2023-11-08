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
};  // namespace cppdescent