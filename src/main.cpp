#include "grammar.h"
#include "lexer.h"
#include "LR.h"

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
    
    Grammar::FollowTable ft = G.followtable();

    std::cout << "Follow Table" <<std::endl;
    std::cout << "--------" <<std::endl;
    for (auto &pair: ft) {
        auto &nt = pair.first;
        auto &flw = pair.second;
        std::cout << "FOLLOW( " << nt << " ) = { ";
        for (auto &t: flw) {
            std::cout << t << ", ";
        }
        std::cout << "}" <<std::endl;
    } 
    std::cout << "--------" <<std::endl;

    
    LR_Collection C = LR_Collection::from_grammar(G);
    std::cout << "LR collection" <<std::endl;
    std::cout << "--------" <<std::endl;
    for (auto &state: C.states) {
        state.print(C);
    }
    std::cout << "--------" <<std::endl;
    
    
    return 0;
}