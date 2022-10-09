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

struct AbsToken 
{
    virtual void accept(TokenVisitor& v) = 0;     // accept
    virtual ~AbsToken() = default;
};

class TokenVisitor {
public:
    TokenVisitor() = default;;
    virtual void VisitInt(INTEGER* node) = 0;
    virtual void VisitPlus(PLUS* node) = 0;
    virtual void VisitMinus(MINUS* node) = 0;
    virtual void VisitMul(MUL* node) = 0;
    virtual void VisitDiv(DIV* node) = 0;
    virtual void VisitEof(EndOfFile* node) = 0;
    virtual void VisitLParen(LPAREN* node) = 0;
    virtual void VisitRParen(RPAREN* node) = 0;
    virtual ~TokenVisitor() = default;
};


struct INTEGER : public AbsToken
{
    int _val;
    INTEGER(int x) : _val(x) {};
    void accept(TokenVisitor& v) { v.VisitInt(this); }
    ~INTEGER() = default;
};

struct PLUS: public AbsToken
{
    PLUS() = default;
    void accept(TokenVisitor& v) {
        v.VisitPlus(this);
    }
    ~PLUS() = default;
};

struct MINUS: public AbsToken
{
    MINUS() = default;
    void accept(TokenVisitor& v) {
        v.VisitMinus(this);
    }
    ~MINUS() = default;
};

struct MUL: public AbsToken
{
    MUL() = default;
    void accept(TokenVisitor& v) {
        v.VisitMul(this);
    }
    ~MUL() = default;
};

struct DIV: public AbsToken
{
    DIV() = default;
    void accept(TokenVisitor& v) {
        v.VisitDiv(this);
    }
    ~DIV() = default;
};

struct LPAREN: public AbsToken
{
    LPAREN() = default;
    void accept(TokenVisitor& v) {
        v.VisitLParen(this);
    }
    ~LPAREN() = default;
};

struct RPAREN: public AbsToken
{
    RPAREN() = default;
    void accept(TokenVisitor& v) {
        v.VisitRParen(this);
    }
    ~RPAREN() = default;
};

struct EndOfFile: public AbsToken
{
    EndOfFile() {};
    void accept(TokenVisitor& v) {
        v.VisitEof(this);
    }
    ~EndOfFile() = default;
};


class TokenDebugVisitor : public TokenVisitor{
public:
    TokenDebugVisitor() = default;
    virtual void VisitInt(INTEGER* node) override;
    virtual void VisitPlus(PLUS* node) override;
    virtual void VisitMinus(MINUS* node) override;
    virtual void VisitMul(MUL* node) override;
    virtual void VisitDiv(DIV* node) override;
    virtual void VisitEof(EndOfFile* node) override;
    virtual void VisitLParen(LPAREN* node) override;
    virtual void VisitRParen(RPAREN* node) override;

    ~TokenDebugVisitor() = default;
};

template<typename TOKENTYPE>
bool IsTokenType(AbsToken* token)
{
    return dynamic_cast<TOKENTYPE* >(token) != nullptr;
}


#endif