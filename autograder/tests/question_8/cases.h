#pragma once
#include "catch.hpp"
#include "evaluate_postfix.h"
#include <vector>
#include <string>
#include <deque>
#include <stdexcept>
#include <random>
#include <type_traits>
#include <future>
#include <chrono>

#if TEST_CASE_INDEX == 1
TEST_CASE("Q8.1 - Basico 1.00x") {
REQUIRE(evaluate_postfix("0")==0); REQUIRE(evaluate_postfix("+17")==17);
REQUIRE(evaluate_postfix("10 20 + 3 *")==90); REQUIRE(evaluate_postfix("2 3 4 + *")==14);
REQUIRE_THROWS_AS(evaluate_postfix(""),std::invalid_argument);
REQUIRE_THROWS_AS(evaluate_postfix("   "),std::invalid_argument);
}
#elif TEST_CASE_INDEX == 2
TEST_CASE("Q8.2 - Intermedio 1.25x") {
REQUIRE(evaluate_postfix("  10   20 +   ")==30);
REQUIRE(evaluate_postfix("-3 4 * 2 +")==-10);
for(auto s : {"+", "-", "*", "/", "10 +", "10 20", "10 dos +", "2 3 + *", "1x", "1.0", "0x10", "--1", "++2", "2 +3x +"}) REQUIRE_THROWS_AS(evaluate_postfix(s),std::invalid_argument);
REQUIRE_THROWS_AS(evaluate_postfix("1 0 /"),std::domain_error);
}
#elif TEST_CASE_INDEX == 3
TEST_CASE("Q8.3 - Avanzado 1.50x") {
REQUIRE(evaluate_postfix("8 3 -")==5); REQUIRE(evaluate_postfix("3 8 -")==-5);
REQUIRE(evaluate_postfix("20 5 / 2 -")==2); REQUIRE(evaluate_postfix("-7 3 /")==-2);
REQUIRE(evaluate_postfix("7 -3 /")==-2); REQUIRE(evaluate_postfix("-7 -3 /")==2);
REQUIRE(evaluate_postfix("100 20 5 / -")==96);
REQUIRE_THROWS_AS(evaluate_postfix("99 10 0 / +"),std::domain_error);
REQUIRE_THROWS_AS(evaluate_postfix("99 1 bad +"),std::invalid_argument);
REQUIRE(evaluate_postfix("9 3 /")==3);
}
#elif TEST_CASE_INDEX == 4
TEST_CASE("Q8.4 - Intensivo 2.00x") {
std::mt19937 rng(8008);
for(int trial=0;trial<40;++trial) {
    int expected=0; std::string expression="0";
    for(int i=0;i<2000;++i) {
        int value=static_cast<int>(rng()%201)-100; bool add=rng()%2;
        expression+=" "+std::to_string(value)+(add ? " +" : " -");
        expected=add ? expected+value : expected-value;
    }
    REQUIRE(evaluate_postfix(expression)==expected);
    REQUIRE_THROWS_AS(evaluate_postfix(expression+" 0 /"),std::domain_error);
    REQUIRE(evaluate_postfix("42")==42);
}
std::string expr; for(int i=0;i<10000;++i) expr+="1 "; for(int i=1;i<10000;++i) expr+="+ ";
REQUIRE(evaluate_postfix(expr)==10000);

auto independent = [] { return evaluate_postfix("100 20 5 / -"); };
auto first = std::async(std::launch::async, independent);
auto second = std::async(std::launch::async, independent);
const auto first_status = first.wait_for(std::chrono::seconds(10));
const auto second_status = second.wait_for(std::chrono::seconds(10));
REQUIRE(first_status == std::future_status::ready);
REQUIRE(second_status == std::future_status::ready);
auto first_result = first.get();
auto second_result = second.get();
REQUIRE(first_result == (96));
REQUIRE(second_result == (96));
}
#else
#error "TEST_CASE_INDEX must be 1..4"
#endif
