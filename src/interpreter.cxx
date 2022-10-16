#include "token.h"
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <string>
#include <iostream>
#include <cctype>

#include "parser.h"

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