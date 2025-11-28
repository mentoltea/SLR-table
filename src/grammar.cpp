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

void Grammar::describe(std::ostream &os) const {
    os << "#TERMS" << std::endl;
    for (auto& t: terms) {
        os <<  t << " ";
    }
    os << std::endl;
    
    os << "#NONTERMS" << std::endl;
    for (auto& nt: nonterm) {
        os << nt << " ";
    }
    os << std::endl;
    
    os << "# Rules" << std::endl;
    int i = 0;
    for (auto& rule: rules) {
        os << "# " << i << ". " << rule.lhs << " -> ";
        if (rule.rhs.empty()) os << "^";
        for (auto &el: rule.rhs) {
            os << (std::string) el << " ";
        }
        os << std::endl;

        // if (i == 0) continue;
        os << rule.lhs << " ";
        for (auto &el: rule.rhs) {
            os << (std::string) el << " ";
        }
        os << std::endl;

        i++;
    }
}

Grammar::FollowTable Grammar::followtable() const {
    FollowTable ft;

    for (auto &nt: nonterm) {
        auto f = follow(nt);
        ft.insert( {nt, f} );
    }

    return ft;
}

std::set<Terminal> Grammar::follow(const NonTerminal& nt) const {
    // std::cout << "[Follow] " << nt << std::endl;

    std::set<Terminal> result;

    Element ref;
    ref.type = Element::NONTERM;
    ref.nt = nt;

    for (auto &rule: rules) {
        std::set<Terminal> current_follow;
        for (int i=0; i<(int)rule.rhs.size(); i++) {
            if (rule.rhs[i] == ref) {
                if (i+1 == (int)rule.rhs.size()) {
                    if (rule.lhs == "S") {
                        current_follow.insert("$");
                    }
                    auto flw = follow(rule.lhs);
                    current_follow.insert(flw.begin(), flw.end());
                } else {
                    auto &next = rule.rhs[i+1];
                    if (next.type == Element::TERM) current_follow.insert(next.t.value());
                    else {
                        auto fst = first(next.nt.value());
                        current_follow.insert(fst.begin(), fst.end());
                    }
                }
            }
        }

        result.insert(current_follow.begin(), current_follow.end());
        current_follow.clear();
    }

    return result;
}

std::set<Terminal> Grammar::first(const NonTerminal& nt) const {
    std::set<Terminal> result;

    for (auto &rule: rules) {
        std::set<Terminal> current_first;
        if (rule.lhs == nt) {
            if (rule.rhs.size() == 0) continue;
            auto &rho = rule.rhs[0];
            if (rho.type == Element::TERM) current_first.insert(rho.t.value());
            else {
                if (rho.nt.value() == nt) continue;
                auto fst = first(rho.nt.value());
                current_first.insert(fst.begin(), fst.end());
            }
        }
        result.insert(current_first.begin(), current_first.end());
    }

    return result;
}