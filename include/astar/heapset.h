#pragma once

#include <deque>
#include <queue>
#include <set>
#include <vector>

template<class T, class SC = std::less<T>, class VC = std::less<T>>
class HeapSet {
    public:
        bool empty() const;
        const T& top() const;
        T pop();
        void push(const T& val);
        void push(T&& val);

    private:
        std::set<T,SC> states_;
        std::priority_queue<T,std::vector<T>,VC> heap_;
        // returns true if second arg is higher priority than the first
        VC higher_priority_;
        T pop_heap();
        void update_heap(const T& old, const T& updated);
};

template<class T, class SC, class VC>
bool HeapSet<T,SC,VC>::empty() const {
    return states_.empty();
}

template<class T, class SC, class VC>
const T& HeapSet<T,SC,VC>::top() const {
    return heap_.top();
}

template<class T, class SC, class VC>
T HeapSet<T,SC,VC>::pop() {
    T t = pop_heap();
    states_.erase(t);
    return std::move(t);
}

template<class T, class SC, class VC>
void HeapSet<T,SC,VC>::push(const T& t) {
    auto it_inserted = states_.insert(t);

    if (it_inserted.second) {
        heap_.push(t);
    } else {
        auto old = *it_inserted.first;
        if (higher_priority_(old, t)) {
            update_heap(old, t);
            auto it = states_.erase(it_inserted.first);
            states_.insert(it, t);
        }
    }
}

template<class T, class SC, class VC>
void HeapSet<T,SC,VC>::push(T&& t) {
    auto it_inserted = states_.insert(t);

    if (it_inserted.second) {
        heap_.push(std::forward<T>(t));
    } else {
        auto old = *it_inserted.first;
        if (higher_priority_(old, t)) {
            update_heap(old, t);
            auto it = states_.erase(it_inserted.first);
            states_.insert(it, std::forward<T>(t));
        }
    }
}

template<class T, class SC, class VC>
T HeapSet<T,SC,VC>::pop_heap() {
    T t = heap_.top();
    heap_.pop();
    return std::move(t);
}

template<class T, class SC, class VC>
void HeapSet<T,SC,VC>::update_heap(const T& old, const T& updated) {
    std::deque<T> queue;

    do {
        queue.push_back(pop_heap());
    } while (queue.back() != old);

    queue.back() = updated;

    while (!queue.empty()) {
        heap_.push(queue.back());
        queue.pop_back();
    }
}
