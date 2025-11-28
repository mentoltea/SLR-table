#include "grammar.h"
#include "LR.h"

#include <map>

typedef int Shift;
typedef int Reduce;

struct Action {
    enum Type {
        ACC,
        SHIFT,
        REDUCE,
    } type;
    union Data {
        Shift s;
        Reduce r;
    } data;

    void print(std::ostream &os) const;
};

struct SLR_State {
    int index;
    std::map<Terminal, Action> Actions;
    std::map<NonTerminal, Shift> Gotos;

    void add_action(const Terminal& t, const Action& a);
    void add_goto(const NonTerminal& nt, Shift idx);

    void print(std::ostream &os) const;
};

class SLR_Table {
    public:
    std::vector<SLR_State> states;

    void print(std::ostream &os) const;
    static SLR_Table from_LR_collection(const Grammar& G, const LR_Collection& C);
};