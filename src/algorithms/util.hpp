#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstddef>
#include <utility>
#include <cmath>
#include <algorithm>

#include "state.hpp"
#include "algorithms/algorithm.hpp"

/**
 * The main type used by nodes to refer to each other.
 * Represents an index into a flat array of nodes.
 */
typedef size_t node_index;
constexpr node_index NULL_NODE_IDX = std::numeric_limits<node_index>::infinity();

/**
 * Square root of two rounded upwards.
 * Used for diagonal movement costs.
 */
constexpr float SQRT_2 = 1.41421357;


struct Direction
{
    enum : uint8_t
    {
        NORTH = 0,
        NORTHEAST,
        EAST,
        SOUTHEAST,
        SOUTH,
        SOUTHWEST,
        WEST,
        NORTHWEST
    } type;

    bool straight;

    // Relative change in position
    std::pair<int, int> movement;

    // If a diagonal direction, the component directions
    // For example NORTHEAST -> NORTH, EAST
    std::pair<const Direction*, const Direction*> components;
};

typedef const Direction* dir_t;

/**
 * All of the directions in an array.
 */
extern dir_t const directions[];

extern dir_t const DIR_NORTH;
extern dir_t const DIR_NORTHEAST;
extern dir_t const DIR_EAST;
extern dir_t const DIR_SOUTHEAST;
extern dir_t const DIR_SOUTH;
extern dir_t const DIR_SOUTHWEST;
extern dir_t const DIR_WEST;
extern dir_t const DIR_NORTHWEST;


namespace Util
{
    /**
     * Finds all the possible neighbours of the node in the specified location.
     * 
     * @param buffer The buffer in which to store the neighbours.
     *      The first pair element is the node index, the second is the direction of the neighbour.
     * @param state The state from which to read.
     * @param x The x-position of the node.
     * @param y The y-position of the node.
     * 
     * @returns The number of neighbours found and added to the buffer.
     */
    int get_neighbours(std::pair<node_index, dir_t>* buffer, const State& state, int x, int y);

    /**
     * Checks if the move from (x, y) is valid.
     * Especially important for checking diagonals.
     * 
     * @param state The world state.
     * @param x, y The coordinates of the node from which to move
     * @param dir The direction in which to move.
     * 
     * @returns Whether the move is possible.
     */
    bool is_move_valid(const State& state, int x, int y, dir_t dir);

    /**
     * Returns the direction from (x1, y1) to (x2, y2).
     * Only designed for neighbours, but also somewhat works for farther-away nodes.
     * 
     * Northeast is assumed to be (1, -1).
     */
    dir_t get_direction(int x1, int y1, int x2, int y2);

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
     * Is the specified node within map bounds?
     */
    inline bool is_valid(const State& state, int x, int y)
    {
        return x >= 0
            && x < state.width
            && y >= 0
            && y < state.height;
    }

    /**
     * Is the specified node a wall?
     */
    inline bool is_wall(const State& state, int x, int y)
    {
        auto idx = flatten(state.width, x, y);
        return is_valid(state, x, y) && state.map[idx] == Node::WALL;
    }

    /**
     * Is the specified node an empty point, i.e. valid and not a wall?
     */
    inline bool is_empty(const State& state, int x, int y)
    {
        auto idx = flatten(state.width, x, y);
        return is_valid(state, x, y) && state.map[idx] != Node::WALL;
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

    /**
     * Returns the euclidian distance between two points (x1, y1) and (x2, y2).
     */
    inline float euclidian_distance(int x1, int y1, int x2, int y2)
    {
        return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }

    /**
     * Returns the distance between the two points as if there were no obstacles between them.
     */
    inline float diagonal_distance(int x1, int y1, int x2, int y2)
    {
        auto x_diff = std::abs(x2 - x1);
        auto y_diff = std::abs(y2 - y1);
        int diagonal = std::min(x_diff, y_diff);
        int straight = std::max(x_diff, y_diff) - diagonal;
        return diagonal * SQRT_2 + straight;
    }

    /**
     *  Builds the path from the nodes when the end has been reached.
     *  The nodes must be of type T.
     *  Each node object of type T is expected to have a node_index member variable called "prev".
     *  This "prev" variable points to the previous point in the path.
     *  The path generation is started from the end node in the state.
     */
    template<typename T>
    void build_path(State& state, T* nodes, Algorithm::Result& res)
    {
        auto end_idx = flatten(state.width, state.end.x, state.end.y);
        node_index prev_idx = end_idx;
        while(true)
        {
                T& prev_node = nodes[prev_idx];
                if(prev_node.prev == NULL_NODE_IDX)
                {
                    break;
                }
                auto [x, y] = expand(state.width, prev_idx);
                res.path.emplace_back(x, y);
                if(!(x == state.end.x && y == state.end.y))
                    state.map[prev_idx] = Node::PATH;
                prev_idx = prev_node.prev;
        }
        std::reverse(res.path.begin(), res.path.end());
    }
}

#endif