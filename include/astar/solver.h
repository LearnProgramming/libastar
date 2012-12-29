#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <set>
#include <utility>
#include <vector>

#include "heapset.h"

/*
 * This class finds the best path from the starting state to the goal state
 * given a way to get neighbor states, a way to measure distance from the last state,
 * and a way to estimate cost to the goal state.
 */
template<class T, class G, class H>
class AStarSolver
{
    public:
        using Generator = std::function<void(std::vector<T>&, const T&)>;
        using Distance = std::function<G(const T& from, const T& to)>;
        using Estimator = std::function<H(const T& from, const T& to)>;

        AStarSolver(T to_solve, T goal_state, Generator g, Distance d, Estimator e);

        void print_solution(std::ostream& out = std::cout) const;
        /*
         * Attempts to solve the puzzle and returns true if it was able, false otherwise
         */
        bool solve();

    private:
        struct Node
        {
                Node* prev_;
                T state_;
                G distance_;
                H estimate_;
                decltype(distance_ + estimate_) cost_;

                Node(const AStarSolver& s, T&& state, Node* previous = nullptr);
        };


        struct ByCost {
            // using greater than creates a min-heap
            bool operator()(const Node& l, const Node& r) {
                return l.cost_ > r.cost_;
            }
        };

        struct ByState {
            bool operator()(const Node& l, const Node& r) {
                return l.state_ < r.state_;
            }
        };

        T goal_;
        Generator generator_func_;
        Distance distance_func_;
        Estimator cost_func_;
        Node* last_;
        HeapSet<Node,ByState,ByCost> states_;
};

template<class T, class G, class H>
AStarSolver<T,G,H>::Node::Node(const AStarSolver& as, T&& s, Node* p) :
    prev_(p), state_(std::forward<T>(s)),
    distance_(p ? (p->distance_ + as.distance_func_(p->state_, state_)) : G()),
    estimate_(as.cost_func_(state_, as.goal_)),
    cost_(distance_ + estimate_)
{
}

template<class T, class G, class H>
AStarSolver<T,G,H>::AStarSolver(T s, T g, Generator gen, Distance d, Estimator e) :
    goal_(std::move(g)), generator_func_(std::move(gen)), distance_func_(std::move(d)), cost_func_(std::move(e))
{
    states_.emplace(*this,std::move(s));
}

template<class T, class G, class H>
void AStarSolver<T,G,H>::print_solution(std::ostream& o) const
{
    if (last_)
    {
        o << "Solution:\n";

        auto p = last_;
        size_t step = 0;
        while(p) {
            o << "Step: " << ++step << '\n';
            o << "Estimator: " << p->cost_ << '\n';
            o << "State:\n" << p->state_ << '\n';
            p = p->prev_;
        }

    }
    else
    {
        o << "No solution found.\n";
    }
}

template<class T, class G, class H>
bool AStarSolver<T,G,H>::solve()
{
    std::vector<T> neighbors;

    while (!states_.empty()) {
        auto pnode = states_.pop();

        if (pnode->state_ == goal_) {
            last_ = pnode;
            return true;
        }

        generator_func_(neighbors, pnode->state_);

        for (auto&& n : neighbors) {
            states_.emplace(*this, std::move(n), pnode);
        }

        neighbors.clear();
    }

    return false;
}

template<class T, class F, class G, class H>
auto make_solver(T&& start, T&& goal, F&& generator, G&& distance, H&& estimator)
    -> AStarSolver<typename std::remove_reference<T>::type,decltype(distance(start, goal)), decltype(estimator(start, goal))> {
    return AStarSolver<typename std::remove_reference<T>::type,
           decltype(distance(start, goal)),
           decltype(estimator(start, goal))>
               (std::forward<T>(start),
                std::forward<T>(goal),
                std::forward<F>(generator),
                std::forward<G>(distance),
                std::forward<H>(estimator));
}
