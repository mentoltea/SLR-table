#ifndef LL_H
#define LL_H

#include "grammar.h"

struct LL_Rule: public Rule {
    int dot_place;
    bool was_analyzed;
    LL_Rule(const Rule& rule);
    LL_Rule& operator=(const Rule& rule);
    bool operator==(const LL_Rule& other) const;

    void print() const;
};

class LL_Collection;

struct LL_State {
    int index;
    Element ref_element;
    // Rule_N = goto( Rule_I, Element ) = goto( Rule_J, Element ) = ...
    std::vector< int > refs;

    std::vector< LL_Rule > rules;

    LL_State(const std::vector< LL_Rule >& rls);

    void print(const LL_Collection& C) const;

    bool operator==(const LL_State& other) const;
};

class LL_Collection {
    public:
    std::vector<LL_State> states;

    LL_State* has_same_state(const LL_State& ref) const;
    LL_State* has_same_ref(const LL_State& ref) const;
    
    static LL_Collection from_grammar(const Grammar& G);
};

// RECURSIVE
void closure(const Grammar& G, const LL_Rule& rule, std::vector<LL_Rule> &result);

#endif