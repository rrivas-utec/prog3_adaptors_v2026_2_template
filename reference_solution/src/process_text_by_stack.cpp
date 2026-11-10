#include "process_text_by_stack.h"
std::string process_text_by_stack(std::string source) {
    utec::stack<char> pending;
    std::string result;
    for (char c : source) {
        if (c != '*') pending.push(c);
        else if (!pending.empty()) { result += pending.top(); pending.pop(); }
    }
    return result;
}
