#ifndef QUEUE_UTEC_H
#define QUEUE_UTEC_H

#include "adaptor_concepts.h"
#include <deque>
#include <utility>

namespace utec {
template<class T, QueueConcept Container = std::deque<T>>
    requires std::same_as<T, typename Container::value_type>
class queue {
    Container data_;
public:
    bool empty() const { return data_.empty(); }
    std::size_t size() const { return data_.size(); }
    T& front() { return data_.front(); }
    const T& front() const { return data_.front(); }
    T& back() { return data_.back(); }
    const T& back() const { return data_.back(); }
    void push(const T& value) { data_.push_back(value); }
    void push(T&& value) { data_.push_back(std::move(value)); }
    void pop() { data_.pop_front(); }
};
}

#endif

