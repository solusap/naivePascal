#ifndef _AST_H_
#define _AST_H_
#include "token.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <limits>
#include <functional>

struct AST;
struct ASTVisitor;
struct BiOp;
struct UnaryOp;
struct Num;

template<typename SubType, typename Type>
bool IsSubType(Type* ptr)
{
    return dynamic_cast<SubType* >(ptr) != nullptr;
}

struct AST
{
    AST() = default;
    virtual int accept(ASTVisitor* ast_visitor) = 0;
};

struct ASTVisitor
{
    ASTVisitor() = default;
    virtual int visit_BiOp(BiOp* ast) = 0;
    virtual int visit_Num(Num* ast) = 0;
    virtual int visit_UnaryOp(UnaryOp* ast) = 0;
};

struct BiOp : public AST
{
    AST* left;
    AST* right;
    AbsToken* op;
    BiOp(AST* l, AST* r, AbsToken* op) : left(l), right(r), op(op) {}
    int accept(ASTVisitor* visitor) override {
        return visitor->visit_BiOp(this);
    }
};

struct Num : public AST
{
    int value;
    Num(INTEGER* num)  
    {
        value = num->_val;
    }
    int accept(ASTVisitor* visitor) override {
        return visitor->visit_Num(this);
    }
};

struct UnaryOp: public AST
{
    AbsToken* op;
    AST* expr;

    UnaryOp(AbsToken* op, AST* expr) : op(op), expr(expr) {}
    int accept(ASTVisitor* visitor) override {
        return visitor->visit_UnaryOp(this);
    }
};

struct ASTVisitValue : public ASTVisitor
{
    virtual int visit_BiOp(BiOp* biop) override;
    virtual int visit_Num(Num* num) override;
    virtual int visit_UnaryOp(UnaryOp* unaryop) override;
    std::function<int(AST* b)> vis = [=](AST* b) -> int {
        if (IsSubType<BiOp, AST>(b)) {
            return visit_BiOp(dynamic_cast<BiOp*>(b));
        } else if (IsSubType<Num, AST>(b)) {
            return visit_Num(dynamic_cast<Num*>(b));
        } else {
            return visit_UnaryOp(dynamic_cast<UnaryOp*>(b));
        }
    };
};

int ASTVisitValue::visit_Num(Num* num)
{
    return num->value;
}

int ASTVisitValue::visit_BiOp(BiOp* biop)
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
    if (IsTokenType<DIV>(biop->op)) {
        return l / r;
    }
    fmt::print("unknown operator l = {}, r = {}\n!", l, r);
    return std::numeric_limits<int>::max();
}

int ASTVisitValue::visit_UnaryOp(UnaryOp *unaryop)
{
    if (IsSubType<MINUS, AbsToken>(unaryop->op)) {
        return -vis(unaryop->expr);
    } else {
        return vis(unaryop->expr);
    }
}
#endif