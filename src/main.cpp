#include "grammar.h"
#include "lexer.h"
#include "LL.h"

int main(int argc, char** argv) {
    std::istream *stream;
    std::ifstream fd;
    if (argc == 1) stream = &std::cin;
    else {
        fd = std::ifstream(argv[1]);
        stream = &fd;
    }

    Grammar G = Grammar::load(*stream);
    if (fd.is_open()) fd.close();

    std::cout << "Grammar" <<std::endl;
    std::cout << "--------" <<std::endl;
    G.describe();
    std::cout << "--------" <<std::endl;
    
    LL_Collection C = LL_Collection::from_grammar(G);
    std::cout << "LL collection" <<std::endl;
    std::cout << "--------" <<std::endl;
    for (auto &state: C.states) {
        state.print(C);
    }
    std::cout << "--------" <<std::endl;
    
    
    return 0;
}