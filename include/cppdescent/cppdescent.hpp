/**
 * @brief A KNN-Graph creation library.
 *
 */
#include "ADTGraph.hpp"
#include "ADTVector.hpp"

#define DEFAULT_POINTS_NUM 100

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
 * @return Vector* An N-sized vector with vectors for each element.
 */
Vector* readBinData(char* fp);
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
 * @brief Returns the Euclidean distance between two points of arbitrary
 * dimension.
 *
 * @param first A pointer to the first point.
 * @param second A pointer to the second point.
 * @return float The Euclidean distance.
 */
float euclideanDistance(Vector* first, Vector* second);
/**
 * @brief
 *
 * @param first A Pointer to the first element.
 * @param second A Pointer to the second element.
 * @return int
 */
int compareVertexPair(Pointer first, Pointer second);
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
 * @return Graph* A pointer to the optimal K-NN graph.
 */
Graph* KNNBruteForceGraph(Vector* data, int K);
};  // namespace cppdescent