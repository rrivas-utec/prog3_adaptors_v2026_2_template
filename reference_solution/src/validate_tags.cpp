#include "validate_tags.h"
bool validate_tags(const std::string& source) {
    utec::stack<std::string> pending;
    std::size_t i = 0;
    while (i < source.size()) {
        if (source[i] == '>') return false;
        if (source[i] != '<') { ++i; continue; }
        ++i;
        bool closing = i < source.size() && source[i] == '/';
        if (closing) ++i;
        const auto start = i;
        while (i < source.size() && ((source[i] >= 'a' && source[i] <= 'z') || (source[i] >= 'A' && source[i] <= 'Z'))) ++i;
        if (i == start || i == source.size() || source[i] != '>') return false;
        const auto name = source.substr(start, i - start);
        ++i;
        if (closing) {
            if (pending.empty() || pending.top() != name) return false;
            pending.pop();
        } else pending.push(name);
    }
    return pending.empty();
}
