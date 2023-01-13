#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <map>
#include <set>
#include <queue>

// decls
namespace Alg
{
    using std::tuple;
    using std::vector;

    struct Edge;

    struct Node
    {
    public:
        Edge *head = nullptr;
        bool is_end = false;
        void push_edge(Edge *e);
        void destory_edges();
    public:
    };

    struct Edge
    {
    public:
        Edge(bool _is_ep, char _val, Node *_dst) : is_epsilon(_is_ep), value(_val), dest(_dst) {}

    public:
        bool is_epsilon;
        char value;
        Edge *next = nullptr;
        Node *dest;
    };

    struct Graph
    {
        Graph(Node *s, Node *e) : start(s), end(e) {}
        //destroy all the structures. including nodes edges. 
        void destroy();
        Node *start;
        Node *end;

    public:
        // implemented by dfs
        // format: <u,v,char>
        // you can view the graph by : https://csacademy.com/app/graph_editor/
        vector<tuple<int, int, char>> traverse_graph();

    public:
        static Graph *build_graph_by_char(char ch);
        // g1,g2 will destory!
        static Graph *concat(Graph *g1, Graph *g2);
        // g1,g2 will destroy!
        static Graph *cup(Graph *g1, Graph *g2);
        // g will destroy!
        static Graph *repeat(Graph *g, bool enable_zero);
    };
}