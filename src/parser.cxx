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
    print("Error in parsing input text!\n");
    std::exit(1);
    return nullptr;
}

template<typename TOKENTYPE>
void Parser::eat()
{
    if (IsTokenType<TOKENTYPE>(curToken)) {
        this->curToken = lexer.get_next_token();
    } else {
        Error();
    }
}

AST* Parser::term()
{
    AST* res = factor();
    while (IsTokenType<MUL>(curToken) || IsTokenType<DIV>(curToken)) {
        auto t = curToken;
        if (IsTokenType<MUL>(t)) {
            eat<MUL>();
        } else if (IsTokenType<DIV>(t)) {
            eat<DIV>();
        }
        res = new BiOp(res, term(), t);
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
    AST* node = compound_statement();
    eat<DOT>();
    return node;
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
    if (IsTokenType<ID>(curToken)) {
        Error();
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
      | INTEGER
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
    } else if (IsTokenType<INTEGER>(token)) {
        eat<INTEGER>();
        return new Num(dynamic_cast<INTEGER*>(token));
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