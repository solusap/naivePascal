#include "parser.h"
#include "AST.h"
#include "token.h"

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <cctype>
#include <vector>


using fmt::print;
using std::string;


AST* Parser::Error()
{
    lexer.Error();
    return nullptr;
}

template<typename TOKENTYPE>
void Parser::eat()
{
    if (IsTokenType<TOKENTYPE>(curToken)) {
        this->curToken = lexer.get_next_token();
    } else {
        print("cur token is not match input type!\n");
        Error();
    }
}

AST* Parser::term()
{
    AST* res = factor();
    while (IsTokenType<MUL>(curToken) || IsTokenType<INTEGER_DIV>(curToken) || IsTokenType<FLOAT_DIV>(curToken)) {
        auto t = curToken;
        if (IsTokenType<MUL>(t)) {
            eat<MUL>();
        } else if (IsTokenType<INTEGER_DIV>(t)) {
            eat<INTEGER_DIV>();
        } else if (IsTokenType<FLOAT_DIV>(t)) {
            eat<FLOAT_DIV>();
        }
        res = new BiOp(res, factor(), t);
    }
    return res;
}

AST* Parser::expr()
{
    AST* res = term();
    while (IsTokenType<PLUS>(curToken) || IsTokenType<MINUS>(curToken)) {
        auto t = curToken;
        if (IsTokenType<PLUS>(curToken)) {
            eat<PLUS>();
        } else if (IsTokenType<MINUS>(curToken)) {
            eat<MINUS>();
        }
        res = new BiOp(res, term(), t);
    }
    return res;
}

// program: compound_statement DOT
AST* Parser::program()
{
    eat<PROGRAM>();
    Var* var_node =  dynamic_cast<Var*>(this->variable());
    string prog_name = var_node->value;
    eat<SEMI>();
    Block* blocK_node = dynamic_cast<Block*>(block());
    Program * prog_node = new Program(prog_name, blocK_node);
    eat<DOT>();
    return prog_node;
}

// compound_statement: BEGIN statement_list END
AST* Parser::compound_statement()
{
    eat<BEGIN>();
    std::vector<AST*> nodes = statement_list();
    eat<END>();
    return new Compound(nodes);
}

/* 
statement_list: statement
              | statement SEMI statement_lists
*/
std::vector<AST*> Parser::statement_list()
{
    AST* node = statement();

    std::vector<AST*> results;
    results.push_back(node);

    while (IsTokenType<SEMI>(curToken)) {
        eat<SEMI>();
        results.push_back(statement());
    }

    return results;
}

/* 
statement: compound_statement
         | assignment_statement
         | empty
*/
AST* Parser::statement()
{
    if (IsTokenType<BEGIN>(curToken)) {
        return compound_statement();
    } else if (IsTokenType<ID>(curToken)) {
        return assignment_statement();
    } else {
        return empty();
    }
}

// assignment_statement : variable ASSIGN expr
AST* Parser::assignment_statement()
{
    AST* left = variable();
    auto token = curToken;
    eat<ASSIGN>();
    AST* right = expr();
    return new Assign(left, right, token);
}

// variable: ID
AST* Parser::variable()
{
    AST* node = new Var(curToken);
    eat<ID>();
    return node;
}

AST* Parser::empty()
{
    return new NoOp(); 
}

/*
factor: PLUS  factor
      | MINUS factor
      | INTEGER_CONSTANT
      | LPAREN expr RPAREN
      | variable

*/
AST* Parser::factor()
{
    auto token = this->curToken;
    if (IsTokenType<PLUS>(token)) {
        eat<PLUS>();
        return new UnaryOp(token, factor());
    } else if (IsTokenType<MINUS>(token)) {
        eat<MINUS>();
        return new UnaryOp(token, factor());
    } else if (IsTokenType<INTEGER_CONSTANT>(token)) {
        eat<INTEGER_CONSTANT>();
        return new Num(dynamic_cast<INTEGER_CONSTANT*>(token));
    } else if (IsTokenType<REAL_CONST>(token)) {
        eat<REAL_CONST>();
        return new Num(dynamic_cast<REAL_CONST*>(token));
    } else if (IsTokenType<LPAREN>(token)) {
        eat<LPAREN>();
        auto res = expr();
        eat<RPAREN>();
        return res;
    } else {
        AST* node = variable();
        return node;
    }
}

AST* Parser::parse()
{
    AST* node = program();
    if (!IsTokenType<EndOfFile>(curToken)) {
        Error();
    }
    return node;
}

// block : declarations compounds_statement

AST* Parser::block()
{
    return new Block(declarations(), 
                     dynamic_cast<Compound*>(compound_statement()));
}

std::vector<VarDecl*> Parser::declarations()
{
    std::vector<VarDecl*> decls;
    if (IsTokenType<VAR>(curToken)) {
        eat<VAR>();
        while (IsTokenType<ID>(curToken)) {
            std::vector<VarDecl*> var_decl = variable_declaration();
            for (auto vd : var_decl) {
                decls.push_back(vd);
            }
            eat<SEMI>();
        }
    }
    return decls;
}

// variable_declaration : ID (COMMA ID)* COLON type_spec
std::vector<VarDecl*> Parser::variable_declaration()
{
    std::vector<Var*> var_nodes;
    var_nodes.push_back(new Var(curToken));
    eat<ID>();

    while (IsTokenType<COMMA>(curToken)) {
        eat<COMMA>();
        var_nodes.push_back(new Var(curToken));
        eat<ID>();
    }
    eat<COLON>();

    Type* type_node = type_spec();
    std::vector<VarDecl*> var_decls;
    for (auto var : var_nodes) {
        var_decls.push_back(new VarDecl(var, type_node));
    }
    return var_decls;
}
// type_spec: INTEGER | REAL
Type* Parser::type_spec()
{
    auto token = curToken;
    if (IsTokenType<INTEGER>(curToken)) {
        eat<INTEGER>();
    } else {
        eat<REAL>();
    }
    return new Type(token);
}