#include "token.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <cctype>

#include <fmt/core.h>
#include <fmt/ranges.h>

using fmt::print;

bool m_isdigit(char ch)
{
    return std::isdigit(static_cast<char>(ch));
}

using std::string;
class Interpreter
{
    string text;
    AbsToken* curToken;
    size_t pos;
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
    return nullptr;

}

AbsToken* Interpreter::GetNextToken()
{
    // 
    const string& ttext = text;
    if (pos > ttext.length() - 1) {
        return new EndOfFile();
    }
    char curChar = ttext[pos];
    if (m_isdigit(curChar)) {
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
    eat<INTEGER>();
    
    AbsToken* op = curToken;
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