#include "token.h"
#include "lexer.h"
#include <cctype>
#include <fmt/core.h>
#include <execinfo.h>
#include <string>

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
        reserved_id["VAR"] = new VAR();
        reserved_id["DIV"] = new INTEGER_DIV();
        reserved_id["INTEGER"] = new INTEGER();
        reserved_id["REAL"] = new REAL();
        reserved_id["PROGRAM"] = new PROGRAM();

    };

AbsToken* Lexer::Error()
{
    print_stacktrace();
    print("Error in parsing input text at {}, {}!\n", pos, text.substr(pos, text.length() - pos));
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

void Lexer::skip_comment()
{
    while (pos != std::string::npos && text[pos] != '}') {
        advance();
    }
    advance();
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
        } else if (text[pos] == '{') {
            advance();
            skip_comment();
            continue;
        }
 
        if (std::isdigit(text[pos])) {
            return number();
        } else if (std::isalpha(text[pos])) {
            return proc_id();
        } else if (text[pos] == ':' && peek() == '=') {
            advance();
            advance();
            return new ASSIGN();
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
                return new FLOAT_DIV();
            case '(':
                advance();
                return new LPAREN();
            case ')':
                advance();
                return new RPAREN();
            case ':':
                advance();
                return new COLON();
            case ';':
                advance();
                return new SEMI();
            case '.':
                advance();
                return new DOT();
            case ',':
                advance();
                return new COMMA();
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

AbsToken* Lexer::number()
{
    string result_tmp = "";
    while (pos != std::string::npos && std::isdigit(text[pos])) {
        result_tmp += text[pos];
        advance();
    }

    if (text[pos] == '.') {
        result_tmp += text[pos];
        advance();
        while (pos != std::string::npos && std::isdigit(text[pos])) {
            result_tmp += text[pos];
            advance();
        }
        return new REAL_CONST(std::stod(result_tmp));
    }
    return new INTEGER_CONSTANT(std::stoi(result_tmp));

}