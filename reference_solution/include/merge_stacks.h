#ifndef MERGE_STACKS_H
#define MERGE_STACKS_H

#include "stack_utec.h"
template<class T> utec::stack<T> merge_stacks(utec::stack<T> a, utec::stack<T> b) {
    utec::stack<T> reversed, result;
    while (!a.empty() || !b.empty()) {
        if (!b.empty()) { reversed.push(b.top()); b.pop(); }
        if (!a.empty()) { reversed.push(a.top()); a.pop(); }
    }
    while (!reversed.empty()) { result.push(reversed.top()); reversed.pop(); }
    return result;
}

#endif