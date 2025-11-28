#include "lexer.h"
#include "grammar.h"


Grammar Grammar::load(std::istream& fd) {
    Grammar G;

    // G.terms.insert("$");
    // G.nonterm.insert("S");
    
    auto tokens = parse(fd);

    int terminals_line = -1;
    int nonterminals_line = -1;
    int ignore_line = -1;
    
    Rule rule;
    int rule_line = -1;
    for (auto &t: tokens) {
        // std::cout << t.line << " : " << t.str << std::endl;
        // continue;
        if (t.str[0] == '#') {
            if (t.str == "#TERMS") {
                terminals_line = t.line+1;
            } else if (t.str == "#NONTERMS") {
                nonterminals_line = t.line+1;
            } else {
                ignore_line = t.line;
            }
            continue;
        }
        if (t.line == ignore_line) continue;

        if (t.str == "$") {
            std::cerr << "Token `$` is reserved and cannot be used" << std::endl;
            std::cerr << t.line << ':' << t.pos << ": " << t.str << std::endl;
            throw std::runtime_error("Reserved token");
        }

        if (t.line == terminals_line) {
            if (t.str == "S") {
                std::cerr << "Token `S` is reserved and cannot be used" << std::endl;
                std::cerr << t.line << ':' << t.pos << ": " << t.str << std::endl;
                throw std::runtime_error("Reserved token");
            }
            
            G.terms.insert(t.str);
        }

        else if (t.line == nonterminals_line) {
            if (t.str == "S") {
                std::cerr << "Token `S` is reserved and cannot be used" << std::endl;
                std::cerr << t.line << ':' << t.pos << ": " << t.str << std::endl;
                throw std::runtime_error("Reserved token");
            }
            G.nonterm.insert(t.str);
        }
        else {
            bool in_term = G.terms.find(t.str)!=G.terms.end();
            bool in_nonterm = G.nonterm.find(t.str)!=G.nonterm.end();

            if (t.str == "S") {
                in_nonterm = true;
            }

            if (t.line == rule_line) {
                if (in_term && in_nonterm) assert(0 && "Unreachable");

                Element el = {};
                if (in_term) {
                    el.t = t.str;
                    el.type = el.TERM;
                } else if (in_nonterm) {
                    el.nt = t.str;
                    el.type = el.NONTERM;
                } else {
                    std::cerr << "Unexpected token: not termianal nor non-terminal" << std::endl;
                    std::cerr << t.line << ':' << t.pos << ": " << t.str << std::endl;
                    throw std::runtime_error("Unexpected token");
                }

                rule.rhs.push_back(el);
            } else {

                if (rule_line != -1) {
                    G.rules.push_back(rule);
                }
                rule_line = t.line;
                rule = Rule();
                if (in_term) {
                    std::cerr << "Terminal cannot be in lefthand side of a rule" << std::endl;
                    std::cerr << t.line << ':' << t.pos << ": " << t.str << std::endl;
                    throw std::runtime_error("Terminal in lhs of rule");
                }
                if (!in_nonterm) {
                    std::cerr << "Unexpected token: was expected non-terminal in the lefthand side of the rule" << std::endl;
                    std::cerr << t.line << ':' << t.pos << ": " << t.str << std::endl;
                    throw std::runtime_error("Unexpected token in lhs of rule");
                }
                rule.lhs = t.str;
            }
        }
    }
    if (rule_line != -1) G.rules.push_back(rule);

    return G;
}