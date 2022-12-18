#ifndef _PASER_H_
#define _PASER_H_

#include "AST.h"
#include "lexer.h"

struct Parser
{
    Lexer lexer;
    AbsToken* curToken;
    AST* Error();
    string::iterator cur_char;
    AST* factor();
    AST* term();
    AST* program();
    AST* compound_statement();
    std::vector<AST*> statement_list();
    AST* statement();
    AST* assignment_statement();
    AST* variable();
    AST* empty();    
    Parser(const string& str) :lexer(str) {
        curToken = lexer.get_next_token();
    };

    
    template<typename TOKENTYPE> void eat();
    AST* expr();
    AST* parse();
    AST* block();
    std::vector<AST*> declarations();
    std::vector<VarDecl*> variable_declaration();
    Type* type_spec();
};

#endif