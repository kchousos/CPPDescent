# CPP-Descent

Part of the [“Software Development for Computing Systems”
course](https://eclass.uoa.gr/courses/DI485/) of the [Department of
Informatics and Telecommunications, UoA](https://www.di.uoa.gr/en) for
the first semester of the academic year 2023-2024.

A library that implements the K-NN graph creation algorithm
described in [1], [2], [4].

## Contributors

- [Konstantinos Chousos](https://github.com/kchousos) (1115202000215,
  sdi2000215 *at* di.uoa.gr)

  <details>
  <summary><em>Contributions</em></summary>

  - 1st submission
    - Build/Testing/Code coverage pipeline
    - ADTVector, ADTList, ADTPQueue
    - Brute Force K-NN Graph creation. NN-Descent Graph creation
    - (Incomplete) Query point
  - 2nd submission
    - I/O of graphs in binary files
    - Recall function
    - Metric functions
    - Query point
    - Early termination
    - Local join
  - 3rd submission
    - Incremental search
    - Sampling
    - Code refactoring, removing Map data structure from the graph
    - Incorporation of the GNU Scientific Library [7] for faster mathematical
      computations
    - Optimization of the distance metric, by using linear computations
    - Implementation of CLI flags for the main app
    - Google Benchmark integration (DEFUNCT)
    - Refactoring of the lookup of a datapoint's neighbors, leading to major speed up
    - Script for experiments that outputs a csv file with the results
    - Recursive random projection trees [6], [5]

  </details>

- [Anastasios-Fedon Seitanidis](https://github.com/FedonSeitan)
  (1115202000179, sdi2000179 *at* di.uoa.gr)

  <details>
  <summary><em>Contributions</em></summary>

  - 1st submission
    - ADTMap, ADTGraph
    - Brute Force K-NN Graph creation
    - NN-Descent Graph creation
  - 2nd submission
    - Implementation of GraphVertex class
    - Refactor of previous Graph implementation to use GraphVertex class (without changing the existing interface)
    - `getMin()` property to Priority Queue
    - `remove()` property to Priority Queue
    - Implementation of `find()` helper function for Priority Queue

  </details>

## Running the project

To run the main executable, the following commands suffice:

    $ cmake -S . -B build
    $ cmake --build build
    $ ./build/app/app -K <number of neighbors> <filepath-to-dataset>

For example:

    $ ./build/app/app -K 100 ./datasets/00000200.bin -D 20 -T 5 -d 0.001 -r 0.3
    For K = 100
    For δ = 0.001
    For ρ = 0.3
    5 random projection trees are used, for D = 20
    Dataset: ./datasets/00000200.bin
    Dimensions: 100
    ----------------------------------------------------------
    NN-Descent
            Initializing starting graph...
            Using random projection tree...
            Starting graph has been created
            Number of changes in the graph (c) = 1950
            Number of changes in the graph (c) = 540
            Number of changes in the graph (c) = 84
            Number of changes in the graph (c) = 36
            Number of changes in the graph (c) = 15
            NN-Descent iterations: 5
    NN-Descent K-NN Graph created in 3007 milliseconds for δ = 0.001, ρ = 0.3
    Computing recall...
    Total recall is 98.9451%

### Command-line flags

| Flag | Default value | Usage                                                                                                                                                                            |
| ---- | ------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `-K` | -             | The $K$ number of neighbors for each datapoint. This value *must* be given.                                                                                                      |
| `-d` | 0.01          | The precision parameter $\delta$ used for early termination. If the updates during an iteration are less than $\delta KN$, then the algorithm concludes.                         |
| `-r` | 0.5           | The sample rate $\rho$ used for sampling. Before local join, we sample $\rho K$ out of the K-NN items marked `true` for each object to use in local join.                        |
| `-D` | 0             | The upper bound of vertices a *random projection tree*'s leaf must have. If set to 0, then the starting graph is created randomly. Must be less than $K$.                        |
| `-T` | 4             | The number of *random projection trees* to create. Must be at least 2.                                                                                                           |
| `-q` |               | For "quiet". If this flag is set, then the output is a line of comma separated values. Mainly used for automating experiments with the [corresponding script](./experiments.sh). |

### Results

The *recall* percentage that is printed during execution is computed by
the following formula, derived from [1, Sec. 3.2].

> The recall of one object is the number of its true K-NN members found
> divided by K. The recall of an approximate K-NNG is the average recall
> of all objects.

## Documentation

### Code Structure

In the [wiki](https://kchousos.github.io/CPPDescent/html/), you can find
documentation generated by [Doxygen](https://www.doxygen.nl/). It is
also available in [pdf
form](https://kchousos.github.io/CPPDescent/latex/refman.pdf).

### Code Coverage

This project uses [LCOV](https://github.com/linux-test-project/lcov) for
its code coverage needs. The results are uploaded upon commit
[here](https://kchousos.github.io/CPPDescent/lcov).

## Dependencies

For local development, you will need `Make` and `CMake`. To install them
on an apt-based linux distro, run the following commands:

    $ sudo apt update && sudo apt upgrade
    $ sudo apt install make
    $ sudo apt install cmake

### Minor dependencies

- The project uses [Google Test](https://github.com/google/googletest)
  for its testing needs. Gtest is used as a git submodule of this repo,
  that is downloaded by CMake the first time you clone. That means that
  nothing extra needs to be installed to run the tests.

- If you would like to generate the documentation, you will need
  `doxygen` installed.

## Bibliography

[1] W. Dong, C. Moses, and K. Li, “Efficient k-nearest neighbor graph construction for generic similarity measures,” in Proceedings of the 20th international conference on World wide web, in WWW ’11. New York, NY, USA: Association for Computing Machinery, Mar. 2011, pp. 577–586. doi: 10.1145/1963405.1963487.

[2] “How PyNNDescent works — pynndescent 0.5.0 documentation.” Accessed: Oct. 10, 2023. [Online]. Available: https://pynndescent.readthedocs.io/en/latest/how_pynndescent_works.html

[3] D. Kluser, J. Bokstaller, S. Rutz, and T. Buner, “Fast Single-Core K-Nearest Neighbor Graph Computation.” arXiv, Dec. 13, 2021. doi: 10.48550/arXiv.2112.06630.

[4] PyNNDescent Fast Approximate Nearest Neighbor Search with Numba | SciPy 2021, (Jul. 23, 2021). Accessed: Dec. 05, 2023. [Online Video]. Available: https://www.youtube.com/watch?v=xPadY4_kt3o

[5] J. Brugger, “brj0/nndescent.” Dec. 11, 2023. Accessed: Dec. 11, 2023. [Online]. Available: https://github.com/brj0/nndescent

[6] S. Dasgupta and Y. Freund, “Random projection trees and low dimensional manifolds,” in Proceedings of the fortieth annual ACM symposium on Theory of computing, Victoria British Columbia Canada: ACM, May 2008, pp. 537–546. doi: 10.1145/1374376.1374452.

[7] “GSL - GNU Scientific Library - GNU Project - Free Software Foundation.” Accessed: Jan. 15, 2024. [Online]. Available: https://www.gnu.org/software/gsl/