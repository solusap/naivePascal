#include "AST.h"
#include <fmt/core.h>

using fmt::print;
using fmt::format;

int ASTDraw::VisitBiOp(BiOp *node) {
    string op = GetOp(node->op);

    int t = cnt++;
    dot_body += fmt::format("    node{} [label=\"{}\"]\n", t, op);
    int l = vis(node->left);
    int r = vis(node->right);
    dot_body += format("    node{} -> node{}\n", t, l);
    dot_body += format("    node{} -> node{}\n", t, r);
    return t;
}

int ASTDraw::VisitNum(Num *node) {
    int t = cnt++;
    dot_body += format("    node{} [label=\"{}\"]\n", t, node->value);
    return t;
}

int ASTDraw::VisitUnaryOp(UnaryOp *node) {
    int t = cnt++;
    dot_body += format("    node{} [label=\"unary {}\"]\n", t, GetOp(node->op));
    int ex = vis(node->expr);
    dot_body += format("    node{} -> node{}\n", t, ex);
    return t++;        
}

int ASTDraw::VisitCompound(Compound *node) {
    int t = cnt++;
    dot_body += format("    node{} [label=\"Compund\"]\n", t);

    for (auto child : node->children) {
        int number = vis(child);
        dot_body += format("    node{} -> node{}\n", t, number);
    }
    return t;
}

int ASTDraw::VisitAssign(Assign *node) {
    int t = cnt++;
    dot_body += format("    node{} [label=\"{}\"]\n", t, GetOp(node->op));

    int x = vis(node->left);
    int y = vis(node->right);
    dot_body += format("    node{} -> node{}\n", t, x);
    dot_body += format("    node{} -> node{}\n", t, y);
    return t;
}

int ASTDraw::VisitVar(Var *node) {
    int t = cnt++;
    dot_body += format("    node{} [label=\"{}\"]\n", t, node->value);
    return t;
}

int ASTDraw::VisitNoOp(NoOp *node) {
    int t = cnt++;
    dot_body += format("    node{} [label=\"NoOp\"]\n", t);
    return t;
}