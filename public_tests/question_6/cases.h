#pragma once
#include "catch.hpp"
#include "browser_history.h"
#include <vector>
#include <string>
#include <deque>
#include <stdexcept>
#include <random>
#include <type_traits>
#include <future>
#include <chrono>
namespace {
void state(const Browser& b,const std::string& url,int scroll) {
    REQUIRE(b.current()==url); REQUIRE(b.scroll()==scroll);
}
static_assert(!std::is_default_constructible_v<BrowserMemento>);
static_assert(!std::is_constructible_v<BrowserMemento,const std::string&,int>);
static_assert(std::is_copy_constructible_v<BrowserMemento>);
static_assert(std::is_copy_assignable_v<BrowserMemento>);
}
#if TEST_CASE_INDEX == 1
TEST_CASE("Q6.1 - Basico 1.00x") {
Browser b("inicio"); BrowserHistory h(b); state(b,"inicio",0);
REQUIRE_FALSE(h.back()); REQUIRE_FALSE(h.forward()); state(b,"inicio",0);
b.set_scroll(120); h.visit("cursos"); state(b,"cursos",0);
REQUIRE(h.back()); state(b,"inicio",120); REQUIRE_FALSE(h.back()); state(b,"inicio",120);
REQUIRE(h.forward()); state(b,"cursos",0); REQUIRE_FALSE(h.forward());
}
#elif TEST_CASE_INDEX == 2
TEST_CASE("Q6.2 - Intermedio 1.25x") {
Browser b("a"); b.set_scroll(17); auto snapshot=b.save(); auto copy=snapshot;
b.visit("b"); b.set_scroll(22); auto other=b.save(); other=snapshot;
b.restore(copy); state(b,"a",17); b.visit("c"); b.restore(other); state(b,"a",17);
BrowserHistory h(b); h.visit("b"); b.set_scroll(32); h.visit("c");
REQUIRE(h.back()); state(b,"b",32); REQUIRE(h.back()); state(b,"a",17);
REQUIRE(h.forward()); state(b,"b",32); REQUIRE(h.forward()); state(b,"c",0);
}
#elif TEST_CASE_INDEX == 3
TEST_CASE("Q6.3 - Avanzado 1.50x") {
Browser b("a"); BrowserHistory h(b);
b.set_scroll(10); h.visit("same"); b.set_scroll(20); h.visit("same"); b.set_scroll(30);
REQUIRE(h.back()); state(b,"same",20); b.set_scroll(21);
REQUIRE(h.forward()); state(b,"same",30);
REQUIRE(h.back()); state(b,"same",21); h.visit("branch");
REQUIRE_FALSE(h.forward()); state(b,"branch",0);
REQUIRE(h.back()); state(b,"same",21); REQUIRE(h.back()); state(b,"a",10);
Browser independent("other"); BrowserHistory separate(independent);
separate.visit("new"); REQUIRE(separate.back()); state(independent,"other",0); state(b,"a",10);
}
#elif TEST_CASE_INDEX == 4
TEST_CASE("Q6.4 - Intensivo 2.00x") {
using S=std::pair<std::string,int>;
Browser b("start"); BrowserHistory h(b); S current{"start",0};
std::vector<S> back,forward; std::mt19937 rng(6006);
for(int i=0;i<20000;++i) {
    switch(rng()%4) {
    case 0: { auto url="page"+std::to_string(rng()%37); back.push_back(current); forward.clear(); current={url,0}; h.visit(url); break; }
    case 1: { int y=static_cast<int>(rng()%100000); current.second=y; b.set_scroll(y); break; }
    case 2: { bool ok=!back.empty(); REQUIRE(h.back()==ok); if(ok) { forward.push_back(current); current=back.back(); back.pop_back(); } break; }
    case 3: { bool ok=!forward.empty(); REQUIRE(h.forward()==ok); if(ok) { back.push_back(current); current=forward.back(); forward.pop_back(); } break; }
    }
    state(b,current.first,current.second);
}
while(!back.empty()) { REQUIRE(h.back()); forward.push_back(current); current=back.back(); back.pop_back(); state(b,current.first,current.second); }
REQUIRE_FALSE(h.back()); state(b,current.first,current.second);

auto independent = [] { Browser b("own"); BrowserHistory h(b); b.set_scroll(123); h.visit("next"); bool ok=h.back(); return ok && b.current()=="own" && b.scroll()==123 && !h.back(); };
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
