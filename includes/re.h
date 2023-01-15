#pragma once
#include <string>
#include "parser.h"

namespace Htto
{
    struct ReResult
    {
        int start;
        std::string str;
    };
    class ReExpr
    {
    public:
        ReExpr(const std::string &re_expr);
        ReResult match(const std::string &str, int start = 0);
    private:
        Alg::StateTable st;
    };
}