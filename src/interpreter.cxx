#include "token.h"
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <string>
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include "parser.h"

struct Interpreter
{
    string text;
    Parser parser;
    Interpreter(const string& t) : text(t), parser(text) {}
    void interpreter()
    {
        Parser parser(text);
        ASTVisitValue visitor;
        auto ptr = parser.parse();
        visitor.vis(ptr);
        for (auto&& p : visitor.GLOBAL_SCOPE) {
            fmt::print("{} = {}\n", p.first, p.second);
        }
    }   
};

void test1()
{
    string text = R"( BEGIN
     BEGIN
         number := 2;
         a := number;
         b := 10 * a + 10 * number / 4;
         c := a - - b
     END;

     x := 11;
 END.)";
    Interpreter intep{text};
    TokenDebugVisitor visit;
    ASTVisitValue visitor_value;

    intep.interpreter();
}

int main()
{
    test1();
}