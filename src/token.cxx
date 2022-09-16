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