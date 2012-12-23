#pragma once

#include <deque>
#include <queue>
#include <set>
#include <vector>

enum class CloseOnPop : bool { FALSE, TRUE };

template<class T>
class Closeable {
    public:
        Closeable(const T& t, bool c = false) : t_(t), closed_(c) {}
        operator T&() { return t_; }
        operator const T&() const { return t_; }
        bool operator!() const { return closed_; }
    private:
        T t_; bool closed_;
};

template<class T>
class Wrapper {
    public:
        Wrapper(const T&, bool = false) {}
        operator T&() { return t_; }
        operator const T&() const { return t_; }
        bool operator!() const { return false; }
    private:
        T t_;
};

template<class T, CloseOnPop COP> struct SetT {
    using type = Wrapper<T>;
};
template<class T> struct SetT<T,CloseOnPop::TRUE> {
    using type = Closeable<T>;
};

template<class T,
    class SC = std::less<T>, class VC = std::less<T>,
    CloseOnPop COP = CloseOnPop::FALSE>
class CachingHeapSet {
    public:
        bool empty() const;
        const T& top() const;
        T pop();
        void push(const T& val);

    private:
        using SetType = typename SetT<T,COP>::type;

        std::set<SetType,SC> states_;
        std::priority_queue<T,std::vector<T>,VC> heap_;
        // returns true if second arg is higher priority than the first
        VC higher_priority_;
        T pop_heap(CloseOnPop);
        void update_heap(const T& old, const T& updated);
};

template<class T, class SC, class VC, CloseOnPop COP>
bool CachingHeapSet<T,SC,VC,COP>::empty() const {
    return states_.empty();
}

template<class T, class SC, class VC, CloseOnPop COP>
const T& CachingHeapSet<T,SC,VC,COP>::top() const {
    return heap_.top();
}

template<class T, class SC, class VC, CloseOnPop COP>
T CachingHeapSet<T,SC,VC,COP>::pop() {
    return pop_heap(COP);
}

template<class T, class SC, class VC, CloseOnPop COP>
void CachingHeapSet<T,SC,VC,COP>::push(const T& t) {
    auto it_inserted = states_.insert(SetType{t});
    bool inserted = it_inserted.second;
    auto& tt = *it_inserted.first;

    if (inserted) {
        heap_.push(tt);
    } else {
        if (!tt && higher_priority_(tt, t)) {
            update_heap(tt, t);
            auto it = states_.erase(it_inserted.first);
            states_.insert(it, t);
        }
    }
}

template<class T, class SC, class VC, CloseOnPop COP>
T CachingHeapSet<T,SC,VC,COP>::pop_heap(CloseOnPop cop) {
    T t = heap_.top();
    if (COP == CloseOnPop::TRUE && cop == CloseOnPop::TRUE) {
        SetType val{t, true};
        auto it = states_.find(val);
        it = states_.erase(it);
        states_.insert(it, val);
    }
    heap_.pop();
    return std::move(t);
}

template<class T, class SC, class VC, CloseOnPop COP>
void CachingHeapSet<T,SC,VC,COP>::update_heap(const T& old, const T& updated) {
    std::deque<T> queue;

    do {
        queue.push_back(pop_heap(CloseOnPop::FALSE));
    } while (queue.back() != old);

    queue.back() = updated;

    while (!queue.empty()) {
        heap_.push(queue.back());
        queue.pop_back();
    }
}
