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
        using Generator = std::function<std::vector<T>(const T&)>;
        using Distance = std::function<G(const T& from, const T& to)>;
        using Estimator = std::function<H(const T& from, const T& to)>;

        AStarSolver(const T& to_solve, const T& goal_state, const Generator& g, const Distance& d, const Estimator& e);

        void print_solution(std::ostream& out = std::cout) const;
        /*
         * Attempts to solve the puzzle and returns true if it was able, false otherwise
         */
        bool solve();

    private:
        struct Node;
        using SNode = std::shared_ptr<Node>;

        struct Node
        {
                SNode prev_;
                T state_;
                G distance_;
                H estimate_;
                decltype(distance_ + estimate_) cost_;

                Node(AStarSolver& s, const T& state, SNode previous = nullptr);
        };

        template<class... Args>
        SNode make_snode(Args&&... args) {
            return std::make_shared<Node>(std::forward<Args>(args)...);
        }

        struct ByCost {
            // using greater than creates a min-heap
            bool operator()(const SNode& l, const SNode& r) {
                return l->cost_ > r->cost_;
            }
        };

        struct ByState {
            bool operator()(const SNode& l, const SNode& r) {
                return l->state_ < r->state_;
            }
        };

        T goal_;
        Generator generator_func_;
        Distance distance_func_;
        Estimator cost_func_;
        SNode last_;
        CachingHeapSet<SNode,ByState,ByCost,CloseOnPop::TRUE> states_;
};

template<class T, class G, class H>
AStarSolver<T,G,H>::Node::Node(AStarSolver& as, const T& s, SNode p) :
    prev_(p), state_(s),
    distance_(p ? (p->distance_ + as.distance_func_(p->state_, s)) : G()),
    estimate_(as.cost_func_(s, as.goal_)),
    cost_(distance_ + estimate_)
{
}

template<class T, class G, class H>
AStarSolver<T,G,H>::AStarSolver(const T& s, const T& g, const Generator& gen, const Distance& d, const Estimator& c) :
    goal_(g), generator_func_(gen), distance_func_(d), cost_func_(c)
{
    states_.push(make_snode(*this,s));
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
    while (!states_.empty()) {
        auto snode = states_.pop();

        if (snode->state_ == goal_) {
            last_ = snode;
            return true;
        }

        for (auto& n : generator_func_(snode->state_)) {
            states_.push(make_snode(*this, n, snode));
        }
    }

    return false;
}

template<class T, class F, class G, class H>
auto make_solver(const T& start, const T& goal, F generator, G distance, H estimator)
    -> AStarSolver<T,decltype(distance(start, goal)), decltype(estimator(start, goal))> {
    return AStarSolver<T,decltype(distance(start, goal)), decltype(estimator(start, goal))>(start, goal, generator, distance, estimator);
}
