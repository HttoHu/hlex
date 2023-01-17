#pragma once
#include "../includes/subset-cons.h"
namespace ReParser
{
    class ParserNode
    {
    public:
        virtual Alg::Graph *gen_graph() { return nullptr; }

        virtual void print() const
        {
            std::cout << "(not defined)";
        }
        virtual ~ParserNode() {}

    private:
    };

    enum OpType
    {
        CONCAT,
        CUP,
        REP,
        PREP
    };

    class BinOp : public ParserNode
    {
    public:
        BinOp(OpType _type, ParserNode *l, ParserNode *r) : type(_type), left(l), right(r) {}
        Alg::Graph *gen_graph() override;
        void print() const override;
        ~BinOp()
        {
            delete left;
            delete right;
        }

    private:
        OpType type;
        ParserNode *left, *right;
    };
    class Repeat : public ParserNode
    {
    public:
        Repeat(ParserNode *n, int is_positive) : unit(n), positive(is_positive) {}
        Alg::Graph *gen_graph() override;
        void print() const override;
        ~Repeat() { delete unit; }

    private:
        // -1: ? , 0: * , 1:+
        int positive;
        ParserNode *unit;
    };
    class Unit : public ParserNode
    {
    public:
        Unit(Alg::char_type _ch) : ch(_ch) {}
        Alg::Graph *gen_graph() override;
        void print() const override;

    private:
        Alg::char_type ch;
    };
    void print_line(const std::string &s, int &pos);
    Alg::Graph *parser(const std::string &str);
    ParserNode *parse_re(const std::string &s, int &pos);
    ParserNode *parse_expr(const std::string &s, int &pos);
    ParserNode *parse_concat(const std::string &s, int &pos);
    ParserNode *parse_term(const std::string &s, int &pos);
    ParserNode *parse_factor(const std::string &s, int &pos);
    ParserNode *parse_atom(const std::string &s, int &pos);
    void error(const std::string &msg);
    namespace Test
    {
        void case1();
    }
}