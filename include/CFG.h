#ifndef CFG_H
#define CFG_H

template <typename V_t, typename S_t >
class CFG
{
    class Symbols: public V_t, T_t
    {
    public:
        Symbols(const V_t & v)
            : V_t(v), is_terminal(false)
        {}
        Symbols(const T_t & t)
            : T_t(t), is_terminal(true)
        {}
        const bool is_terminal;
        
    };
    using Body   = vectors<Symbols>;
    using Bodies = unordered_set<Body>;
    using Setv   = unordered_set<V_t>;
public:

    Setv Epsilon_Productions() const;
    
    Setv V_;
    unordered_set<T_t> sigma_;
    unordered_map<V_t, Bodies> P_;
    V_t S_;
    Setv nullables_;
};

class Parse_Tree
{
    Parse_Tree_Node root_;
};

class Parse_Tree_Node
{
    Token x;
    vector<Parse_Tree_Node> children_;
};

#endif
