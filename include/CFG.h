#ifndef CFG_H
#define CFG_H

#include "STL_util.h"
#include <variant>

template <typename V_t, typename S_t >
class CFG
{
    using Symbol = variant<V_t, S_t>;
    using Body   = list<Symbol>;
    using Bodies = unordered_set<Body>;
    using Setv   = unordered_set<V_t>;
    using Sets   = unordered_set<S_t>;
    using MapvB  = unordered_map<V_t, Bodies>;
public:
    CFG(const Setv & V, const Sets & sigma, const MapvB & P,
        const V_t & S, Setv NULLABLES)
        : V_(V), sigma_(sigma), P_(P), S_(S)
    {}
    CFG(const V_t & v)
        : V_({v}), S_(v)
    {}
    void add_variable(const V_t & v)
    {
        V_.insert(v);
    }
    void add_terminal(const S_t & t)
    {
        sigma_.insert(t);
    }
    void add_production(const V_t & v, const Body & b)
    {
        V_.insert(v);
        for (auto & sym: b)
        {
            if (sym.index() == 0)
            {
                V_t var = get<0>(sym);
                V_.insert(var);
            }
            else
            {
                S_t term = get<1>(sym);
                sigma_.insert(term);
            }
        }

        P_[v].insert(b);
    }
    void add_e_transition(const V_t & v)
    {
        V_.insert(v);
        NULLABLES_.insert(v);
    }
    void remove_E_productions();
    void remove_unit_productions();
    void remove_useless_variables();
    void Normalize();
    
    
    Setv  V_;
    Sets  sigma_;
    MapvB P_;
    V_t   S_;
    Setv  NULLABLES_;
};


template <typename V_t, typename S_t>
void CFG<V_t, S_t>::Normalize()
{
    // remove stuff
    // remove epsilon_productions
    remove_E_productions();
    // remove unit_productions
    remove_unit_productions();
    // useless Variables
    remove_useless_variables();
}

template <typename V_t, typename S_t>
void CFG<V_t, S_t>::remove_E_productions()
{
    Setv Old_Nullables = NULLABLES_;

    while (true)
    {
        for (auto q: V_)
        {
            auto p = P_.find(q);
            if ( is_member(q, NULLABLES_) || p == P_.end() )
                break;

            for (auto r: p->second)
            {
                Setv union_ = unordered_set<V_t>(r.begin(), r.end()) | NULLABLES_;

                if (union_ == NULLABLES_)
                {
                    Old_Nullables.insert(q);
                }
                
            }
        }
        if (Old_Nullables != NULLABLES_)
        {
            NULLABLES_ = Old_Nullables;
        }
        else
            break;
        
    }

    // perform substitution on the nullable variables
    for (auto var: V_)
    {
        Bodies varset      = P_[var];
        Bodies temp_varset = varset;
        while (!temp_varset.empty())
        {
            auto symlis = *temp_varset.begin();
            temp_varset.erase(temp_varset.begin());
            
            for (auto sym: symlis)
            {
                if (sym.index() == 1)
                    continue;
                
                V_t var = get<0>(sym);
                if (!(is_member(var, NULLABLES_)))
                    continue;
                symlis.remove(var);
                break;
            }
            temp_varset.insert(symlis);
        }
        P_[var] = temp_varset;
    }
    
}

template <typename V_t, typename S_t>
void CFG<V_t, S_t>::remove_unit_productions()
{}

template <typename V_t, typename S_t>
void CFG<V_t, S_t>::remove_useless_variables()
{}

// class Parse_Tree
// {
//     Parse_Tree_Node root_;
// };

// class Parse_Tree_Node
// {
//     Token x;
//     vector<Parse_Tree_Node> children_;
// };

template <typename V_t, typename S_t>
ostream & operator<<(ostream & cout, const variant<V_t, S_t> & v)
{
    const int index = v.index();
    if (index == 0)
        cout << get<0>(v);
    else
        cout << get<1>(v);
    
    return cout;
}

template <typename V_t, typename S_t>
ostream & operator<<(ostream & cout, const CFG<V_t, S_t> & G)
{
    cout << "G:\n";
    cout << "Variables: " << G.V_ << endl;
    cout << "Start Variable: " << G.S_ << endl;
    cout << "Sigma: " << G.sigma_ << endl;
    cout << "Nullables: " << G.NULLABLES_ << endl;
    cout << "Productions: \n";
    for (auto & p: G.P_)
    {
        auto var = p.first;
        auto varset = p.second;
        cout << "\t" << var << " --> ";
        
        
        string delim = "";
        
        for (auto symlis: varset)
        {
            cout << delim;
            
            for (auto sym : symlis)
            {
                cout << sym;
            }
            delim  = " | ";
        }
        
        if (is_member(var, G.NULLABLES_))
            cout << delim << "e";
        cout << endl;
        
    }
    
    return cout;
}    
    
#endif
