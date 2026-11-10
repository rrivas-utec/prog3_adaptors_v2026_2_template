#include "is_equation_balanced.h"
bool is_equation_balanced(std::string source) {
    utec::stack<char> pending;
    for (char c : source) {
        if (c == '(' || c == '[' || c == '{') pending.push(c);
        else if (c == ')' || c == ']' || c == '}') {
            const char expected = c == ')' ? '(' : c == ']' ? '[' : '{';
            if (pending.empty() || pending.top() != expected) return false;
            pending.pop();
        }
    }
    return pending.empty();
}
