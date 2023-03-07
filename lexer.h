#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <fstream>

namespace HLex
{
    using std::map;
    using std::vector;
    using char_type = char;
    enum Tag {SPACE,ADD,SUB,MUL,DIV,INT};
    std::string tag_to_str(Tag tag){
        static map<Tag,std::string> mp = {{SPACE,"SPACE"},{ADD,"ADD"},{SUB,"SUB"},{MUL,"MUL"},{DIV,"DIV"},{INT,"INT"}};
        return mp[tag];
    }
    struct Token
    {
        Tag tag;
        std::string val;
    };

    class Lexer
    {
    public:
        Lexer(const std::string &con):content(con)
        {
            entry = 0;
            fin_stat_tab = {{1,ADD},{2,DIV},{3,INT},{4,MUL},{5,SPACE},{6,SUB}};
            tab = {{{'\t',5},{' ',5},{'*',4},{'+',1},{'-',6},{'/',2},{'0',3},{'1',3},{'2',3},{'3',3},{'4',3},{'5',3},{'6',3},{'7',3},{'8',3},{'9',3}},{},{},{{'0',3},{'1',3},{'2',3},{'3',3},{'4',3},{'5',3},{'6',3},{'7',3},{'8',3},{'9',3}},{},{},{}};
            ignore = {SPACE};
            keywords = {};
            user_defs = {};
        }
        std::vector<Token> lex()
        {
            std::vector<Token> ret;
            int cur_state = entry;
            // to roll back state tag val
            std::vector<std::pair<int, Token>> pos_stac;
            // str pos
            int pos = 0;
            std::string cur_token;
            Tag cur_tag;

            while (pos < content.size())
            {
                if (tab[cur_state].count(content[pos]))
                {
                    cur_state = tab[cur_state][content[pos]];
                    cur_token += content[pos];

                    if (fin_stat_tab.count(cur_state))
                    {                            
                        cur_tag = fin_stat_tab[cur_state];
                        pos_stac.clear();
                        pos_stac.push_back({pos + 1, {cur_tag, cur_token}});
                    }
                    pos++;
                }
                else
                {
                    if (pos_stac.empty())
                    {
                        print_line(content, pos);
                        throw std::runtime_error(" LexerGenerator::lex: Lexer Error");
                    }
                    auto [p, tok] = pos_stac.back();
                    // if a symbol is a keyword or ignore
                    auto val = tok.val;
                    if (keywords.count(val))
                        tok = Token{keywords[val], val};
                    if (user_defs.count(tok.tag))
                    {
                        p -= tok.val.size();
                        tok.val = user_defs[tok.tag](content, p);
                    }
                    if (!ignore.count(tok.tag))
                        ret.push_back(tok);

                    // roll back
                    pos = p;
                    cur_state = entry;
                    cur_token = "";
                    pos_stac.clear();
                }
            }
            if (pos_stac.size())
                ret.push_back(pos_stac.back().second);
            return ret;
        }
        void print_line(const std::string &s, int &pos)
        {
            int cnt = 100;
            while (pos < s.size() && cnt)
            {
                std::cout << s[pos++];
                cnt--;
            }
            std::cout.flush();
        }

    private:
        std::string content;
        int pos = 0;
        std::map<std::string, Tag> keywords;
        std::map<Tag, std::function<std::string(const std::string &, int &)>> user_defs;
        std::set<Tag> ignore;

        int entry;
        map<int, Tag> fin_stat_tab;
        vector<map<char_type, int>> tab;
    };
}