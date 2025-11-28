#ifndef LL_H
#define LL_H

#include "grammar.h"

struct LR_Rule: public Rule {
    int dot_place;
    bool was_analyzed;
    LR_Rule(const Rule& rule);
    LR_Rule& operator=(const Rule& rule);
    bool operator==(const LR_Rule& other) const;

    void print(std::ostream &os) const;
};

class LR_Collection;

struct LR_State {
    int index;
    Element ref_element;
    // Rule_N = goto( Rule_I, Element ) = goto( Rule_J, Element ) = ...
    std::vector< int > refs;

    std::vector< LR_Rule > rules;

    LR_State(const std::vector< LR_Rule >& rls);

    void print(const LR_Collection& C, std::ostream &os) const;

    bool operator==(const LR_State& other) const;
};

class LR_Collection {
    public:
    std::vector<LR_State> states;

    LR_State* has_same_state(const LR_State& ref) const;
    LR_State* has_same_ref(const LR_State& ref) const;
    
    static LR_Collection from_grammar(const Grammar& G);
};

// RECURSIVE
void closure(const Grammar& G, const LR_Rule& rule, std::vector<LR_Rule> &result);

#endif