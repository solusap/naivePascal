#include "AST.h"
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
            fmt::print("{} = {}\n", p.first, p.second._dvalue);
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
    ASTVisitValue visitor_value;

    intep.interpreter();
}

void test_draw()
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
    Parser parser{text};
    auto tree = parser.parse();
    ASTDraw visitor;
    visitor.vis(tree);
    visitor.dot_body += "}";
    fmt::print("{}\n", visitor.dot_body);
}

void test_draw2()
{
    string text = R"(PROGRAM Part10AST;
VAR
   a, b : INTEGER;
   y    : REAL;

BEGIN {Part10AST}
   a := 2;
   b := 10 * a + 10 * a DIV 4;
   y := 20 / 7 + 3.14;
END.  {Part10AST})";
    Parser parser{text};
    auto tree = parser.parse();
    ASTDraw visitor;
    visitor.vis(tree);
    visitor.dot_body += "}";
    fmt::print("{}\n", visitor.dot_body);

}

void test_SymTab()
{
    string p1 = R"(PROGRAM Part11;
VAR
   x : INTEGER;
   y : REAL;

BEGIN

END.)";
string textError1 = R"(PROGRAM NameError1;
VAR
   a : INTEGER;

BEGIN
   a := 2 + b;
END.)";

string text2 = R"(PROGRAM Part11;
VAR
   number : INTEGER;
   a, b   : INTEGER;
   y      : REAL;

BEGIN {Part11}
   number := 2;
   a := number ;
   b := 10 * a + 10 * number DIV 4;
   y := 20 / 7 + 3.14
END.  {Part11})";
Parser parser{text2};
    auto tree = parser.parse();
    SymbolTableBuilder visitor;
    visitor.vis(tree);
    for (auto&& p : visitor.symtab._symbols) {
        auto typePtr = dynamic_cast<VarSymbol*>(p.second);
        if (typePtr != nullptr) {
            auto type = typePtr->type_symbol->type;
            fmt::print("var is {}, type is {}\n", p.first, type);
        }
    }
}

void test_draw3()
{
    string text = R"(PROGRAM Part12;
VAR
   a : INTEGER;

PROCEDURE P1;
VAR
   a : REAL;
   k : INTEGER;

   PROCEDURE P2;
   VAR
      a, z : INTEGER;
   BEGIN {P2}
      z := 777;
   END;  {P2}

BEGIN {P1}

END;  {P1}

BEGIN {Part12}
   a := 10;
END.  {Part12})";
    Parser parser{text};
    auto tree = parser.parse();
    ASTDraw visitor;
    visitor.vis(tree);
    visitor.dot_body += "}";
    fmt::print("{}\n", visitor.dot_body);

}

int main()
{
    // test1();
    // test_draw2();
    // test_SymTab();
    test_draw3();
}