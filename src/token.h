#ifndef _TOKEN_H_
#define _TOKEN_H_
#include <string>

using std::string;

struct INTEGER_CONSTANT;
struct PLUS;
struct MINUS;
struct MUL;
struct INTEGER_DIV;
struct LPAREN;
struct RPAREN;
struct EndOfFile;
struct BEGIN;
struct END;
struct ID;
struct SEMI;
struct DOT;
struct ASSIGN;
struct PROGRAM;
struct VAR;
struct COLON; // :
struct COMMA; // ;
struct INTEGER;
struct REAL;
struct REAL_CONST;
struct FLOAT_DIV;
struct PROCEDURE;

struct AbsToken 
{
    virtual ~AbsToken() = default;
};

#define CONSTRUCT_TOKEN_TYPE(TOKEN_TYPE) \
struct TOKEN_TYPE : public AbsToken {\
    TOKEN_TYPE() = default; \
    ~TOKEN_TYPE() = default; \
}; \

CONSTRUCT_TOKEN_TYPE(PLUS);
CONSTRUCT_TOKEN_TYPE(MINUS);
CONSTRUCT_TOKEN_TYPE(MUL);
CONSTRUCT_TOKEN_TYPE(INTEGER_DIV);
CONSTRUCT_TOKEN_TYPE(LPAREN);
CONSTRUCT_TOKEN_TYPE(RPAREN);
CONSTRUCT_TOKEN_TYPE(EndOfFile);
CONSTRUCT_TOKEN_TYPE(BEGIN);
CONSTRUCT_TOKEN_TYPE(END);
CONSTRUCT_TOKEN_TYPE(SEMI);
CONSTRUCT_TOKEN_TYPE(DOT);
CONSTRUCT_TOKEN_TYPE(ASSIGN);
CONSTRUCT_TOKEN_TYPE(PROGRAM);
CONSTRUCT_TOKEN_TYPE(PROCEDURE);
CONSTRUCT_TOKEN_TYPE(VAR);
CONSTRUCT_TOKEN_TYPE(COLON);
CONSTRUCT_TOKEN_TYPE(COMMA);
CONSTRUCT_TOKEN_TYPE(INTEGER);
CONSTRUCT_TOKEN_TYPE(REAL);
CONSTRUCT_TOKEN_TYPE(FLOAT_DIV);

struct INTEGER_CONSTANT : public AbsToken
{
    int _val;
    INTEGER_CONSTANT(int x) : _val(x) {};
    INTEGER_CONSTANT() : _val(0) {};
       ~INTEGER_CONSTANT() = default;
};

struct REAL_CONST : public AbsToken
{
    double _val;
    REAL_CONST(double x) : _val(x) {};
    REAL_CONST() : _val(0.0) {};
       ~REAL_CONST() = default;
};

struct ID : public AbsToken
{
    string id;
    ID(const string& inid) : id(inid) {};
       ~ID() = default;
};

template<typename TOKENTYPE>
bool IsTokenType(AbsToken* token)
{
    return dynamic_cast<TOKENTYPE* >(token) != nullptr;
}

#endif