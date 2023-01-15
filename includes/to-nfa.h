#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <functional>

#include <map>
#include <set>
#include <queue>

// decls
namespace Alg
{
    using std::tuple;
    using std::vector;
    using char_type = char;

    struct Edge;

    struct Node
    {
    public:
        Edge *head = nullptr;
        bool is_end = false;
        std::string val;

        void push_edge(Edge *e);
        void destory_edges();

    public:
    };

    struct Edge
    {
    public:
        Edge(bool _is_ep, char_type _val, Node *_dst) : is_epsilon(_is_ep), value(_val), dest(_dst) {}

    public:
        bool is_epsilon;
        char_type value;
        Edge *next = nullptr;
        Node *dest;
    };

    struct Graph
    {
        Graph(Node *s, Node *e) : start(s), end(e) {}
        // destroy all the structures. including nodes edges.
        void destroy();
        // match a fin state and get the word tag.
        void attach_tag(const std::string &str)
        {
            end->val = str;
        }
        Node *start;
        Node *end;

    public:
        // implemented by dfs
        // format: <u,v,char>
        // you can view the graph by : https://csacademy.com/app/graph_editor/
        void traverse_graph(std::function<void(Node *)> f);

    public:
        static Graph *build_graph_by_char(char ch);
        // g1,g2 will destory!
        static Graph *concat(Graph *g1, Graph *g2);
        // g1,g2 will destroy!
        static Graph *cup(Graph *g1, Graph *g2);
        // g will destroy!
        static Graph *repeat(Graph *g, bool enable_zero);
    };

    // build a simple number-idx-graph by a Graph , and all nodes from old graph will represent as a number from 1 to n.
    // 0 is the start
    struct MGraph
    {
        MGraph(Graph *g);
        void print();
        bool is_end(int u);

        std::vector<Node *> node_tab;

        std::vector<std::vector<std::pair<int, char>>> ng;
    };
}