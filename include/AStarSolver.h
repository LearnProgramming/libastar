#ifndef A_STAR_SOLVER_H
#define A_STAR_SOLVER_H

#include "Node.h"

/*
 * This class finds the best path from the starting state to the goal state
 * given the heuristic.
 */
template<class T>
class AStarSolver {
    public:
        typedef typename Node<T>::Heuristic Heuristic;
        AStarSolver(const T& to_solve, const T& goal_state, Heuristic h);

        /*
         * Attempts to solve the puzzle and returns true if it was able, false otherwise
         */
        bool solve();
        void print_solution(std::ostream& out = std::cout) const;
};
#endif // A_STAR_SOLVER_H

