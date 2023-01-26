#include "../includes/parser.h"
#include <string>

namespace ReParser
{
    Alg::Graph *BinOp::gen_graph()
    {
        auto left_graph = left->gen_graph();
        auto right_graph = right->gen_graph();

        if (type == CUP)
        {
            return Alg::Graph::cup(left_graph, right_graph);
        }
        else if (type == CONCAT)
        {
            return Alg::Graph::concat(left_graph, right_graph);
        }
        else
        {
            error("invalid bin operator");
        }
    }
    Alg::Graph *Repeat::gen_graph()
    {
        if (positive == -1)
            return Alg::Graph::optional(unit->gen_graph());
        auto g = unit->gen_graph();
        return Alg::Graph::repeat(g, !positive);
    }
    Alg::Graph *Unit::gen_graph()
    {
        return Alg::Graph::build_graph_by_char(ch);
    }

    ParserNode *parse_re(const std::string &s, int &pos)
    {
        return nullptr;
    }
    void skip_space(const std::string &s, int &pos)
    {
        while (pos < s.size() && s[pos] == ' ')
            pos++;
    }
    void match(const std::string &s, int &pos, Alg::char_type ch)
    {
        skip_space(s, pos);
        if (s[pos] != ch)
            error("expect " + std::string(1, ch) + " but got " + s[pos]);
        pos++;
        skip_space(s, pos);
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
    //get and move
    char get_ch(const std::string &s, int &pos)
    {
        std::map<Alg::char_type, Alg::char_type> escape_tab = {
            {'s', ' '}, {'|', '|'}, {'n', '\n'}, {'r', '\r'}, {'t', '\t'}, {'(', '('}, {')', ')'}, {'[', '['}, {']', ']'}, {'+', '+'}, {'*', '*'}, {'-', '-'}};
        skip_space(s, pos);
        char ret = 0;
        if (pos < s.size())
        {
            if (s[pos] == '\\')
            {
                pos++;
                if (pos == s.size())
                    throw std::runtime_error("ReParser::get_ch(): escape unexpect eof!");
                // undefined escape char.
                if (!escape_tab.count(s[pos]))
                    return s[pos++];
                return escape_tab[s[pos++]];
            }
            else
                return s[pos++];
        }
    }
    Alg::Graph *parser(const std::string &str)
    {
        int pos = 0;
        auto tmp = parse_expr(str, pos);
        auto ret = tmp->gen_graph();
        delete tmp;
        return ret;
    }
    ParserNode *parse_expr(const std::string &s, int &pos)
    {
        auto cur_node = parse_concat(s, pos);
        while (pos < s.size())
        {
            if (s[pos] == '|')
            {
                skip_space(s, pos);
                pos++;
                auto right = parse_concat(s, pos);
                cur_node = new BinOp(OpType::CUP, cur_node, right);
            }
            else
                return cur_node;
        }
        return cur_node;
    }
    ParserNode *parse_concat(const std::string &s, int &pos)
    {
        std::set<char> reserved_char = {')', ']', '|'};
        auto cur_node = parse_term(s, pos);
        while (pos < s.size())
        {
            skip_space(s, pos);
            char cur_ch = s[pos];
            if (!reserved_char.count(cur_ch))
            {
                auto right = parse_term(s, pos);
                cur_node = new BinOp(OpType::CONCAT, cur_node, right);
            }
            else
                return cur_node;
        }
        return cur_node;
    }
    ParserNode *parse_term(const std::string &s, int &pos)
    {
        auto cur_node = parse_factor(s, pos);
        if (pos < s.size())
        {
            skip_space(s, pos);
            if (pos == s.size())
                return cur_node;
            if (s[pos] == '*' || s[pos] == '+' || s[pos] == '?')
            {
                int is_p = 0;
                if (s[pos] == '?')
                    is_p--;
                else if (s[pos] == '+')
                    is_p++;
                cur_node = new Repeat(cur_node, is_p);
                pos++;
                return cur_node;
            }
            else
                return cur_node;
        }
        return cur_node;
    }
    ParserNode *parse_factor(const std::string &s, int &pos)
    {
        skip_space(s, pos);
        if (pos == s.size())
            error("unexpected end. ");
        if (s[pos] == '(')
        {
            match(s, pos, '(');
            auto expr = parse_expr(s, pos);
            match(s, pos, ')');
            return expr;
        }
        // range
        else if (s[pos] == '[')
        {
            ParserNode *ret = nullptr;
            match(s, pos, '[');
            do
            {
                Alg::char_type start = get_ch(s,pos);
                ParserNode *cur = nullptr;
                if (s[pos] == '-')
                {
                    match(s, pos, '-');
                    Alg::char_type end = get_ch(s,pos);
                    if (end <= start)
                        error("invalid range ");

                    cur = new Unit(start);
                    for (char i = start + 1; i <= end; i++)
                        cur = new BinOp(OpType::CUP, cur, new Unit(i));
                }
                else
                    cur = new Unit(start);
                if (ret == nullptr)
                    ret = cur;
                else
                    ret = new BinOp(OpType::CUP, ret, cur);
            } while (s[pos] != ']');
            match(s, pos, ']');
            return ret;
        }
        return parse_atom(s, pos);
    }
    ParserNode *parse_atom(const std::string &s, int &pos)
    {
        char cur = get_ch(s, pos);
        return new Unit(cur);
    }
    void error(const std::string &msg)
    {
        std::cout << msg << std::endl;
        exit(1);
    }

    // class methold defination ....
    void BinOp::print() const
    {
        std::cout << "(";
        if (type == CONCAT)
        {
            std::cout << "concat ";
        }
        else if (type == CUP)
        {
            std::cout << "cup ";
        }
        left->print();
        std::cout << " ";
        right->print();
        std::cout << ")";
    }
    void Repeat::print() const
    {
        std::cout << "(";
        std::cout << "*+"[positive] << " ";
        unit->print();
        std::cout << ")";
    }
    void Unit::print() const
    {
        std::cout << ch;
    }

    namespace Test
    {
        // 2023-1-13 : 10 times 1.65625 O2 WSL
        // 2023-1-14 : 10 times 0.04687 O2 WSL
        void case1()
        {
            const std::string patten = "[_a-zA-Z][_a-zA-Z0-9]+";
            int pos = 0;
            ParserNode *tmp = parse_expr(patten, pos);
            // tmp->print();
            auto g = tmp->gen_graph();

            Alg::SubsetAlg sa(g);

            auto tab = sa.gen_state_tab().trim_tab();

            // tab.print_tab();
            std::string str = "jdlkajglk21098jdlkajf21098hlsajg091238h";
            // std::getline(std::cin, str);
            if (tab.match_whole(str))
                std::cout << "AC!\n";
            else
                std::cout << "WA!\n";

            delete tmp;
        }
    }
}