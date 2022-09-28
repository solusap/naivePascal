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

class Lexer
{
    string text;
    size_t pos;
public:
    Lexer(const string& str) : text(str), pos(0) {
        fmt::print("lexer {}\n", text);
    };
    AbsToken* Error();
    void advance();
    void skip_whitespace();
    int build_integer();
    AbsToken* get_next_token();
};

AbsToken* Lexer::Error()
{
    print("Error in parsing input text!\n");
    std::exit(1);
    return nullptr;
}

void Lexer::advance()
{
    pos += 1;
    if (pos >= text.length()) {
        pos = std::string::npos;
    }
}

void Lexer::skip_whitespace()
{
    while (pos != std::string::npos && text[pos] == ' ') {
        advance();
    }
}

int Lexer::build_integer()
{
    int start = pos;
    while (pos != std::string::npos && std::isdigit(text[pos])) {
        advance();
    }
    return std::stoi(text.substr(start, pos - start));
}

AbsToken* Lexer::get_next_token()
{
    while (pos != std::string::npos) {
        if (text[pos] == ' ') {
            skip_whitespace();
            continue;
        }
        if (std::isdigit(text[pos])) {
            return new INTEGER(build_integer());
        }

        if (text[pos] == '+') {
            advance();
            return new PLUS();
        }
        if (text[pos] == '-') {
            advance();
            return new MINUS();
        }

        if (text[pos] == '*') {
            advance();
            return new MUL();
        }
        if (text[pos] == '/') {
            advance();
            return new DIV();
        }
        return Error();
    }
    return new EndOfFile();
}

class Interpreter
{
    Lexer lexer;
    AbsToken* curToken;
    AbsToken* Error();
    string::iterator cur_char;
    int factor();
    
public:
    Interpreter(const string& str) :lexer(str) {
        curToken = lexer.get_next_token();
    };
    
    template<typename TOKENTYPE> void eat();
    AbsToken* expr();
};

AbsToken* Interpreter::Error()
{
    print("Error in parsing input text!\n");
    std::exit(1);
    return nullptr;
}

template<typename TOKENTYPE>
void Interpreter::eat()
{
    if (IsTokenType<TOKENTYPE>(curToken)) {
        this->curToken = lexer.get_next_token();
    } else {
        Error();
    }
}

int Interpreter::factor()
{
    auto token = this->curToken;
    eat<INTEGER>();
    INTEGER* itoken = dynamic_cast<INTEGER*>(token);
    return itoken->_val;
}

AbsToken* Interpreter::expr()
{
    int res = factor();
    while (IsTokenType<MUL>(curToken) || IsTokenType<DIV>(curToken)) {
        auto t = curToken;
        if (IsTokenType<MUL>(t)) {
            eat<MUL>();
            res *= factor();
        } else if (IsTokenType<DIV>(t)) {
            eat<DIV>();
            res /= factor();
        }
    }
    return new INTEGER(res);
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