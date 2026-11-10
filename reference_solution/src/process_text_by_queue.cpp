#include "process_text_by_queue.h"
std::string process_text_by_queue(const std::string& source) {
    utec::queue<char> pending;
    std::string result;
    for (char c : source) {
        if (c != '*') pending.push(c);
        else if (!pending.empty()) { result += pending.front(); pending.pop(); }
    }
    return result;
}
