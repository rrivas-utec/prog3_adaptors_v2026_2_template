#pragma once
#include "catch.hpp"
#include "validate_tags.h"
#include <vector>
#include <string>
#include <deque>
#include <stdexcept>
#include <random>
#include <type_traits>
#include <future>
#include <chrono>

#if TEST_CASE_INDEX == 1
TEST_CASE("Q7.1 - Basico 1.00x") {
for(auto s : {"", "texto sin etiquetas", "<a></a>", "<libro><titulo>Hola</titulo></libro>", "<a></a><b></b>"}) REQUIRE(validate_tags(s));
for(auto s : {"<a>", "</a>", "<a><b></a></b>", "<A></a>"}) REQUIRE_FALSE(validate_tags(s));
}
#elif TEST_CASE_INDEX == 2
TEST_CASE("Q7.2 - Intermedio 1.25x") {
for(auto s : {"<a/>", "<a x>", "< a>", "<a >", "</ a>", "<>", "</>", "<!--x-->", "<a1></a1>", "<a_b></a_b>", "<a-b></a-b>", "<a", "abc>", "<<a>>", "<a><", "<a></a>>"}) REQUIRE_FALSE(validate_tags(s));
REQUIRE(validate_tags("antes<ABC>texto<z></z></ABC>despues"));
}
#elif TEST_CASE_INDEX == 3
TEST_CASE("Q7.3 - Avanzado 1.50x") {
for(auto s : {"<a><a></a></a>", "<Ab><cD></cD><E></E></Ab>", "<a></a>texto<a></a>"}) REQUIRE(validate_tags(s));
for(auto s : {"<ab></a>", "<a><b></b>", "<a></a></a>", "<a></A>", "<a><b></a></b>", "<a>text>more</a>", "<a></a/>", "<a></a x>"}) REQUIRE_FALSE(validate_tags(s));
for(int i=0;i<100;++i) { REQUIRE_FALSE(validate_tags("<a>")); REQUIRE(validate_tags("<b></b>")); }
}
#elif TEST_CASE_INDEX == 4
TEST_CASE("Q7.4 - Intensivo 2.00x") {
for(int trial=0;trial<10;++trial) {
    std::vector<std::string> tags; std::string s;
    for(int i=0;i<5000;++i) { tags.push_back(i%3==0 ? "Alpha" : i%3==1 ? "b" : "Gamma"); s+="<"+tags.back()+">text"; }
    auto open=s;
    for(auto it=tags.rbegin();it!=tags.rend();++it) s+="</"+*it+">";
    REQUIRE(validate_tags(s)); REQUIRE_FALSE(validate_tags(open));
    s.replace(s.size()-8,8,"</Other>"); REQUIRE_FALSE(validate_tags(s));
    std::string siblings; for(int i=0;i<3000;++i) siblings+="<a></a>txt";
    REQUIRE(validate_tags(siblings));
}

auto independent = [] { return validate_tags("<a><B></B></a><c></c>"); };
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
