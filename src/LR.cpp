#include "LR.h"

LR_Rule::LR_Rule(const Rule& rule) {
    this->lhs = rule.lhs;
    this->rhs = rule.rhs;
    this->dot_place = 0;
    this->was_analyzed = false;
}

LR_Rule& LR_Rule::operator=(const Rule& rule) {
    this->lhs = rule.lhs;
    this->rhs = rule.rhs;
    this->dot_place = 0;
    this->was_analyzed = false;
    return *this;
}

bool LR_Rule::operator==(const LR_Rule& other) const {
    if (this->lhs != other.lhs) return false;
    if (this->rhs != other.rhs) return false;
    if (this->dot_place != other.dot_place) return false;
    return true;
}

void LR_Rule::print(std::ostream &os) const {
    os << lhs << " -> ";
    for (int i=0; i<(int)rhs.size(); i++) {
        if (dot_place == i) os << "* ";
        os << (std::string) rhs[i] << " ";
    }
    if (dot_place == (int)rhs.size()) os << "*";
    if (!was_analyzed) os << " [NOT ANALYZED]";
    os << std::endl;
}

LR_State::LR_State(const std::vector< LR_Rule >& rls): rules(rls) {
    index = 0;
}

void LR_State::print(const LR_Collection& C, std::ostream &os) const {
    os << "I_" << index;
    if (index != 0) {
        for (int ref_idx: refs) {
            auto &ref = C.states[ref_idx];
            os << " = goto( I_" << ref.index << ", " 
                << (std::string)ref_element << " )";
        }
    }
    os << std::endl;
    for (auto &rule: rules) {
        os << '\t';
        rule.print(os);
    }
    os << std::endl;
}

bool LR_State::operator==(const LR_State& other) const {
    if (ref_element != other.ref_element) return false;
    if (rules.size() != other.rules.size()) return false;
    if (rules != other.rules) return false;
    return true;
}

LR_State* LR_Collection::has_same_state(const LR_State& ref) const {
    for (const LR_State &state: this->states) {
        if (state == ref) return (& const_cast<LR_State&>(state) );
    }
    return nullptr;
}

LR_State* LR_Collection::has_same_ref(const LR_State& ref) const {
    for (const LR_State &state: this->states) {
        if (state.ref_element == ref.ref_element) {
            bool has_intersection = false;
            for (auto &r1: state.refs) {
                for (auto &r2: ref.refs) {
                    if (r1 == r2) {
                        has_intersection = true;
                        break;
                    }
                }
                if (has_intersection) break;
            }
            if (has_intersection) return (& const_cast<LR_State&>(state) );
        } 
    }
    return nullptr;
}

LR_Collection LR_Collection::from_grammar(const Grammar& G) {
    LR_Collection C;
    
    LR_Rule base_rule = G.rules[0];
    LR_State base_state({base_rule});
    {
        std::vector<LR_Rule> cls;
        closure(G, base_rule, cls);

        base_state.rules.insert(base_state.rules.end(), cls.begin(), cls.end());
    }
    base_state.ref_element.type = Element::NONTERM;
    base_state.ref_element.nt = "S";
    base_state.index = 0;
    C.states.push_back(base_state);


    bool finished = false;
    bool new_state_added = false;
    while (!finished) {
        finished = true;
        new_state_added = false;
        int C_states_size = C.states.size();
        for (int i_state=0; i_state<C_states_size; i_state++) {
            LR_State &state = C.states[i_state];

            for (int i_rule=0; i_rule < (int)state.rules.size(); i_rule++) {
                LR_Rule &rule = state.rules[i_rule];
                // rule.print();
                if (rule.dot_place == (int)rule.rhs.size()) rule.was_analyzed = true;
                if (rule.was_analyzed) continue;
                // not analyzed
                finished = false;

                Element eaten = rule.rhs[rule.dot_place];

                std::vector<LR_Rule> new_rules;
                LR_Rule new_rule = rule;
                new_rule.dot_place = rule.dot_place+1;
                new_rules.push_back(new_rule);

                if (new_rule.dot_place != (int)new_rule.rhs.size()) {

                    std::vector<LR_Rule> cls;
                    closure(G, new_rule, cls);

                    new_rules.insert(new_rules.end(), cls.begin(), cls.end());                    
                }

                rule.was_analyzed = true;

                LR_State new_state(new_rules);
                new_state.ref_element = eaten;
                new_state.refs.push_back(state.index);
                new_state.index = C.states.size();
                // new_state.print(C);

                LR_State *same_ref = C.has_same_ref(new_state);
                if (same_ref) {
                    std::vector<LR_Rule> to_add;
                    for (auto &n: new_state.rules) {
                        bool is_found = false;
                        for (auto &r: same_ref->rules) {
                            if (n.lhs == r.lhs && n.rhs == r.rhs) {
                                is_found = true;
                                break;
                            }
                        }
                        if (is_found) continue;
                        for (auto &a: to_add) {
                            if (n.lhs == a.lhs && n.rhs == a.rhs) {
                                is_found = true;
                                break;
                            }
                        }
                        if (is_found) continue;
                        to_add.push_back(n);
                    }
                    same_ref->rules.insert(same_ref->rules.end(), to_add.begin(), to_add.end());
                    new_state_added = true;

                    break;
                }

                LR_State *same_state = C.has_same_state(new_state);
                if (same_state != nullptr) {
                    same_state->refs.push_back(state.index);
                } else {
                    C.states.push_back(new_state);
                    new_state_added = true;
                    break;
                }
            }
            if (new_state_added) break;
        }
    }
    return C;
}

void closure(const Grammar& G, const LR_Rule& rule, std::vector<LR_Rule> &result) {


    if (rule.dot_place >= (int)rule.rhs.size()) return;
    const Element &after_dot = rule.rhs[rule.dot_place];
    if (after_dot.type != Element::NONTERM) return;
  
    for (const auto &g_rule: G.rules) {
        if (g_rule.lhs == after_dot.nt.value()) {
            LR_Rule new_rule(g_rule);
            if (new_rule.lhs == rule.lhs && new_rule.rhs == rule.rhs) continue;
            bool found_flag = false;
            for (const auto &r: result) {
                if (r.lhs == new_rule.lhs && r.rhs == new_rule.rhs) {
                    found_flag = true; break;
                }
            }
            if (found_flag) continue;

            result.push_back(new_rule);
            // new_rule.print();

            closure(G, new_rule, result);
        }
    }
}