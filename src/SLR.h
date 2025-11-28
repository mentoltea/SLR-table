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
};

struct SLR_State {
    int index;
};

class SLR_Table {
};