#pragma cone

#include <iostream>
#include <map>
#include <set>
#include "./to-nfa.h"

/*
 */
namespace Alg
{
    using std::map;
    using std::set;
    using char_type = char;

    struct StateTable
    {
    public:
        void print_tab();

        StateTable trim_tab();
        bool match_whole(const std::string &str);
    public:
        int entry = 0;
        set<int> fin_stat_tab;
        vector<map<char_type, int>> tab;
    };
    class SubsetAlg
    {
    public:
        SubsetAlg(Graph *_NFA) : NFA(_NFA)
        {
            gen_epsilon_tab();
        }
        StateTable gen_state_tab();
        ~SubsetAlg() {NFA->destroy(); delete NFA; }

    private:
        Graph *NFA;
        map<Node *, set<Node *>> closure_tab;

        // dfs
        set<Node *> epsilon_clo(Node *node)
        {
            std::vector<Node *> s;
            s.push_back(node);

            set<Node *> vis;
            vis.insert(node);
            while (!s.empty())
            {
                auto cur = s.back();
                s.pop_back();
                for (auto i = cur->head; i; i = i->next)
                {
                    if (!i->is_epsilon || vis.count(i->dest))
                        continue;
                    auto v = i->dest;
                    vis.insert(v);

                    if (closure_tab.count(v))
                    {
                        set<Node *> &tmp_set = closure_tab[v];
                        vis.insert(tmp_set.begin(), tmp_set.end());
                        continue;
                    }
                    s.push_back(v);
                }
            }
            return vis;
        }
        void gen_epsilon_tab()
        {
            vector<Node *> s;
            s.push_back(NFA->start);
            while (!s.empty())
            {
                auto cur = s.back();
                s.pop_back();
                if (closure_tab.count(cur))
                    continue;
                closure_tab.insert({cur, std::move(epsilon_clo(cur))});
                for (auto i = cur->head; i; i = i->next)
                {
                    if (!closure_tab.count(i->dest))
                        s.push_back(i->dest);
                }
            }
        }
    };
}