#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <vector>
#include <SFML/System/Vector2.hpp>

#include "state.hpp"

class Algorithm
{
public:

    struct Result
    {
        enum Type
        {
            EXECUTING,
            FAILURE,
            SUCCESS
        };

        Type type = Type::FAILURE;
        std::vector<sf::Vector2i> path;
        float length = 0;
    };

    /**
     * Initializes the algorithm.
     * 
     * @param state The state to update during the algorithm.
     */
    virtual void init(State* state) = 0;

    /**
     * Runs one iteration of the algorithm.
     */
    virtual Result::Type update() = 0;

    /**
     * Gets the Result object of the algorithm.
     * Only call after the algorithm has completed. Otherwise undefined behaviour.
     */
    virtual Result get_result() = 0;

protected:
    State* state;
};

#endif