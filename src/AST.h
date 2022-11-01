#ifndef _AST_H_
#define _AST_H_
#include "token.h"
#include <limits>
#include <functional>
#include <string>
#include <vector>
#include <map>

struct AST;
struct ASTVisitor;
struct BiOp;
struct UnaryOp;
struct Num;
struct Compound;
struct Assign;
struct Var;
struct NoOp;

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

#define CONSTRUCT_AST_VISTFUNC(TYPE) \
    virtual int Visit##TYPE(TYPE* node) = 0

struct ASTVisitor
{
    ASTVisitor() = default;
    CONSTRUCT_AST_VISTFUNC(BiOp);
    CONSTRUCT_AST_VISTFUNC(Num);
    CONSTRUCT_AST_VISTFUNC(UnaryOp);
    CONSTRUCT_AST_VISTFUNC(Compound);
    CONSTRUCT_AST_VISTFUNC(Assign);
    CONSTRUCT_AST_VISTFUNC(Var);
    CONSTRUCT_AST_VISTFUNC(NoOp);
};

struct BiOp : public AST
{
    AST* left;
    AST* right;
    AbsToken* op;
    BiOp(AST* l, AST* r, AbsToken* op) : left(l), right(r), op(op) {}
    int accept(ASTVisitor* visitor) override {
        return visitor->VisitBiOp(this);
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
        return visitor->VisitNum(this);
    }
};

struct UnaryOp: public AST
{
    AbsToken* op;
    AST* expr;

    UnaryOp(AbsToken* op, AST* expr) : op(op), expr(expr) {}
    int accept(ASTVisitor* visitor) override {
        return visitor->VisitUnaryOp(this);
    }
};

struct Compound: public AST
{
    std::vector<AST*> children;
    Compound();
    Compound(const std::vector<AST*> nodes) : children(nodes) {}
    virtual int accept(ASTVisitor* visitor) override{
        return visitor->VisitCompound(this);
    }
};

struct Assign: public AST
{
    AST* left;      // should be Var
    AST* right;     // should be Num
    AbsToken* op;   // should be ASSIGN
    Assign(AST* left, AST* right, AbsToken* op) : left(left), right(right), op(op) {}
    virtual int accept(ASTVisitor* visitor) override{
        return visitor->VisitAssign(this);
    }
};

struct Var: public AST
{
    AbsToken* token;
    string value;
    Var(AbsToken* t,string val) : token(t), value(val) {}
    Var(AbsToken* t) : token(t) {
        value = dynamic_cast<ID*>(t)->id;
    }
    virtual int accept(ASTVisitor* visitor) override{
        return visitor->VisitVar(this);
    }
};

struct NoOp: public AST
{
    NoOp() = default;
    virtual int accept(ASTVisitor* visitor) override{
        return visitor->VisitNoOp(this);
    }
};

#define CONSTRUCT_AST_VISTFUNC_OVERRRIDE(TYPE) \
    virtual int Visit##TYPE(TYPE* node) override

struct ASTVisitValue : public ASTVisitor
{
    std::map<string, int> GLOBAL_SCOPE;

    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(BiOp);
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(Num);
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(UnaryOp);
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(Compound);
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(Assign);
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(Var);
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(NoOp);
    std::function<int(AST* b)> vis = [=](AST* b) -> int {
        if (IsSubType<BiOp, AST>(b)) {
            return VisitBiOp(dynamic_cast<BiOp*>(b));
        } else if (IsSubType<Num, AST>(b)) {
            return VisitNum(dynamic_cast<Num*>(b));
        } else if (IsSubType<Compound, AST>(b)) {
            return VisitCompound(dynamic_cast<Compound*>(b));
        } else if (IsSubType<UnaryOp, AST>(b)) {
            return VisitUnaryOp(dynamic_cast<UnaryOp*>(b));
        } else if (IsSubType<Assign, AST>(b)) {
            return VisitAssign(dynamic_cast<Assign*>(b));
        } else if (IsSubType<Var, AST>(b)) {
            return VisitVar(dynamic_cast<Var*>(b));
        } else if (IsSubType<NoOp, AST>(b)) {
            return VisitNoOp(dynamic_cast<NoOp*>(b));
        }
        return 0;
    };
};

struct ASTDraw: public ASTVisitor
{
    int cnt = 1;
    string dot_body = R"(digraph astgraph {
    node [shape=circle, fontsize=12, fontname="Courier", height=.1];
    ranksep=.3;
    edge [arrowsize=.5]
    
)";
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(BiOp);
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(Num);
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(UnaryOp);
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(Compound);
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(Assign);
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(Var);
    CONSTRUCT_AST_VISTFUNC_OVERRRIDE(NoOp);
    std::function<int(AST* b)> vis = [=](AST* b) -> int {
        if (IsSubType<BiOp, AST>(b)) {
            return VisitBiOp(dynamic_cast<BiOp*>(b));
        } else if (IsSubType<Num, AST>(b)) {
            return VisitNum(dynamic_cast<Num*>(b));
        } else if (IsSubType<Compound, AST>(b)) {
            return VisitCompound(dynamic_cast<Compound*>(b));
        } else if (IsSubType<UnaryOp, AST>(b)) {
            return VisitUnaryOp(dynamic_cast<UnaryOp*>(b));
        } else if (IsSubType<Assign, AST>(b)) {
            return VisitAssign(dynamic_cast<Assign*>(b));
        } else if (IsSubType<Var, AST>(b)) {
            return VisitVar(dynamic_cast<Var*>(b));
        } else if (IsSubType<NoOp, AST>(b)) {
            return VisitNoOp(dynamic_cast<NoOp*>(b));
        }
        return 0;
    };
    string GenDot();

    string GetOp(AbsToken *abs) {
        if (IsTokenType<MUL>(abs)) { return "*"; }
        if (IsTokenType<PLUS>(abs)) { return "+"; }
        if (IsTokenType<MINUS>(abs)) { return "-"; }
        if (IsTokenType<DIV>(abs)) { return "/"; }
        if (IsTokenType<ASSIGN>(abs)) { return ":="; }
        return "";
    }
};
#endif