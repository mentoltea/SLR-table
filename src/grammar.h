#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <set>
#include <vector>
#include <optional>
#include <string>
#include <fstream>
#include <iostream>
#include <assert.h>

typedef std::string Terminal; 
typedef std::string NonTerminal; 

struct Element {
    std::optional<Terminal> t;
    std::optional<NonTerminal> nt;
    enum Type {
        TERM,
        NONTERM
    } type;

    operator std::string() const;
    bool operator==(const Element& other) const;
    bool operator!=(const Element& other) const;
};

typedef std::vector<Element> Chain; 

struct Rule {
    NonTerminal lhs;
    Chain rhs;
};

class Grammar {
    public:
    // Terminals
    std::set<Terminal> terms;
    // Non-Terminals
    std::set<NonTerminal> nonterm;
    // Rules
    std::vector<Rule> rules;

    static Grammar load(std::istream& fd);

    void describe() const;
};

#endif