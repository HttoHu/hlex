#pragma cone

#include <iostream>
#include <unordered_map>
#include <set>
#include <map>
#include "./to-nfa.h"
#include "./mbitset.h"

/*
 */
namespace Alg
{
    using std::unordered_map;
    using std::set;
    using std::map;
    
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
        SubsetAlg(Graph *_NFA) : NFA(_NFA), mg(_NFA)
        {
            gen_epsilon_tab();
        }
        StateTable gen_state_tab();
        ~SubsetAlg()
        {
            NFA->destroy();
            delete NFA;
        }

    private:
        Graph *NFA;
        MGraph mg;
        unordered_map<int, DS::BitSet> closure_tab;

        // dfs to gen epsilon closure from u
        DS::BitSet epsilon_clo(int u);
        // gen all state epsilon closure 
        void gen_epsilon_tab();
    };
}