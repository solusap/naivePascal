#ifndef _TOKEN_H_
#define _TOKEN_H_
#include <string>

using std::string;

class TokenVisitor;
struct INTEGER;
struct PLUS;
struct MINUS;
struct EndOfFile;

struct AbsToken 
{
    virtual void debug(TokenVisitor& v) = 0;     // accept
    virtual ~AbsToken() = default;
};

class TokenVisitor {
public:
    TokenVisitor() = default;;
    virtual void VisitInt(INTEGER* node) = 0;
    virtual void VisitPlus(PLUS* node) = 0;
    virtual void VisitMinus(MINUS* node) = 0;
    virtual void VisitEof(EndOfFile* node) = 0;
    virtual ~TokenVisitor() = default;
};


struct INTEGER : public AbsToken
{
    int _val;
    INTEGER(int x) : _val(x) {};
    void debug(TokenVisitor& v) { v.VisitInt(this); }
    ~INTEGER() = default;
};

struct PLUS: public AbsToken
{
    PLUS() = default;
    void debug(TokenVisitor& v) {
        v.VisitPlus(this);
    }
    ~PLUS() = default;
};

struct MINUS: public AbsToken
{
    MINUS() = default;
    void debug(TokenVisitor& v) {
        v.VisitMinus(this);
    }
    ~MINUS() = default;
};

struct EndOfFile: public AbsToken
{
    EndOfFile() {};
    void debug(TokenVisitor& v) {
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
    virtual void VisitEof(EndOfFile* node) override;
    ~TokenDebugVisitor() = default;
};

template<typename TOKENTYPE>
bool IsTokenType(AbsToken* token)
{
    return dynamic_cast<TOKENTYPE* >(token) != nullptr;
}


#endif