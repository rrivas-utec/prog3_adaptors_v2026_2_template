#ifndef BROWSER_HISTORY_H
#define BROWSER_HISTORY_H

#include <string>
#include "stack_utec.h"
class BrowserMemento {
    friend class Browser;
    std::string url_;
    int scroll_;
    BrowserMemento(const std::string& url, int scroll);
public:
    BrowserMemento(const BrowserMemento&) = default;
    BrowserMemento& operator=(const BrowserMemento&) = default;
};
class Browser {
    std::string url_;
    int scroll_;
public:
    explicit Browser(const std::string& initial_url);
    void visit(const std::string& url);
    void set_scroll(int position);
    std::string current() const;
    int scroll() const;
    BrowserMemento save() const;
    void restore(const BrowserMemento& state);
};
class BrowserHistory {
    Browser& browser_;
    utec::stack<BrowserMemento> backward_, forward_;
public:
    explicit BrowserHistory(Browser& browser);
    void visit(const std::string& url);
    bool back();
    bool forward();
};

#endif
