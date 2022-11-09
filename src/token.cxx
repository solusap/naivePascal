// #include "token.h"
// #include <fmt/core.h>
// #include <fmt/ranges.h>

// using fmt::print;

// void TokenDebugVisitor::VisitInt(INTEGER_CONSTANT *node)
// {
//     print("TOKEN(INT, {})\n", node->_val);
// }

// #define REALIZE_DBG_VISITOR(TOKEN_TYPE, TOKEN_PRINT) \
// void TokenDebugVisitor::Visit##TOKEN_TYPE(TOKEN_TYPE* node)\
// {\
//     print("TOKEN("#TOKEN_TYPE", "#TOKEN_PRINT")\n"); \
// }

// REALIZE_DBG_VISITOR(PLUS, "+")
// REALIZE_DBG_VISITOR(MINUS, "-")
// REALIZE_DBG_VISITOR(MUL, "*")
// REALIZE_DBG_VISITOR(INTEGER_DIV, "/")
// REALIZE_DBG_VISITOR(LPAREN, "(")
// REALIZE_DBG_VISITOR(RPAREN, ")")
// REALIZE_DBG_VISITOR(BEGIN, "BEGIN")
// REALIZE_DBG_VISITOR(END, "END")
// REALIZE_DBG_VISITOR(SEMI, "SEMI")
// REALIZE_DBG_VISITOR(DOT, "DOT")
// REALIZE_DBG_VISITOR(ASSIGN, "ASSIGN")
// REALIZE_DBG_VISITOR(EndOfFile, "EOF");

// void TokenDebugVisitor::VisitID(ID* node)
// {
//     print("TOKEN(ID, {})\n", node->id);
// }