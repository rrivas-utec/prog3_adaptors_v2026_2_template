#ifndef JOIN_STACKS_H
#define JOIN_STACKS_H


#include "stack_utec.h"
template<class T> utec::stack<T> join_stacks(utec::stack<T> a, utec::stack<T> b) {
    utec::stack<T> reversed, result;
    while (!b.empty()) { reversed.push(b.top()); b.pop(); }
    while (!a.empty()) { reversed.push(a.top()); a.pop(); }
    while (!reversed.empty()) { result.push(reversed.top()); reversed.pop(); }
    return result;
}

#endif
