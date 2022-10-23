#include "token.h"
#include "lexer.h"
#include <cctype>
#include <fmt/core.h>
#include <execinfo.h>

using fmt::print;

void print_stacktrace()
{
    int size=16;
    void*array[16];
    int stack_num = backtrace(array,size);
    char**stacktrace= backtrace_symbols(array,stack_num);
    for(int i=0;i < stack_num; ++i){
        print("{}\n",stacktrace[i]);
    }
    free(stacktrace);
}

Lexer::Lexer(const string& str) : text(str), pos(0) { 
        reserved_id["BEGIN"] = new BEGIN();
        reserved_id["END"] = new END();
    };

AbsToken* Lexer::Error()
{
    print_stacktrace();
    print("Error in parsing input text at {}, {}!\n", pos, text.at(pos));
    std::exit(1);
    return nullptr;
}

AbsToken* Lexer::proc_id()
{
    string res = "";
    while (pos != string::npos && std::isalnum(text.at(pos))) {
        res += text.at(pos);
        advance();
    }
    if (reserved_id.count(res)) {
        return reserved_id.at(res);
    }
    return new ID(res);
}

char Lexer::peek()
{
    size_t peek_pos = this->pos + 1;
    if (peek_pos >= text.length()) {
        return -1;
    } else {
        return text.at(peek_pos);
    }
}

void Lexer::advance()
{
    pos += 1;
    if (pos >= text.length()) {
        pos = std::string::npos;
    }
}

void Lexer::skip_whitespace()
{
    while (pos != std::string::npos && text[pos] == ' ') {
        advance();
    }
}

int Lexer::build_integer()
{
    int start = pos;
    while (pos != std::string::npos && std::isalnum(text[pos])) {
        advance();
    }
    return std::stoi(text.substr(start, pos - start));
}

AbsToken* Lexer::get_next_token()
{
    while (pos != std::string::npos) {
        if (text[pos] == ' ') {
            skip_whitespace();
            continue;
        }
        if (std::isdigit(text[pos])) {
            return new INTEGER(build_integer());
        } else if (std::isalpha(text[pos])) {
            return proc_id();
        }
        switch(text[pos]) {
            case '+':
                advance();
                return new PLUS();
            case '-':
                advance();
                return new MINUS();
            case '*':
                advance();
                return new MUL();
            case '/':
                advance();
                return new DIV();
            case '(':
                advance();
                return new LPAREN();
            case ')':
                advance();
                return new RPAREN();
            case ':':
                if (peek() == '=') {
                    advance();
                    advance();
                    return new ASSIGN();
                }
            case ';':
                advance();
                return new SEMI();
            case '.':
                advance();
                return new DOT();
            case '\n':
                advance();
                continue;            
            default:
                break;
        }
        
        return Error();
    }
    return new EndOfFile();
}