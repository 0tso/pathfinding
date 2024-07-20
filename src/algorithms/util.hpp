#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstddef>
#include <utility>
#include <cmath>
#include <span>
#include <algorithm>

#include "state.hpp"
#include "algorithms/algorithm.hpp"

/**
 * The main type used by nodes to refer to each other.
 * Represents an index into a flat array of nodes.
 */
typedef size_t node_index;


/**
 * Square root of two.
 * Used for diagonal movement costs.
 */
constexpr float SQRT_2 = 1.4142136;


enum Direction : uint8_t
{
    NORTH = 0,
    NORTHEAST,
    EAST,
    SOUTHEAST,
    SOUTH,
    SOUTHWEST,
    WEST,
    NORTHWEST
};



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
     * Gets the (x, y) coordinates matching the specified direction.
     * (0, 0) is assumed to be in the northwestern corner, so north and west are both -1.
     * 
     * @param dir The direction whose movement to query.
     * 
     * @returns An std::pair<int, int> with (x, y) changes.
     */
    std::pair<int, int> get_movement(Direction dir);


    /**
     * Returns the manhattan distance between two points: (x1, y1) and (x2, y2).
     * Manhattan distance: |x1 - x2| + |y1 - y2| 
     * Used for heuristics in the pathfinding algorithms.
     */
    inline float manhattan_distance(int x1, int y1, int x2, int y2)
    {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    }

    /**
     * Returns the euclidian distance between two points (x1, y1) and (x2, y2).
     */
    inline float euclidian_distance(int x1, int y1, int x2, int y2)
    {
        return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }

    /**
     *  Builds the path from the nodes when the end has been reached.
     *  The nodes must be of type T.
     *  Each node object of type T is expected to have a pointer to node_index called "prev".
     *  This "prev" member variable points to the previous point in the path.
     *  The path generation is started from the end node in the state.
     */
    template<typename T>
    void build_path(State& state, std::span<T> nodes, Algorithm::Result& res)
    {
        auto end_idx = flatten(state.width, state.end.x, state.end.y);
        node_index prev_idx = end_idx;
        while(true)
        {
                T& prev_node = nodes[prev_idx];
                if(prev_node.prev == std::numeric_limits<node_index>::infinity())
                {
                    break;
                }
                auto [x, y] = expand(state.width, prev_idx);
                res.path.emplace_back(x, y);
                state.map[prev_idx] = Node::PATH;
                prev_idx = prev_node.prev;
        }
        std::reverse(res.path.begin(), res.path.end());
    }
}

#endif