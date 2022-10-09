#include "token.h"
#include <fmt/core.h>
#include <fmt/ranges.h>

using fmt::print;

void TokenDebugVisitor::VisitInt(INTEGER *node)
{
    print("TOKEN(INT, {})\n", node->_val);
}

void TokenDebugVisitor::VisitPlus(PLUS *node)
{
    print("TOKEN(PLUS, +)\n");
}

void TokenDebugVisitor::VisitEof(EndOfFile *node)
{
    print("TOKEN(EOF, NULL)\n");
}

void TokenDebugVisitor::VisitMinus(MINUS *node)
{
    print("TOKEN(MINUS, -)\n");
}

void TokenDebugVisitor::VisitMul(MUL* node)
{
    print("TOKEN(MUL, *)\n");
}

void TokenDebugVisitor::VisitDiv(DIV* node)
{
    print("TOKEN(DIV, /)\n");
}

void TokenDebugVisitor::VisitLParen(LPAREN* node)
{
    print("TOKEN(DIV, '(')\n");
}

void TokenDebugVisitor::VisitRParen(RPAREN* node)
{
    print("TOKEN(DIV, ')')\n");
}