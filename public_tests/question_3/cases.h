#pragma once
#include "catch.hpp"
#include "process_text_by_stack.h"
#include <vector>
#include <string>
#include <deque>
#include <stdexcept>
#include <random>
#include <type_traits>
#include <future>
#include <chrono>
namespace {
std::string oracle(const std::string& s) {
    std::deque<char> pending; std::string out;
    for(char c:s) {
        if(c!='*') pending.push_back(c);
        else if(!pending.empty()) { out += pending.back(); pending.pop_back(); }
    }
    return out;
}
void check(const std::string& s) { REQUIRE(process_text_by_stack(s)==oracle(s)); }
}
#if TEST_CASE_INDEX == 1
TEST_CASE("Q3.1 - Basico 1.00x") {
REQUIRE(process_text_by_stack("")==""); REQUIRE(process_text_by_stack("***")=="");
REQUIRE(process_text_by_stack("ABC")==""); REQUIRE(process_text_by_stack("ABC**D*")=="CBD");
REQUIRE(process_text_by_stack("EAS*Y*QUE***ST***IO*N***")=="SYEUQTSAONIE");
}
#elif TEST_CASE_INDEX == 2
TEST_CASE("Q3.2 - Intermedio 1.25x") {
for(int n=0;n<30;++n) for(int m=0;m<40;++m) {
    std::string s(n,'A'); s+=std::string(m,'*'); s+="BC**DEF*"; check(s);
}
check("***A**BC*D****E"); check("AAAA**BB***CC");
}
#elif TEST_CASE_INDEX == 3
TEST_CASE("Q3.3 - Avanzado 1.50x") {
check("ABC**D*EF***G****HI**"); check("aZbY***C**D*E");
for(int i=0;i<50;++i) { check("AB*C**DE***F"); check("Z*"); check("XYZ"); }
}
#elif TEST_CASE_INDEX == 4
TEST_CASE("Q3.4 - Intensivo 2.00x") {
std::mt19937 rng(3004);
for(int trial=0;trial<20;++trial) {
    std::string s; s.reserve(20000);
    for(int i=0;i<20000;++i) s+=rng()%5<2 ? '*' : static_cast<char>('A'+rng()%26);
    check(s); check(std::string(4000,'x')+std::string(5000,'*'));
}

auto independent = [] { return process_text_by_stack("ABC**D*"); };
auto first = std::async(std::launch::async, independent);
auto second = std::async(std::launch::async, independent);
const auto first_status = first.wait_for(std::chrono::seconds(10));
const auto second_status = second.wait_for(std::chrono::seconds(10));
REQUIRE(first_status == std::future_status::ready);
REQUIRE(second_status == std::future_status::ready);
auto first_result = first.get();
auto second_result = second.get();
REQUIRE(first_result == (std::string("CBD")));
REQUIRE(second_result == (std::string("CBD")));
}
#else
#error "TEST_CASE_INDEX must be 1..4"
#endif
