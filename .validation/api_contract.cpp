#include "../reference_solution/solution.h"
#include <type_traits>
#include <string>

static_assert(std::is_same_v<decltype(&merge_stacks<int>), utec::stack<int>(*)(utec::stack<int>,utec::stack<int>)>);
static_assert(std::is_same_v<decltype(&join_stacks<std::string>), utec::stack<std::string>(*)(utec::stack<std::string>,utec::stack<std::string>)>);
static_assert(std::is_same_v<decltype(&process_text_by_stack),std::string(*)(std::string)>);
static_assert(std::is_same_v<decltype(&process_text_by_queue),std::string(*)(const std::string&)>);
static_assert(std::is_same_v<decltype(&is_equation_balanced),bool(*)(std::string)>);
static_assert(std::is_same_v<decltype(&validate_tags),bool(*)(const std::string&)>);
static_assert(std::is_same_v<decltype(&evaluate_postfix),int(*)(const std::string&)>);
static_assert(std::is_same_v<decltype(&Browser::visit),void(Browser::*)(const std::string&)>);
static_assert(std::is_same_v<decltype(&Browser::set_scroll),void(Browser::*)(int)>);
static_assert(std::is_same_v<decltype(&Browser::current),std::string(Browser::*)() const>);
static_assert(std::is_same_v<decltype(&Browser::scroll),int(Browser::*)() const>);
static_assert(std::is_same_v<decltype(&Browser::save),BrowserMemento(Browser::*)() const>);
static_assert(std::is_same_v<decltype(&Browser::restore),void(Browser::*)(const BrowserMemento&)>);
static_assert(std::is_same_v<decltype(&BrowserHistory::visit),void(BrowserHistory::*)(const std::string&)>);
static_assert(std::is_same_v<decltype(&BrowserHistory::back),bool(BrowserHistory::*)()>);
static_assert(std::is_same_v<decltype(&BrowserHistory::forward),bool(BrowserHistory::*)()>);
template<class T> concept public_state = requires(T t) { t.url_; t.scroll_; };
static_assert(!public_state<BrowserMemento>);
