#ifndef _TOKEN_H_
#define _TOKEN_H_
#include <string>

using std::string;

class TokenVisitor;
struct INTEGER;
struct PLUS;
struct MINUS;
struct MUL;
struct DIV;
struct LPAREN;
struct RPAREN;
struct EndOfFile;
struct BEGIN;
struct END;
struct ID;
struct SEMI;
struct DOT;
struct ASSIGN;

struct AbsToken 
{
    virtual void accept(TokenVisitor& v) = 0;     // accept
    virtual ~AbsToken() = default;
};

#define CONSTRUCT_BASE_VISTFUNC(TYPE) \
    virtual void Visit##TYPE(TYPE* node) = 0

class TokenVisitor {
public:
    TokenVisitor() = default;;
    virtual void VisitInt(INTEGER* node) = 0;
    CONSTRUCT_BASE_VISTFUNC(PLUS);
    CONSTRUCT_BASE_VISTFUNC(MINUS);
    CONSTRUCT_BASE_VISTFUNC(MUL);
    CONSTRUCT_BASE_VISTFUNC(DIV);
    CONSTRUCT_BASE_VISTFUNC(EndOfFile);
    CONSTRUCT_BASE_VISTFUNC(LPAREN);
    CONSTRUCT_BASE_VISTFUNC(RPAREN);
    CONSTRUCT_BASE_VISTFUNC(BEGIN);
    CONSTRUCT_BASE_VISTFUNC(END);
    CONSTRUCT_BASE_VISTFUNC(ID);
    CONSTRUCT_BASE_VISTFUNC(SEMI);
    CONSTRUCT_BASE_VISTFUNC(DOT);
    CONSTRUCT_BASE_VISTFUNC(ASSIGN);
    virtual ~TokenVisitor() = default;
};

#define CONSTRUCT_TOKEN_TYPE(TOKEN_TYPE) \
struct TOKEN_TYPE : public AbsToken {\
    TOKEN_TYPE() = default; \
    virtual void accept(TokenVisitor& v) override { \
        v.Visit##TOKEN_TYPE(this); \
    } \
    ~TOKEN_TYPE() = default; \
}

CONSTRUCT_TOKEN_TYPE(PLUS);
CONSTRUCT_TOKEN_TYPE(MINUS);
CONSTRUCT_TOKEN_TYPE(MUL);
CONSTRUCT_TOKEN_TYPE(DIV);
CONSTRUCT_TOKEN_TYPE(LPAREN);
CONSTRUCT_TOKEN_TYPE(RPAREN);
CONSTRUCT_TOKEN_TYPE(EndOfFile);
CONSTRUCT_TOKEN_TYPE(BEGIN);
CONSTRUCT_TOKEN_TYPE(END);
CONSTRUCT_TOKEN_TYPE(ID);
CONSTRUCT_TOKEN_TYPE(SEMI);
CONSTRUCT_TOKEN_TYPE(DOT);
CONSTRUCT_TOKEN_TYPE(ASSIGN);

struct INTEGER : public AbsToken
{
    int _val;
    INTEGER(int x) : _val(x) {};
    void accept(TokenVisitor& v) { v.VisitInt(this); }
    ~INTEGER() = default;
};

#define CONSTRUCT_DBG_VISTFUNC(TYPE) \
    virtual void Visit##TYPE(TYPE* node) override

class TokenDebugVisitor : public TokenVisitor{
public:
    TokenDebugVisitor() = default;
    virtual void VisitInt(INTEGER* node) override;
    CONSTRUCT_DBG_VISTFUNC(PLUS);
    CONSTRUCT_DBG_VISTFUNC(MINUS);
    CONSTRUCT_DBG_VISTFUNC(MUL);
    CONSTRUCT_DBG_VISTFUNC(DIV);
    CONSTRUCT_DBG_VISTFUNC(EndOfFile);
    CONSTRUCT_DBG_VISTFUNC(LPAREN);
    CONSTRUCT_DBG_VISTFUNC(RPAREN);
    CONSTRUCT_DBG_VISTFUNC(BEGIN);
    CONSTRUCT_DBG_VISTFUNC(END);
    CONSTRUCT_DBG_VISTFUNC(ID);
    CONSTRUCT_DBG_VISTFUNC(SEMI);
    CONSTRUCT_DBG_VISTFUNC(DOT);
    CONSTRUCT_DBG_VISTFUNC(ASSIGN);
    ~TokenDebugVisitor() = default;
};

template<typename TOKENTYPE>
bool IsTokenType(AbsToken* token)
{
    return dynamic_cast<TOKENTYPE* >(token) != nullptr;
}


#endif