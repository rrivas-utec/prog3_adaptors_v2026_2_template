#pragma once
#include "catch.hpp"
#include "merge_stacks.h"
#include <vector>
#include <string>
#include <deque>
#include <stdexcept>
#include <random>
#include <type_traits>
#include <future>
#include <chrono>
namespace {
template<class T> utec::stack<T> make_stack(const std::vector<T>& xs) {
    utec::stack<T> result;
    for (auto it = xs.rbegin(); it != xs.rend(); ++it) result.push(*it);
    return result;
}
template<class T> std::vector<T> drain(utec::stack<T> xs) {
    std::vector<T> result;
    while (!xs.empty()) { result.push_back(xs.top()); xs.pop(); }
    return result;
}
}
namespace {
template<class T> void check(const std::vector<T>& a, const std::vector<T>& b) {
    auto sa=make_stack(a), sb=make_stack(b);
    std::vector<T> expected;
    for (std::size_t i=0; i<a.size() || i<b.size(); ++i) {
        if (i<b.size()) expected.push_back(b[i]);
        if (i<a.size()) expected.push_back(a[i]);
    }
    REQUIRE(drain(merge_stacks(sa,sb)) == expected);
    REQUIRE(drain(sa) == a);
    REQUIRE(drain(sb) == b);
}
}
#if TEST_CASE_INDEX == 1
TEST_CASE("Q1.1 - Basico 1.00x") {
check<int>({},{}); check<int>({1},{}); check<int>({}, {2});
check<int>({1,2,3,4},{10,20}); check<int>({1},{10});
}
#elif TEST_CASE_INDEX == 2
TEST_CASE("Q1.2 - Intermedio 1.25x") {
for (int n=0;n<13;++n) for (int m=0;m<17;++m) {
    std::vector<int> a,b;
    for(int i=0;i<n;++i) a.push_back(i-5);
    for(int i=0;i<m;++i) b.push_back(100-i);
    check(a,b);
}
}
#elif TEST_CASE_INDEX == 3
TEST_CASE("Q1.3 - Avanzado 1.50x") {
check<std::string>({"first","repeat","last"},{"B","repeat","C","D"});
check<int>({4,4,-2,0,-2},{0,4,0});
auto a=make_stack<int>({1,2,3}); auto copy=a;
copy.top()=99; REQUIRE(a.top()==1);
const auto& view=a; REQUIRE(view.top()==1); REQUIRE(view.size()==3);
}
#elif TEST_CASE_INDEX == 4
TEST_CASE("Q1.4 - Intensivo 2.00x") {
std::mt19937 rng(2036);
for (int trial=0;trial<24;++trial) {
    std::vector<int> a(1000+rng()%3000), b(rng()%5000);
    for(auto& x:a) x=static_cast<int>(rng()%101)-50;
    for(auto& x:b) x=static_cast<int>(rng()%53)-26;
    check(a,b);
    check<int>({trial,-trial},{});
}

auto independent = [] { return drain(merge_stacks(make_stack<int>({1,2,3}),make_stack<int>({7,8}))); };
auto first = std::async(std::launch::async, independent);
auto second = std::async(std::launch::async, independent);
const auto first_status = first.wait_for(std::chrono::seconds(10));
const auto second_status = second.wait_for(std::chrono::seconds(10));
REQUIRE(first_status == std::future_status::ready);
REQUIRE(second_status == std::future_status::ready);
auto first_result = first.get();
auto second_result = second.get();
REQUIRE(first_result == (std::vector<int>{7,1,8,2,3}));
REQUIRE(second_result == (std::vector<int>{7,1,8,2,3}));
}
#else
#error "TEST_CASE_INDEX must be 1..4"
#endif
