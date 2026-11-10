#include "browser_history.h"
BrowserMemento::BrowserMemento(const std::string& url, int scroll) : url_(url), scroll_(scroll) {}
Browser::Browser(const std::string& initial_url) : url_(initial_url), scroll_(0) {}
void Browser::visit(const std::string& url) { url_ = url; scroll_ = 0; }
void Browser::set_scroll(int position) { scroll_ = position; }
std::string Browser::current() const { return url_; }
int Browser::scroll() const { return scroll_; }
BrowserMemento Browser::save() const { return BrowserMemento(url_, scroll_); }
void Browser::restore(const BrowserMemento& state) { url_ = state.url_; scroll_ = state.scroll_; }
BrowserHistory::BrowserHistory(Browser& browser) : browser_(browser) {}
void BrowserHistory::visit(const std::string& url) {
    backward_.push(browser_.save());
    while (!forward_.empty()) forward_.pop();
    browser_.visit(url);
}
bool BrowserHistory::back() {
    if (backward_.empty()) return false;
    forward_.push(browser_.save());
    browser_.restore(backward_.top()); backward_.pop(); return true;
}
bool BrowserHistory::forward() {
    if (forward_.empty()) return false;
    backward_.push(browser_.save());
    browser_.restore(forward_.top()); forward_.pop(); return true;
}
