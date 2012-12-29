#pragma once

#include <deque>
#include <map>
#include <memory>
#include <queue>
#include <vector>

template<class T, class SC = std::less<T>, class VC = std::less<T>>
class HeapSet {
    public:
        bool empty() const;
        T* pop();
        void push(T&& val);

        template<typename... Args>
        void emplace(Args&&... args);

    private:
        using UT = std::unique_ptr<T>;

        template<typename U, typename... Args>
        std::unique_ptr<U> make_unique(Args&&... args)
        {
            return std::unique_ptr<U>(new U(std::forward<Args>(args)...));
        }

        template<class... Args>
        UT make_utype(Args&&... args) {
            return make_unique<T>(std::forward<Args>(args)...);
        }

        struct USC {
            bool operator()(const UT& l, const UT& r) {
                return l && r && state_compare_(*l, *r);
            }
            SC state_compare_;
        };

        struct PVC {
            bool operator()(const T* l, const T* r) {
                return l && r && value_compare_(*l, *r);
            }
            VC value_compare_;
        };

        std::map<UT,bool,USC> states_;

        using heap_type = T*;
        std::priority_queue<heap_type,std::vector<heap_type>,PVC> heap_;

        // returns true if second arg is higher priority than the first
        PVC higher_priority_;
        heap_type pop_heap(bool close = true);
        void update_heap(const heap_type& old, const heap_type& updated);
        void insert(UT ut);
};

template<class T, class SC, class VC>
bool HeapSet<T,SC,VC>::empty() const {
    return states_.empty();
}

template<class T, class SC, class VC>
auto HeapSet<T,SC,VC>::pop() -> heap_type {
    return pop_heap();
}

template<class T, class SC, class VC>
void HeapSet<T,SC,VC>::push(T&& t) {
    insert(make_utype(std::move(t)));
}

template<class T, class SC, class VC> template<typename... Args>
void HeapSet<T,SC,VC>::emplace(Args&&... args) {
    insert(make_utype(std::forward<Args>(args)...));
}

template<class T, class SC, class VC>
void HeapSet<T,SC,VC>::insert(UT ut) {
    auto it = states_.find(ut);

    // TODO: emplace(_hint) vs insert
    if (it == end(states_)) {
        heap_.push(ut.get());
        states_.insert(std::make_pair(std::move(ut), false));
    } else {
        auto old = it->first.get();
        auto updated = ut.get();

        if (!it->second && higher_priority_(old, updated)) {
            update_heap(old, updated);
            it = states_.erase(it);
            states_.insert(it, std::make_pair(std::move(ut), false));
        }
    }
}

template<class T, class SC, class VC>
auto HeapSet<T,SC,VC>::pop_heap(bool close) -> heap_type {
    auto t = heap_.top();
    auto ut = UT(t);
    if (close) {
        states_.find(ut)->second = true;
    }
    ut.release();
    heap_.pop();
    return t;
}

template<class T, class SC, class VC>
void HeapSet<T,SC,VC>::update_heap(const heap_type& old, const heap_type& updated) {
    std::deque<heap_type> queue;

    do {
        queue.push_back(pop_heap(false));
    } while (queue.back() != old);

    queue.back() = updated;

    while (!queue.empty()) {
        heap_.push(queue.back());
        queue.pop_back();
    }
}
