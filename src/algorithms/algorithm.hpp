#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <vector>

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
        std::vector<Point> path;
        float length = 0;
        int expanded = 0;
        int examined = 0;
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
    virtual Result get_result() { return result; }

protected:
    State* state;
    
    // Stores result information for the get_result()
    Result result;
};

#endif