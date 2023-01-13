#include "../includes/subset-cons.h"

namespace Alg
{
    using std::pair;

    // a star on the front of the state number represents it is a finished state.
    void StateTable::print_tab()
    {
        int i = 0;
        for (auto line : tab)
        {
            if (i == entry)
                std::cout << "-";
            if (fin_stat_tab.count(i))
                std::cout << "*";
            std::cout << "\t" << i++ << " ";
            for (auto [ch, t] : line)
                std::cout << ch << "-" << t << " ";
            std::cout << "\n";
        }
    }
    bool StateTable::match_whole(const std::string &str)
    {
        int s = entry;
        for (auto ch : str)
        {
            if (tab[s].count(ch))
                s = tab[s][ch];
            else
                return false;
        }
        return fin_stat_tab.count(s);
    }
    StateTable SubsetAlg::gen_state_tab()
    {
        StateTable ret;
        int cnt = 0;
        // dfs stack
        vector<pair<int, set<Node *>>> s;
        map<set<Node *>, int> vis;

        vis.insert({closure_tab[NFA->start], cnt});
        s.push_back({cnt++, closure_tab[NFA->start]});
        while (!s.empty())
        {
            auto [id, cur_set] = std::move(s.back());
            s.pop_back();
            // ch -> new DFA state set
            map<char_type, set<Node *>> trans_tab;
            // ch -> new DFA state number
            map<char_type, int> trans_id_tab;
            bool is_fin_state = false;
            for (auto node : cur_set)
            {
                if (node->is_end)
                    is_fin_state = true;

                for (auto i = node->head; i; i = i->next)
                {
                    if (i->is_epsilon)
                        continue;
                    auto it = trans_tab.find(i->value);
                    set<Node *> tmp_tab = closure_tab[i->dest];
                    if (it == trans_tab.end())
                    {
                        trans_tab.insert({i->value, tmp_tab});
                        if (!vis.count(tmp_tab))
                            trans_id_tab.insert({i->value, cnt++});
                        else
                            trans_id_tab.insert({i->value, vis[tmp_tab]});
                    }
                    else
                        it->second.insert(tmp_tab.begin(), tmp_tab.end());
                }
            }
            if (ret.tab.size() <= id)
                ret.tab.resize(id + 1);

            if (is_fin_state)
                ret.fin_stat_tab.insert(id);
            ret.tab[id] = trans_id_tab;

            for (auto [ch, v] : trans_tab)
            {
                if (!vis.count(v))
                {
                    vis.insert({v, trans_id_tab[ch]});
                    s.push_back({trans_id_tab[ch], std::move(v)});
                }
            }
        }
        return ret;
    }

}