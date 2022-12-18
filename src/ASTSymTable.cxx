#include "AST.h"
#include <fmt/core.h>

int SymbolTableBuilder::VisitBlock(Block *node)
{
    for (auto decl : node->vardecl) {
        vis(decl);
    }
    vis(node->compound_statements);
    return 0;
}

int SymbolTableBuilder::VisitProgram(Program *node)
{
    vis(node->block);
    return 0;
}

int SymbolTableBuilder::VisitBiOp(BiOp *node)
{
    vis(node->left);
    vis(node->right);
    return 0;
}

int SymbolTableBuilder::VisitNum(Num *node)
{
    return 0;
}

int SymbolTableBuilder::VisitNoOp(NoOp *node)
{
    return 0;
}

int SymbolTableBuilder::VisitType(Type *node)
{
    return 0;
}

int SymbolTableBuilder::VisitUnaryOp(UnaryOp *node)
{
    vis(node->expr);
    return 0;
}

int SymbolTableBuilder::VisitProcedureDecl(ProcedureDecl *node)
{
    return 0;
}

int SymbolTableBuilder::VisitCompound(Compound *node)
{
    for (auto child : node->children) {
        vis(child);
    }
    return 0;
}

int SymbolTableBuilder::VisitVarDecl(VarDecl *node)
{
    string typeName = node->type->value;
    Symbol* type_symbol = symtab.lookup(typeName);
    string var_name = node->var->value;
    Symbol * var_symbol = new VarSymbol(var_name, type_symbol);
    symtab.define(var_symbol);
    return 0;
}

int SymbolTableBuilder::VisitAssign(Assign *node)
{
    string var_name = dynamic_cast<Var*>(node->left)->value;
    Symbol* var_symbol = symtab.lookup(var_name);
    if (var_symbol == nullptr) {
        string ret = "variable {" + var_name + "} undefined before!";
        fmt::print(ret);
        throw ret;
    }
    vis(node->right);
    return 0;
}

int SymbolTableBuilder::VisitVar(Var *node)
{
    string var_name = node->value;
    Symbol* var_symbol = symtab.lookup(var_name);
    string ret = "variable {" + var_name + "}undefined before!";
    if (var_symbol == nullptr) {
        string ret = "variable {" + var_name + "} undefined before!";
        fmt::print("variable ' {} ' undefined before!\n", var_name);
        throw ret;
    }
    return 0;
}
