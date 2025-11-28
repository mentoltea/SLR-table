#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <exception>
#include <vector>
#include <exception>

struct Token {
    std::string str;
    int line;
    int pos;
};

std::vector<Token> parse( std::istream& fd );

#endif