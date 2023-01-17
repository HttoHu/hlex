#include "../includes/re.h"
namespace Htto
{
    ReExpr::ReExpr(const std::string &re_expr)
    {
        auto graph = ReParser::parser(re_expr);
        // graph moved to sa
        Alg::SubsetAlg sa(graph);
        st = sa.gen_state_tab();
        
    }
    ReResult ReExpr::match(const std::string &str, int start)
    {
        ReResult ret;
        while (start < str.size())
        {
            int cur_state = st.entry;

            ret.str = "";
            ret.start = start;
            bool matched = false;
            for (int i = start; i < str.size(); i++)
            {
                char ch = str[i];
                if (!st.tab[cur_state].count(ch))
                {
                    if (matched)
                        return ret;
                    else
                    {
                        start++;
                        break;
                    }
                }
                else
                {
                    cur_state = st.tab[cur_state][ch];
                    if (st.fin_stat_tab.count(cur_state))
                        matched = true;
                    ret.str += ch;
                }
            }
            if (matched)
                return ret;
            start++;
        }
        ret.start = -1, ret.str = "";
        return ret;
    }
}