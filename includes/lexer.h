#pragma once
#include <iostream>
#include "parser.h"

namespace Lexer
{
    struct RuleLine
    {
        enum Tag
        {
            RE,
            SPECIFY,
            USER_DEF,
            END
        };

        Tag kind;
        std::string symbol;
        std::string content;
        std::map<std::string, std::string> specify_tab;
    };
    class Scanner
    {
    public:
        Scanner(const std::string &con) : content(con) {}
        void match(char ch);
        char get_ch();
        RuleLine next_ruleline();

    private:
        void skip();
        std::string read_word();
        std::string read_to_dollar();
        int pos = 0;
        std::string content;
    };

    struct Token
    {
        std::string tag;
        std::string val;
        std::string to_string() const
        {
            return "<" + tag +"," + val + ">";
        }
    };
    class LexerGenerator
    {
    public:
        LexerGenerator(Scanner s);
        std::vector<Token> lex(const std::string &str);
        std::string gen_code();

    private:
        std::vector<RuleLine> rules;
        Alg::StateTable st;
    };
}