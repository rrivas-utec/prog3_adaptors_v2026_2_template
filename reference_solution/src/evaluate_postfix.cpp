#include "evaluate_postfix.h"
#include <sstream>
#include <stdexcept>
int evaluate_postfix(const std::string& source) {
    utec::stack<int> values;
    std::istringstream input(source);
    std::string token;
    while (input >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (values.size() < 2) throw std::invalid_argument("missing operands");
            int right = values.top(); values.pop();
            int left = values.top(); values.pop();
            if (token == "/" && right == 0) throw std::domain_error("division by zero");
            values.push(token == "+" ? left + right : token == "-" ? left - right : token == "*" ? left * right : left / right);
        } else {
            std::size_t start = (token[0] == '+' || token[0] == '-') ? 1 : 0;
            if (start == token.size()) throw std::invalid_argument("invalid integer");
            for (auto i = start; i < token.size(); ++i)
                if (token[i] < '0' || token[i] > '9') throw std::invalid_argument("invalid token");
            try { values.push(std::stoi(token)); }
            catch (const std::out_of_range&) { throw std::invalid_argument("integer outside contract"); }
        }
    }
    if (values.size() != 1) throw std::invalid_argument("expected one result");
    return values.top();
}
