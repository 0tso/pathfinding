# Program & algorithm structure

### Program structure
Rendering, user input and other UI-related functionality is located in the top-level [src](../src/) directory.

Actual pathfinding logic is located in the [algorithms](../src/algorithms/) subfolder. This document will focus on that logic.

* The [State](../src/state.hpp) module functions as the link between these two systems. It is the only top-level module referenced in the pathfinding modules, and contains definitions about the state of the pathfinding map.
A pointer to a State structure is passed to the algorithms, which they then subsequently modify to show the results of the pathfinding to the user.

* The [Util](../src/algorithms/util.hpp) module contains utility functions for all the algorithms to use.
These include, for example, functions to get all neighbours of a particular node in the map, heuristic functions, and definitions of directions.

* The [Algorithm](../src/algorithms/algorithm.hpp) module defines the abstract base class from which all algorithms inherit.

* The [Common](../src/algorithms/common.hpp) module defines a base class (inheriting from Algorithm) for the A* and JPS algorithms. This base class contains definitions, as the name suggests, common for both of the algorithms.

* The [AStar](../src/algorithms/a_star.hpp) and [JumpPointSearch](../src/algorithms/jps.hpp) modules contain the actual pathfinding algorithms. The rest of the document will focus on the technical and theoretical bases of these algorithms.

### Algorithms

#### A*
A* is a network pathfinding algorithm that uses both the distance of the starting node and a heuristic function approximating the distance to the goal node. In this manner, it is a direct improvement on the more well-known Dijkstra's algorithm. In the following paragraphs, I will highlight this difference by comparing these algorithms directly.

Like Dijkstra's algorithm, A* maintains a sorted priority queue of nodes to explore next. The next node to look at and expand is picked from the front of the priority queue.

In Dijkstra's algorithm, the priority queue is sorted using the function `g(x)`, denoting the distance of node `x` from the starting node. This way, Dijkstra's algorithm guarantees optimality: if it only selects the node closest to the start, when it eventually expands the goal node, all nodes after it are guaranteed to be either equally far away or even farther away from the start, so their distance would be even greater. Thus the shortest path is found.

A* works on the same principles, but instead of sorting the priority queue using `g(x)`, it sorts the priority queue using the sum of `g(x) + h(x)` where `h(x)` is a heuristic function estimating the distance from node `x` to the goal node. This way A* can find a path to the goal node quicker by also prioritizing an estimated distance to the goal. If this estimation is never greater than the actual distance, A* is guaranteed to find the optimal path.
In the present project, this is done by using euclidian distance as the heuristic: this is guaranteed to be smaller or equal to the shortest path.

In short, in each loop iteration, A* examines and pops the "best" node (judged by `g(x) + h(x)`) from its priority queue, checks if shorter paths (than previously encountered) to its neighbours can be achieved through this node, and adds those shorter paths with their heuristic to the priority queue. When A* encounters the goal node, the shortest path has been found.

#### JPS
Jump Point Search (JPS) is a search-space pruning algorithm that can, and needs to, be combined with any pathfinding algorithm. In the present project, JPS was combined with the aforementioned A* algorithm.



### Testing and benchmarking
For information on testing and benchmarking, see [testing_and_benchmarking.md](./testing_and_benchmarks.md).

### About the usage of large language models (for the University of Helsinki course)
Laajoja kielimalleja ei ole käytetty työssä.