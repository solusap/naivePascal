#ifndef _LEXER_H_
#define _LEXER_H_

#include "token.h"
#include <map>

using  ID_MAP = std::map<string, AbsToken*>;
class Lexer
{
public:    
    string text;
    size_t pos;
    char peek();
    ID_MAP reserved_id;
    Lexer(const string& str);
    AbsToken* Error();
    void advance();
    void skip_whitespace();
    void skip_comment();
    int build_integer();
    AbsToken* get_next_token();
    AbsToken* proc_id();
    AbsToken* number();  
};

#endif