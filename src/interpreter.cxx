#include "token.h"
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <string>
#include <iostream>
#include <cctype>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "AST.h"

using fmt::print;
using std::string;

class Lexer
{
    string text;
    size_t pos;
public:
    Lexer(const string& str) : text(str), pos(0) {    };
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

        if (text[pos] == '(') {
            advance();
            return new LPAREN();
        }

        if (text[pos] == ')') {
            advance();
            return new RPAREN();
        }
        return Error();
    }
    return new EndOfFile();
}

class Parser
{
    Lexer lexer;
    AbsToken* curToken;
    AST* Error();
    string::iterator cur_char;
    AST* factor();
    AST* term();
    
public:
    Parser(const string& str) :lexer(str) {
        curToken = lexer.get_next_token();
    };
    
    template<typename TOKENTYPE> void eat();
    AST* expr();
};

AST* Parser::Error()
{
    print("Error in parsing input text!\n");
    std::exit(1);
    return nullptr;
}

template<typename TOKENTYPE>
void Parser::eat()
{
    if (IsTokenType<TOKENTYPE>(curToken)) {
        this->curToken = lexer.get_next_token();
    } else {
        Error();
    }
}

AST* Parser::factor()
{
    auto token = this->curToken;
    if (IsTokenType<INTEGER>(token)) {
        eat<INTEGER>();
        return new Num(dynamic_cast<INTEGER*>(token));
    } else if (IsTokenType<LPAREN>(token)) {
        eat<LPAREN>();
        auto res = expr();
        eat<RPAREN>();
        return res;
    }
    return nullptr;
}

AST* Parser::term()
{
    AST* res = factor();
    while (IsTokenType<MUL>(curToken) || IsTokenType<DIV>(curToken)) {
        auto t = curToken;
        if (IsTokenType<MUL>(t)) {
            eat<MUL>();
        } else if (IsTokenType<DIV>(t)) {
            eat<DIV>();
        }
        res = new BiOp(res, term(), t);
    }
    return res;
}

AST* Parser::expr()
{
    AST* res = term();
    while (IsTokenType<PLUS>(curToken) || IsTokenType<MINUS>(curToken)) {
        auto t = curToken;
        if (IsTokenType<PLUS>(curToken)) {
            eat<PLUS>();
        } else if (IsTokenType<MINUS>(curToken)) {
            eat<MINUS>();
        }
        res = new BiOp(res, term(), t);
    }
    return res;
}

struct Interpreter
{
    string text;
    Interpreter(const string& t) : text(t) {}
    int interpreter()
    {
        Parser parser(text);
        ASTVisitValue visitor;
        auto ptr = parser.expr();
        return ptr->accept(&visitor);
    }   
};

int main()
{
    for(;;) {
        fmt::print("cal>");
        string text;
        getline(std::cin, text);
        fmt::print("input is {}\n", text);
        Interpreter intep{text};
        TokenDebugVisitor visit;
        ASTVisitValue visitor_value;

        auto result = intep.interpreter();
        fmt::print("{}\n", result);
    }
}