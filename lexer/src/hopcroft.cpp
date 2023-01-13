#include "../includes/subset-cons.h"

#define CHECK_INTERNAL_ERROR
namespace Alg
{
    using std::cout;
    using std::pair;
    namespace
    {
        vector<set<int>> split_states(std::vector<pair<int, int>> &vec)
        {
            vector<set<int>> ret(1);
            sort(vec.begin(), vec.end());
            // two pointers to split different destination by same char.
            int l = 0, r = 1;
            ret[0].insert(vec[0].second);
            while (r < vec.size())
            {
                while (r < vec.size() && vec[r].first == vec[l].first)
                    ret.back().insert(vec[r].second), r++;
                l = r;
                if (r < vec.size())
                    ret.push_back({});
            }
            return ret;
        }
        void update_states_tab(vector<set<int>> &split_vec,
                               int old_stat_id,
                               int &cur_stat_cnt,
                               map<set<int>, int> &new_states_tab,
                               map<int, int> &old_new_tab)
        {
            sort(split_vec.begin(), split_vec.end(), [](auto &a, auto &b)
                 { return a.size() > b.size(); });
            // the biggest set remain old set number.
            bool is_first = true;

            for (auto new_set : split_vec)
            {
                if (is_first)
                {
                    new_states_tab.insert({new_set, old_stat_id});
                    is_first = false;
                    continue;
                }
                new_states_tab.insert({new_set, cur_stat_cnt});
                for (auto cur_state : new_set)
                    old_new_tab[cur_state] = cur_stat_cnt;
                cur_stat_cnt++;
            }
        }
    }

    StateTable StateTable::trim_tab()
    {
        bool flag = true;
        map<set<int>, int> new_states_tab;
        // init split the states to fin states or non-fin states

        // old_state -> new_state
        map<int, int> old_new_tab;
        // 0-non fin,1 fin
        set<int> init_states[2];
        for (int i = 0; i < tab.size(); i++)
        {
            init_states[fin_stat_tab.count(i)].insert(i);
            old_new_tab[i] = fin_stat_tab.count(i);
        }
        // insert init states set
        new_states_tab.insert({init_states[0], 0});
        new_states_tab.insert({init_states[1], 1});
        int new_states_cnt = 2;

        // fixed point iterative.
        while (flag)
        {
            flag = false;
            for (auto it = new_states_tab.begin(); it != new_states_tab.end();)
            {
                // char , <dest_new_state,old_state> ; collect information
                map<char, vector<pair<int, int>>> char_conv_tab;
                for (auto stat : it->first)
                {
                    for (auto [ch, num] : tab[stat])
                        char_conv_tab[ch].push_back({old_new_tab[num], stat});
                }

                bool it_advanced = false;
                // try to split the state
                for (auto &[ch, vec] : char_conv_tab)
                {
                    // some states don't have the ch transfer, we need insert them to vec
                    set<int> calced;
                    for (auto tmp : vec)
                        calced.insert(tmp.second);
                    for (auto cur_s : it->first)
                        if (!calced.count(cur_s))
                            vec.push_back({-1, cur_s});
                    auto cur_split = split_states(vec);
                    if (cur_split.size() > 1)
                    {
                        flag = true;
                        update_states_tab(cur_split, it->second, new_states_cnt, new_states_tab, old_new_tab);
                        it = new_states_tab.erase(it);
                        it_advanced = true;
                        break;
                    }
                }
                if (!it_advanced)
                    it++;
            }
        }

        // gen_new_tab
        StateTable ret;
        ret.tab.resize(new_states_cnt);
        for (int old_state = 0; old_state < tab.size(); old_state++)
        {
            int new_state = old_new_tab[old_state];

            if (this->fin_stat_tab.count(old_state))
                ret.fin_stat_tab.insert(new_state);
            if (old_state == entry)
                ret.entry = new_state;
            for (auto [ch, dest] : tab[old_state])
            {
#ifdef CHECK_INTERNAL_ERROR
                if (ret.tab[new_state].count(ch) && ret.tab[new_state][ch] != old_new_tab[dest])
                {
                    std::cerr << "trim_tab(): internal error, algorithm is contradictory!\n";
                    exit(1);
                }
#endif
                ret.tab[new_state][ch] = old_new_tab[dest];
            }
        }
        return ret;
    }
}
namespace Test
{
    namespace Hopcroft
    {
        using namespace Alg;
        void case1()
        {
            StateTable tmp;
            tmp.tab.resize(6);
            auto &tab = tmp.tab;
            tab[0].insert({'f', 1});
            tab[1].insert({'e', 2});
            tab[1].insert({'i', 4});
            tab[2].insert({'e', 3});
            tab[4].insert({'e', 5});
            tmp.fin_stat_tab.insert(3);
            tmp.fin_stat_tab.insert(5);
            /*
            expect output : 
        0 e-1 
*       1 
-       2 f-3 
        3 e-0 i-0 
            */
            tmp.trim_tab().print_tab();
        }
    }
}