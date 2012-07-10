#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>

template<class T> class AStarSolver;

/*
 * A Node is an element in the graph of solutions used during A* searching
 */
template<class T>
class Node {
    public:
        Node(const T& state, int g_value = 0, int h_value = 0);

        int cost() const;
        const T& get_state() const;
        std::vector<T> get_neighbor_states() const;

        typedef int (*Heuristic)(const T& from, const T& to);

        int estimate_distance(const Node<T>& to, Heuristic) const;
};
#endif // NODE_H

