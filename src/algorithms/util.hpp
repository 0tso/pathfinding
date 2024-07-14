#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstddef>
#include <utility>
#include <cmath>

#include "state.hpp"

/**
 * The main type used by nodes to refer to each other.
 * Represents an index into a flat array of nodes.
 */
typedef size_t node_index;

namespace Util
{
    /**
     * Finds all the possible neighbours of the node in the specified location.
     * 
     * @param buffer The buffer in which to store the neighbours.
     *      The first pair element is the node index, the second is a boolean representing whether or not the neighbour is diagonal.
     * @param state The state from which to read.
     * @param x The x-position of the node.
     * @param y The y-position of the node.
     * 
     * @returns The number of neighbours found and added to the buffer.
     */
    int get_neighbours(std::pair<node_index, bool>* buffer, const State& state, int x, int y);


    /**
     * Flattens the input coordinates (x, y) to single-dimensional array coordinates.
     */
    inline int flatten(int width, int x, int y)
    {
        return y * width + x;
    }

    /**
     * Expands the input node index into a pair of coordinates (x, y).
     * The inverse function of the above flatten()
     */
    inline std::pair<int, int> expand(int width, int idx)
    {
        return {idx % width, idx / width};
    }

    /**
     * Returns the manhattan distance between two points: (x1, y1) and (x2, y2).
     * Manhattan distance: |x1 - x2| + |y1 - y2| 
     * Used for heuristics in the pathfinding algorithms.
     */
    inline float manhattan_distance(int x1, int y1, int x2, int y2)
    {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    }
}

#endif