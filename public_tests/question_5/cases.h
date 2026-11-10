#pragma once
#include "catch.hpp"
#include "is_equation_balanced.h"
#include <vector>
#include <string>
#include <deque>
#include <stdexcept>
#include <random>
#include <type_traits>
#include <future>
#include <chrono>
namespace {
bool oracle(const std::string& text) {
    std::string opens;
    for(char c:text) {
        const auto i=std::string("([{ ").find(c);
        const auto j=std::string(")]}").find(c);
        if(i<3) opens+=c;
        else if(j!=std::string::npos) {
            if(opens.empty() || opens.back()!=std::string("([{")[j]) return false;
            opens.pop_back();
        }
    }
    return opens.empty();
}
}
#if TEST_CASE_INDEX == 1
TEST_CASE("Q5.1 - Basico 1.00x") {
for (auto s : {"", "abc + 12", "()", "[]", "{}", "{ [ (4 - 5) * 3] + 14}"}) REQUIRE(is_equation_balanced(s));
for (auto s : {"(", ")", "[", "]", "{", "}"}) REQUIRE_FALSE(is_equation_balanced(s));
}
#elif TEST_CASE_INDEX == 2
TEST_CASE("Q5.2 - Intermedio 1.25x") {
for(auto s : {"()[]{}", "((10 + 20) + 30)", "{a[b(c)d]e}"}) REQUIRE(is_equation_balanced(s));
for(auto s : {"([)]", "{ (6 + 20} + 4)", "(()", "())", "}{", "(]", "[}", "{)"}) REQUIRE_FALSE(is_equation_balanced(s));
}
#elif TEST_CASE_INDEX == 3
TEST_CASE("Q5.3 - Avanzado 1.50x") {
const std::string alphabet="()[]{}x";
for(int code=0;code<16807;++code) {
    int n=code; std::string s;
    for(int j=0;j<5;++j) { s+=alphabet[n%7]; n/=7; }
    REQUIRE(is_equation_balanced(s)==oracle(s));
}
}
#elif TEST_CASE_INDEX == 4
TEST_CASE("Q5.4 - Intensivo 2.00x") {
for(int repeat=0;repeat<12;++repeat) {
    std::string s; for(int i=0;i<10000;++i) s+="([{x";
    for(int i=0;i<10000;++i) s+="}])";
    REQUIRE(is_equation_balanced(s));
    s.back()=']'; REQUIRE_FALSE(is_equation_balanced(s));
    REQUIRE(is_equation_balanced("()"));
}

auto independent = [] { return is_equation_balanced("{a[b(c)]}"); };
auto first = std::async(std::launch::async, independent);
auto second = std::async(std::launch::async, independent);
const auto first_status = first.wait_for(std::chrono::seconds(10));
const auto second_status = second.wait_for(std::chrono::seconds(10));
REQUIRE(first_status == std::future_status::ready);
REQUIRE(second_status == std::future_status::ready);
auto first_result = first.get();
auto second_result = second.get();
REQUIRE(first_result == (true));
REQUIRE(second_result == (true));
}
#else
#error "TEST_CASE_INDEX must be 1..4"
#endif
