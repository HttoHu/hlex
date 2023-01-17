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

    struct Token
    {
        std::string tag;
        std::string val;
        std::string to_string() const
        {
            return "<" + tag + "," + val + ">";
        }
    };
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
    class Lexer
    {
    public:
        Lexer(const std::string &con);
        std::vector<Token> lex()
        {
            std::vector<Token> ret;
            int cur_state = entry;
            // to roll back state tag val
            std::vector<std::pair<int, Token>> pos_stac;
            // str pos
            int pos = 0;
            std::string cur_token;
            std::string cur_tag;

            while (pos < content.size())
            {
                if (tab[cur_state].count(content[pos]))
                {
                    cur_state = tab[cur_state][content[pos]];
                    cur_token += content[pos];

                    if (fin_stat_tab.count(cur_state))
                    {
                        if (fin_stat_tab[cur_state] != "")
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
                        print_line(content,pos);
                        throw std::runtime_error(" LexerGenerator::lex: Lexer Error");
                    }
                    auto [p, tok] = pos_stac.back();
                    // if a symbol is a keyword pr ignore
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
                    cur_token = cur_tag = "";
                    pos_stac.clear();
                }
            }
            if (pos_stac.size())
                ret.push_back(pos_stac.back().second);
            return ret;
        }

    private:
        std::string content;
        int pos = 0;

        std::map<std::string, std::string> keywords;
        std::map<std::string, std::function<std::string(const std::string &, int &)>> user_defs;
        std::set<std::string> ignore;

        int entry;
        map<int, std::string> fin_stat_tab;
        vector<map<char_type, int>> tab;
    };
    
    std::string read_file(const std::string &filename)
    {
        std::ifstream ifs(filename, std::ios::in | std::ios::binary);
        if (!ifs)
            throw std::runtime_error("open file " + filename + " failed\n");
        char *file_content;
        ifs.seekg(0, std::ios::end);
        size_t file_length = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        file_content = new char[file_length];

        ifs.read(file_content, file_length);
        ifs.close();

        std::string str(file_content, file_content + file_length);
        delete[] file_content;
        return str;
    }
}