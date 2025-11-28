#include "grammar.h"

bool Element::operator==(const Element& other) const {
    if (this->type != other.type) return false;
    if (this->type == Element::TERM) {
        if (this->t.value() != other.t.value()) return false;
    } else {
        if (this->nt.value() != other.nt.value()) return false;
    }
    return true;
}

bool Element::operator!=(const Element& other) const {
    return !(*this == other);
}

Element::operator std::string() const {
    if (type == Element::TERM) return t.value();
    if (type == Element::NONTERM) return nt.value();
    assert(0 && "Unreachable");
}

void Grammar::describe() const {
    std::cout << "Terminals:" << std::endl;
    for (auto& t: terms) {
        std::cout << "`" << t << "` ";
    }
    std::cout << std::endl;
    
    std::cout << "Non-Terminals:" << std::endl;
    for (auto& nt: nonterm) {
        std::cout << "`" << nt << "` ";
    }
    std::cout << std::endl;
    
    std::cout << "Rules:" << std::endl;
    int i = 0;
    for (auto& rule: rules) {
        std::cout << i << ". " << rule.lhs << " -> ";
        if (rule.rhs.empty()) std::cout << "^";
        for (auto &el: rule.rhs) {
            std::cout << (std::string) el << " ";
        }
        std::cout << std::endl;
        i++;
    }
}