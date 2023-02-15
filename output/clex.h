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
    enum Tag {STRUCT,BREAK,CASE,CHAR,CONST,CONTINUE,DEFAULT,DO,DOUBLE,ELSE,FLOAT,FOR,GOTO,IF,INT,LONG,REGISTER,RETURN,SHORT,SIGNED,STATIC,SWITCH,TYPEDEF,UNION,UNSIGNED,VOID,VOLATILE,WHILE,ANNOTATION,NUMBER,LONG_NUMBER,REAL,LITERAL,CHAR_LIT,SYMBOL,DOT,MEM_VIS,COLON,COMMA,SEMI,QUE,ADD,INC,DEC,SUB,MUL,DIV,LSH,RSH,MOD,BAND,BOR,BXOR,BNOT,ASSIGN,SADD,SSUB,SMUL,SDIV,SLSH,SRSH,SMOD,SAND,SOR,SXOR,LT,GT,LE,GE,EQ,NE,NOT,AND,OR,BEGIN,END,LPAR,RPAR,LSP,RSP,SPACE,NEWLINE};
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
            entry = 55;
            fin_stat_tab = {{1,ADD},{2,AND},{3,ANNOTATION},{4,ASSIGN},{5,BAND},{6,BEGIN},{7,BNOT},{8,BOR},{9,BXOR},{10,CHAR_LIT},{11,COLON},{12,COMMA},{13,DEC},{14,DIV},{15,DOT},{16,END},{17,EQ},{18,GE},{19,GT},{20,INC},{21,LE},{22,LITERAL},{23,LONG_NUMBER},{24,LPAR},{25,LSH},{26,LSP},{27,LT},{28,MEM_VIS},{29,MOD},{30,MUL},{31,NE},{32,NEWLINE},{33,NOT},{34,NUMBER},{35,OR},{36,QUE},{37,REAL},{38,RPAR},{39,RSH},{40,RSP},{41,SADD},{42,SAND},{43,SDIV},{44,SEMI},{45,SLSH},{46,SMOD},{47,SMUL},{48,SOR},{49,SPACE},{50,SRSH},{51,SSUB},{52,SUB},{53,SXOR},{54,SYMBOL}};
            tab = {{{'l',23}},{{'+',20},{'0',34},{'1',34},{'2',34},{'3',34},{'4',34},{'5',34},{'6',34},{'7',34},{'8',34},{'9',34},{'=',41}},{},{},{{'=',17}},{{'&',2},{'=',42}},{},{},{{'=',48},{'|',35}},{{'=',53}},{},{},{},{},{{'/',3},{'=',43}},{},{},{},{},{{'=',18},{'>',39}},{},{},{},{},{},{{'=',45}},{},{{'<',25},{'=',21}},{},{{'=',46}},{{'=',47}},{},{},{{'=',31}},{{'.',57},{'0',34},{'1',34},{'2',34},{'3',34},{'4',34},{'5',34},{'6',34},{'7',34},{'8',34},{'9',34},{'l',0}},{},{},{{'0',37},{'1',37},{'2',37},{'3',37},{'4',37},{'5',37},{'6',37},{'7',37},{'8',37},{'9',37}},{},{{'=',50}},{},{},{},{},{},{},{},{},{},{},{},{},{{'-',13},{'0',34},{'1',34},{'2',34},{'3',34},{'4',34},{'5',34},{'6',34},{'7',34},{'8',34},{'9',34},{'=',51},{'>',28}},{},{{'0',54},{'1',54},{'2',54},{'3',54},{'4',54},{'5',54},{'6',54},{'7',54},{'8',54},{'9',54},{'A',54},{'B',54},{'C',54},{'D',54},{'E',54},{'F',54},{'G',54},{'H',54},{'I',54},{'J',54},{'K',54},{'L',54},{'M',54},{'N',54},{'O',54},{'P',54},{'Q',54},{'R',54},{'S',54},{'T',54},{'U',54},{'V',54},{'W',54},{'X',54},{'Y',54},{'Z',54},{'_',54},{'a',54},{'b',54},{'c',54},{'d',54},{'e',54},{'f',54},{'g',54},{'h',54},{'i',54},{'j',54},{'k',54},{'l',54},{'m',54},{'n',54},{'o',54},{'p',54},{'q',54},{'r',54},{'s',54},{'t',54},{'u',54},{'v',54},{'w',54},{'x',54},{'y',54},{'z',54}},{{'\t',49},{'\n',32},{'\r',56},{' ',49},{'!',33},{'"',22},{'#',3},{'%',29},{'&',5},{'\'',10},{'(',24},{')',38},{'*',30},{'+',1},{',',12},{'-',52},{'.',15},{'/',14},{'0',34},{'1',34},{'2',34},{'3',34},{'4',34},{'5',34},{'6',34},{'7',34},{'8',34},{'9',34},{':',11},{';',44},{'<',27},{'=',4},{'>',19},{'?',36},{'A',54},{'B',54},{'C',54},{'D',54},{'E',54},{'F',54},{'G',54},{'H',54},{'I',54},{'J',54},{'K',54},{'L',54},{'M',54},{'N',54},{'O',54},{'P',54},{'Q',54},{'R',54},{'S',54},{'T',54},{'U',54},{'V',54},{'W',54},{'X',54},{'Y',54},{'Z',54},{'[',26},{']',40},{'^',9},{'_',54},{'a',54},{'b',54},{'c',54},{'d',54},{'e',54},{'f',54},{'g',54},{'h',54},{'i',54},{'j',54},{'k',54},{'l',54},{'m',54},{'n',54},{'o',54},{'p',54},{'q',54},{'r',54},{'s',54},{'t',54},{'u',54},{'v',54},{'w',54},{'x',54},{'y',54},{'z',54},{'{',6},{'|',8},{'}',16},{'~',7}},{{'\n',32}},{{'0',37},{'1',37},{'2',37},{'3',37},{'4',37},{'5',37},{'6',37},{'7',37},{'8',37},{'9',37}}};
            ignore = {ANNOTATION,SPACE};
            keywords = {{"",STRUCT},{"break",BREAK},{"case",CASE},{"char",CHAR},{"const",CONST},{"continue",CONTINUE},{"default",DEFAULT},{"do",DO},{"double",DOUBLE},{"else",ELSE},{"float",FLOAT},{"for",FOR},{"goto",GOTO},{"if",IF},{"int",INT},{"long",LONG},{"register",REGISTER},{"return",RETURN},{"short",SHORT},{"signed",SIGNED},{"static",STATIC},{"switch",SWITCH},{"typedef",TYPEDEF},{"union",UNION},{"unsigned",UNSIGNED},{"void",VOID},{"volatile",VOLATILE},{"while",WHILE}};
            user_defs = {{ANNOTATION,[](const std::string &s,int &pos){
    while(pos<s.size() && s[pos]!='\n')
        pos++;
    return "";
}},{CHAR_LIT,[](const std::string &s,int &pos){
    pos++;
    std::map<char, std::string> escape = {
        {'n', "\n"}, {'t', "\t"}, {'r', "\r"}, {'\\', "\\"}, {'\'', "\\'"}, {'\"', "\""}};
    if (s[pos] == '\\')
    {
        pos++;
        if (pos == s.size() || !escape.count(s[pos]))
            throw std::runtime_error("invalid char");
        else
            return escape[s[pos++]];
    }
    // simple version
    std::string ret = std::string(1, s[pos]);
    pos++;
    return ret;
}},{LITERAL,[](const std::string &s,int &pos){
    pos++;
    std::map<char, std::string> escape = {
         {'n', "\n"}, {'t', "\t"}, {'r', "\r"}, {'\\', "\\"}, {'\'', "\\'"}, {'\"', "\""}};
    std::string ret = "";
    while (pos < s.size() && s[pos] != '\"')
    {
        if (s[pos] == '\\')
        {
            if (pos == s.size())
                throw std::runtime_error("invalid string");
            pos++;
            if (!escape.count(s[pos]))
                throw std::runtime_error("invalid escape string!");
            ret += escape[s[pos]];
        }
        else
            ret += s[pos];
        pos++;
    }
    pos++;
    return ret;
}}};
            tag_str_tab ={{STRUCT,"STRUCT"},{BREAK,"BREAK"},{CASE,"CASE"},{CHAR,"CHAR"},{CONST,"CONST"},{CONTINUE,"CONTINUE"},{DEFAULT,"DEFAULT"},{DO,"DO"},{DOUBLE,"DOUBLE"},{ELSE,"ELSE"},{FLOAT,"FLOAT"},{FOR,"FOR"},{GOTO,"GOTO"},{IF,"IF"},{INT,"INT"},{LONG,"LONG"},{REGISTER,"REGISTER"},{RETURN,"RETURN"},{SHORT,"SHORT"},{SIGNED,"SIGNED"},{STATIC,"STATIC"},{SWITCH,"SWITCH"},{TYPEDEF,"TYPEDEF"},{UNION,"UNION"},{UNSIGNED,"UNSIGNED"},{VOID,"VOID"},{VOLATILE,"VOLATILE"},{WHILE,"WHILE"},{ANNOTATION,"ANNOTATION"},{NUMBER,"NUMBER"},{LONG_NUMBER,"LONG_NUMBER"},{REAL,"REAL"},{LITERAL,"LITERAL"},{CHAR_LIT,"CHAR_LIT"},{SYMBOL,"SYMBOL"},{DOT,"DOT"},{MEM_VIS,"MEM_VIS"},{COLON,"COLON"},{COMMA,"COMMA"},{SEMI,"SEMI"},{QUE,"QUE"},{ADD,"ADD"},{INC,"INC"},{DEC,"DEC"},{SUB,"SUB"},{MUL,"MUL"},{DIV,"DIV"},{LSH,"LSH"},{RSH,"RSH"},{MOD,"MOD"},{BAND,"BAND"},{BOR,"BOR"},{BXOR,"BXOR"},{BNOT,"BNOT"},{ASSIGN,"ASSIGN"},{SADD,"SADD"},{SSUB,"SSUB"},{SMUL,"SMUL"},{SDIV,"SDIV"},{SLSH,"SLSH"},{SRSH,"SRSH"},{SMOD,"SMOD"},{SAND,"SAND"},{SOR,"SOR"},{SXOR,"SXOR"},{LT,"LT"},{GT,"GT"},{LE,"LE"},{GE,"GE"},{EQ,"EQ"},{NE,"NE"},{NOT,"NOT"},{AND,"AND"},{OR,"OR"},{BEGIN,"BEGIN"},{END,"END"},{LPAR,"LPAR"},{RPAR,"RPAR"},{LSP,"LSP"},{RSP,"RSP"},{SPACE,"SPACE"},{NEWLINE,"NEWLINE"}};
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
        std::string tag_to_str(Tag t){
            if(tag_str_tab.count(t))
                return tag_str_tab[t];
            return "";
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
        std::map<Tag,std::string> tag_str_tab;
    };
}