#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <set>
#include <utility>
#include <vector>


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
        struct Node
        {
                T state_;
                G distance_;
                H estimate_;
                decltype(distance_ + estimate_) cost_;
                std::shared_ptr<Node> prev_;

                Node(AStarSolver& s, const T& state, std::shared_ptr<Node> previous = nullptr);
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
        std::shared_ptr<Node> last_;
        std::set<Node,ByState> closed_set;
        std::priority_queue<Node,std::vector<Node>,ByCost> open_set;
};

template<class T, class G, class H>
AStarSolver<T,G,H>::Node::Node(AStarSolver& as, const T& s, std::shared_ptr<Node> p) :
    state_(s), prev_(p),
    distance_(p ? (p->distance_ + as.distance_func_(p->state_, s)) : G()),
    estimate_(as.cost_func_(s, as.goal_)),
    cost_(distance_ + estimate_)
{
}

template<class T, class G, class H>
AStarSolver<T,G,H>::AStarSolver(const T& s, const T& g, const Generator& gen, const Distance& d, const Estimator& c) :
    goal_(g), generator_func_(gen), distance_func_(d), cost_func_(c)
{
    open_set.push(Node(*this,s));
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
            o << '\n';
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
    while (!open_set.empty()) {
        auto it_inserted = closed_set.insert(open_set.top());
        auto sp = std::make_shared<Node>(*(it_inserted.first));

        if (sp->state_ == goal_) {
            last_ = sp;
            return true;
        }

        open_set.pop();

        // wasn't present in the set before
        if (it_inserted.second) {
            for (auto& n : generator_func_(sp->state_)) {
                auto node = Node(*this, n, sp);
                if (closed_set.find(node) == end(closed_set)) {
                    open_set.push(node);
                }
            }
        }
    }

    return false;
}

template<class T, class X, class Y, class Z>
auto make_solver(const T& start, const T& goal, X generator, Y distance, Z estimator)
    -> AStarSolver<T,decltype(distance(start, goal)), decltype(estimator(start, goal))> {
    return AStarSolver<T,decltype(distance(start, goal)), decltype(estimator(start, goal))>(start, goal, generator, distance, estimator);
}
