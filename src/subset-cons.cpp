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
                std::cout << "*" << fin_stat_tab[i];
            std::cout << "\t\t" << i++ << " ";
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
        vector<pair<int, DS::BitSet>> s;
        unordered_map<DS::BitSet, int, DS::BitSetHash> vis;

        vis.insert({closure_tab[0], cnt});
        s.push_back({cnt++, closure_tab[0]});

        while (!s.empty())
        {
            auto [id, cur_set] = std::move(s.back());
            s.pop_back();
            // ch -> new DFA state set
            map<char_type, DS::BitSet> trans_tab;
            // ch -> new DFA state number
            map<char_type, int> trans_id_tab;
            bool is_fin_state = false;
            std::string fin_state_tag;

            auto vec = cur_set.to_vector();
            for (auto node : vec)
            {
                if (mg.is_end(node))
                {
                    is_fin_state = true;
                    if (fin_state_tag == "")
                        fin_state_tag = mg.node_tab[node]->val;
                }

                for (auto [v, ch] : mg.ng[node])
                {
                    if (ch == 0)
                        continue;
                    auto it = trans_tab.find(ch);
                    DS::BitSet tmp_tab = closure_tab[v];
                    if (it == trans_tab.end())
                        trans_tab.insert({ch, tmp_tab});

                    else
                        it->second |= tmp_tab;
                }
            }
            if (ret.tab.size() <= id)
                ret.tab.resize(id + 1);

            if (is_fin_state)
                ret.fin_stat_tab.insert({id, fin_state_tag});

            for (auto [ch, v] : trans_tab)
            {
                if (!vis.count(v))
                {
                    vis.insert({v, cnt});
                    s.push_back({cnt, std::move(v)});
                    trans_id_tab[ch] = cnt;
                    cnt++;
                }
                else
                {
                    trans_id_tab[ch] = vis[v];
                }
            }
            ret.tab[id] = trans_id_tab;
        }
        return ret;
    }

    DS::BitSet SubsetAlg::epsilon_clo(int u)
    {
        std::vector<int> s;
        s.push_back(u);

        DS::BitSet vis;
        vis.insert(u);
        while (!s.empty())
        {
            auto cur = s.back();
            s.pop_back();
            auto &vec = mg.ng[cur];
            for (auto [v, ch] : vec)
            {
                if (ch != 0 || vis.count(v))
                    continue;
                vis.insert(v);
                if (closure_tab.count(v))
                {
                    vis |= closure_tab[v];
                    continue;
                }
                s.push_back(v);
            }
        }
        return vis;
    }
    void SubsetAlg::gen_epsilon_tab()
    {
        vector<int> s;
        s.push_back(0);
        while (!s.empty())
        {
            auto cur = s.back();
            s.pop_back();
            if (closure_tab.count(cur))
                continue;
            closure_tab.insert({cur, std::move(epsilon_clo(cur))});
            auto &vec = mg.ng[cur];
            for (auto [v, ch] : vec)
            {
                if (!closure_tab.count(v))
                    s.push_back(v);
            }
        }
    }
}