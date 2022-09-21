#include "token.h"
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <string>
#include <iostream>
#include <cctype>

#include <fmt/core.h>
#include <fmt/ranges.h>

using fmt::print;
using std::string;

class Interpreter
{
    string text;
    AbsToken* curToken;
    size_t pos;
    void advance();
    void skip_whitespace();
    string::iterator cur_char;
    int build_integer();
public:
    Interpreter(const string& str) : text(str), curToken(nullptr), pos(0) {};

    // scanner or lexical analyzer
    AbsToken* GetNextToken();
    AbsToken* Error();
    template<typename TOKENTYPE> void eat();
    AbsToken* expr();
};

AbsToken* Interpreter::Error()
{
    print("Error in parsing input text!\n");
    std::exit(1);
    return nullptr;
}

void Interpreter::advance()
{
    pos += 1;
    if (pos >= text.length()) {
        pos = std::string::npos;
    }
}

void Interpreter::skip_whitespace()
{
    while (pos != std::string::npos && text[pos] == ' ') {
        advance();
    }
}

int Interpreter::build_integer()
{
    int start = pos;
    while (pos != std::string::npos && std::isdigit(text[pos])) {
        advance();
    }
    return std::stoi(text.substr(start, pos - start));
}

AbsToken* Interpreter::GetNextToken()
{
    // 
    const string& ttext = text;
    if (pos > ttext.length() - 1) {
        return new EndOfFile();
    }
    char curChar = ttext[pos];
    if (std::isdigit(curChar)) {
        auto token = new INTEGER(static_cast<int>(curChar - '0'));
        pos += 1;
        return token;
    }

    if (curChar == '+') {
        pos += 1;
        return new PLUS();
    }
    return Error();
}


template<typename TOKENTYPE>
void Interpreter::eat()
{
    if (IsTokenType<TOKENTYPE>(curToken)) {
        this->curToken = GetNextToken();
    } else {
        Error();
    }
}

AbsToken* Interpreter::expr()
{
    curToken = GetNextToken();
    INTEGER* left = dynamic_cast<INTEGER*>(curToken);
    if (left == nullptr) {
        return new EndOfFile();
    }
    eat<INTEGER>();
    
    AbsToken* op = curToken;
    if (left == nullptr) {
        return new EndOfFile();
    }
    eat<PLUS>();

    INTEGER* right = dynamic_cast<INTEGER*>(curToken);
    eat<INTEGER>();

    return new INTEGER(left->_val + right->_val);
}

int main()
{
    for(;;) {
        fmt::print("cal>");
        string text;
        getline(std::cin, text);
        Interpreter interp(text);
        TokenDebugVisitor visit;

        auto result = interp.expr();
        result->debug(visit);
        // fmt::print("{}\n", result->debug(visit));

    }
}