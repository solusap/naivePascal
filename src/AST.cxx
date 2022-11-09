#include "AST.h"

#include <fmt/core.h>
#include <fmt/ranges.h>
int ASTVisitValue::VisitNum(Num* num)
{
    return num->v._value;
}

int ASTVisitValue::VisitBiOp(BiOp* biop)
{
    
    int l = vis(biop->left);
    int r = vis(biop->right);
    if (IsTokenType<MUL>(biop->op)) {
        return l * r;
    }
    if (IsTokenType<PLUS>(biop->op)) {
        return l + r;
    }
    if (IsTokenType<MINUS>(biop->op)) {
        return l - r;
    }
    if (IsTokenType<INTEGER_DIV>(biop->op)) {
        return l / r;
    }
    fmt::print("unknown operator l = {}, r = {}\n!", l, r);
    return std::numeric_limits<int>::max();
}

int ASTVisitValue::VisitUnaryOp(UnaryOp *unaryop)
{
    if (IsSubType<MINUS, AbsToken>(unaryop->op)) {
        return -vis(unaryop->expr);
    } else {
        return vis(unaryop->expr);
    }
}

int ASTVisitValue::VisitNoOp(NoOp *node)
{
    return 0;
}

int ASTVisitValue::VisitProgram(Program *node)
{
    return 0;
}

int ASTVisitValue::VisitBlock(Block *node)
{
    for (auto&& decl : node->vardecl) {
        vis(decl);
    }
    vis(node->compound_statements);
    return 0;
}


int ASTVisitValue::VisitVarDecl(VarDecl *node)
{
    return 0;
}

int ASTVisitValue::VisitType(Type *node)
{
    return 0;
}

int ASTVisitValue::VisitCompound(Compound *node)
{
    for (auto&& p : node->children) {
        this->vis(p);
    }
    return 0;
}

int ASTVisitValue::VisitAssign(Assign *node)
{
    string var_name = dynamic_cast<Var*>(node->left)->value;
    // GLOBAL_SCOPE.insert({var_name, vis(node->right)});
    return 0;
}

int ASTVisitValue::VisitVar(Var *node)
{    
    if (GLOBAL_SCOPE.count(node->value) == 0) {
        // fmt::print("var {} undefined!\n", node->value);
        exit(1);
    }
    
    // return GLOBAL_SCOPE.at(node->value);;
    return 0;
}