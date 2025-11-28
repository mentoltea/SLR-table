#include "SLR.h"

void Action::print(std::ostream &os) const {
    if (type == ACC) {
        os << "ACC" << std::endl;
        return;
    }
    if (type == SHIFT) {
        os << "Shift " << data.s << std::endl;
        return;
    }
    if (type == REDUCE) {
        os << "Reduce " << data.r << std::endl;
        return;
    }
}

void SLR_State::add_action(const Terminal& t, const Action& a) {
    if (Actions.contains(t)) {
        auto &conflict_action = Actions.at(t); 
        std::cerr << "SLR conflict" << std::endl;
        std::cerr << "State " << index << std::endl;
        std::cerr << "Actions:" << std::endl;
        std::cerr << "\t" << t << " : "; 
            conflict_action.print(std::cerr);
        std::cerr << std::endl;
        std::cerr << "\t" << t << " : "; 
            a.print(std::cerr);
        std::cerr << std::endl;
        throw std::runtime_error("Action conflict");
    }
    Actions.insert( {t, a} );
}

void SLR_State::print(std::ostream &os) const {
    os << "i_" << index << std::endl;
    os << "Actions:" << std::endl;
    for (auto &act: Actions) {
        os << "\t" << act.first << " : ";
        act.second.print(os);
    }
    os << "Gotos:" << std::endl;
    for (auto &gt: Gotos) {
        os << "\t" << gt.first << " : " << gt.second << std::endl;
    }
}

void SLR_State::add_goto(const NonTerminal& nt, Shift idx) {
    if (Gotos.contains(nt)) {
        auto conflict = Gotos.at(nt); 
        std::cerr << "SLR conflict" << std::endl;
        std::cerr << "State " << index << std::endl;
        std::cerr << "Goto:" << std::endl;
        std::cerr << "\t" << nt << " : " << conflict << std::endl;
        std::cerr << "\t" << nt << " : " << idx << std::endl;
        throw std::runtime_error("Goto conflict");
    }
    Gotos.insert( {nt, idx} );
}

SLR_Table SLR_Table::from_LR_collection(const Grammar& G, const LR_Collection& C) {
    SLR_Table SLR;
    Grammar::FollowTable ft = G.followtable();

    for (auto &lr_state: C.states) {
        SLR_State slr_state;
        slr_state.index = lr_state.index;

        for (auto &rule: lr_state.rules) {
            if (rule.dot_place != (int)rule.rhs.size()) {
                // A → α • aβ
                auto &a = rule.rhs[rule.dot_place];
                if (a.type == Element::NONTERM) continue;

                int refto_idx = -1;
                for (auto &temp: C.states) {
                    if (temp.ref_element == a) {
                        auto it = std::find(temp.refs.begin(), temp.refs.end(), lr_state.index);
                        if (it != temp.refs.end()) {
                            refto_idx = temp.index;
                            break;
                        }
                    }
                }

                if (refto_idx == -1) {
                    std::cerr << "LR collection is not full" << std::endl;
                    std::cerr << "No continuation for rule" << std::endl;
                    std::cerr << "\t"; rule.print(std::cerr);
                    std::cerr << "of state" << std::endl;
                    lr_state.print(C, std::cerr);
                    throw std::runtime_error("Incomplete LR collection");
                }

                Action act;
                act.type = Action::SHIFT;
                act.data.s = refto_idx;
                
                slr_state.add_action(a.t.value(), act);
            } else {
                // A → α •
                if (rule.lhs == "S") {
                    // S′ → S • $
                    Action act;
                    act.type = Action::ACC;
                    slr_state.add_action("$", act);
                    continue;
                } 
                
                int reduce_idx = 0;
                for (auto &g_rule: G.rules) {
                    if (g_rule.lhs == rule.lhs && g_rule.rhs == rule.rhs) {
                        break; 
                    }
                    reduce_idx++;
                }
                
                if (reduce_idx == (int)G.rules.size()) {
                    std::cerr << "Grammar does not contain rule" << std::endl;
                    std::cerr << "\t"; rule.print(std::cerr);
                    std::cerr << std::endl;
                    throw std::runtime_error("Grammar and LR collection inconsistency");
                }
                Action act;
                act.type = Action::REDUCE;
                act.data.r = reduce_idx;
                
                auto &flw = ft.at(rule.lhs);
                for (auto &term: flw) {
                    slr_state.add_action(term, act);
                }
            }
        }

        SLR.states.push_back(slr_state);
    }

    for (auto &lr_state: C.states) { 
        if (lr_state.ref_element.type != Element::NONTERM) continue;
        // lr_state = goto(ref1, A) = goto(ref2, A)
        for (auto ref_idx: lr_state.refs) {
            auto &ref = SLR.states[ref_idx];
            ref.add_goto(lr_state.ref_element, lr_state.index);
        }
    }

    return SLR;
}

void SLR_Table::print(std::ostream &os) const {
    for (auto &state: states) {
        state.print(os);
        os << std::endl;
    }
}