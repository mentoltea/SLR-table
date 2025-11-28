#include "lexer.h"

std::vector<Token> parse( std::istream& fd ) {
    char c;
    int line = 1;
    int pos = 0;

#define BUFFER_SIZE 4096
    static char buffer[BUFFER_SIZE];
    size_t buffer_pos = 0;

    std::vector<Token> tokens;
    while (!fd.eof()) {
        c = fd.get();
        if (c == ' ' || c == '\n' || c == EOF) {
            if (buffer_pos != 0) {
                buffer[buffer_pos] = '\0';
                
                Token t;
                t.line = line;
                t.pos = pos;
                t.str = std::string(buffer);
                // std::cout << line << ":" << pos << ":" << t.str << std::endl;
                tokens.push_back(t);
                
                buffer_pos = 0;
            }
            if (c == '\n') {
                pos = 1;
                line++;
            }
        } else {
            if (c <= 32) {
                continue;
            } else {
                buffer[buffer_pos++] = c;
                if (buffer_pos >= BUFFER_SIZE) {
                    throw std::runtime_error("BUFFER OVERFLOW");
                }
            }
            pos++;
        }
    }

    return tokens;
}