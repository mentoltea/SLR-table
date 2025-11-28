#include "grammar.h"
#include "lexer.h"
#include "LR.h"
#include "SLR.h"

#include <filesystem>


namespace fs = std::filesystem;

int main(int argc, char** argv) {
    std::istream *stream;
    std::ifstream fd;
    if (argc == 1) stream = &std::cin;
    else {
        fd = std::ifstream(argv[1]);
        stream = &fd;
    }

    std::string dir = "generated";
    fs::create_directory(dir);

    Grammar G = Grammar::load(*stream);
    if (fd.is_open()) fd.close();
    
    
    std::ofstream g_fd(dir + "/grammar.txt");
    std::cout << "Grammar" <<std::endl;
    std::cout << "--------" <<std::endl;
    G.describe(g_fd);
    G.describe(std::cout);
    std::cout << "--------" <<std::endl;
    g_fd.close();
    
    Grammar::FollowTable ft = G.followtable();

    std::ofstream follow_fd(dir + "/follow.txt");
    std::cout << "Follow Table" <<std::endl;
    std::cout << "--------" <<std::endl;
    for (auto &pair: ft) {
        auto &nt = pair.first;
        auto &flw = pair.second;
        follow_fd << "FOLLOW( " << nt << " ) = { ";
        std::cout << "FOLLOW( " << nt << " ) = { ";
        for (auto &t: flw) {
            follow_fd << t << ", ";
            std::cout << t << ", ";
        }
        follow_fd << "}" <<std::endl;
        std::cout << "}" <<std::endl;
    } 
    std::cout << "--------" <<std::endl;
    follow_fd.close();

    std::ofstream LR_fd(dir + "/LR_Collection.txt");
    LR_Collection C = LR_Collection::from_grammar(G);
    std::cout << "LR collection" <<std::endl;
    std::cout << "--------" <<std::endl;
    for (auto &state: C.states) {
        state.print(C, LR_fd);
        state.print(C, std::cout);
    }
    std::cout << "--------" <<std::endl;
    LR_fd.close();
    
    std::ofstream SLR_fd(dir + "/SLR_Table.txt");
    SLR_Table SLR = SLR_Table::from_LR_collection(G, C);
    std::cout << "SLR table" <<std::endl;
    std::cout << "--------" <<std::endl;
    SLR.print(SLR_fd);
    SLR.print(std::cout);
    std::cout << "--------" <<std::endl;
    SLR_fd.close();

    return 0;
}