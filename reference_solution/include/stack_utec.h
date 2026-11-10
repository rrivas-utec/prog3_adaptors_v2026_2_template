#ifndef STACK_UTEC_H
#define STACK_UTEC_H

#include "adaptor_concepts.h"
#include <deque>
#include <cstddef>
#include <utility>

namespace utec {
template<class T, StackConcept Container = std::deque<T>>
    requires std::same_as<T, typename Container::value_type>
class stack {
    Container data_;
public:
    bool empty() const { return data_.empty(); }
    std::size_t size() const { return data_.size(); }
    T& top() { return data_.back(); }
    const T& top() const { return data_.back(); }
    void push(const T& value) { data_.push_back(value); }
    void push(T&& value) { data_.push_back(std::move(value)); }
    void pop() { data_.pop_back(); }
};
}

#endif

