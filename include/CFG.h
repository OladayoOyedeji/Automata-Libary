#ifndef CFG_H
#define CFG_H

#include "STL_util.h"
#include <variant>

template <typename V_t, typename S_t>
std::ostream & operator<<(ostream & cout, const variant<V_t, S_t> & v);

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
        bool flag = true;
        for (auto & sym: b)
        {
            if (sym.index() == 0)
            {
                V_t var = get<0>(sym);
                V_.insert(var);
                flag = false;
            }
            else
            {
                S_t term = get<1>(sym);
                sigma_.insert(term);
            }
        }

        if (flag) TERMINALS_.insert(v);

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
    Setv  TERMINALS_;
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
bool belongs(list<variant<V_t, S_t>> & varlist, unordered_set<V_t> & s)
{
    for (auto & sym: varlist)
    {
        int index = sym.index();
        if (index == 1)
            return false;
        
        V_t var = get<0>(sym);
        if (!(is_member(var, s)))
            return false;
    }
    return true;
}

template <typename V_t, typename S_t>
ostream & operator<<(ostream & cout, const variant<V_t, S_t> & v)
{
    const int index = v.index();
    // cout << "<" << index << " ";
    if (index == 0)
        cout << get<0>(v);
    else
        cout << get<1>(v);
    
    return cout;
}

template <typename V_t, typename S_t>
void CFG<V_t, S_t>::remove_E_productions()
{
    Setv Old_Nullables = NULLABLES_;

    while (true)
    {
        for (auto var: V_)
        {
            auto p = P_.find(var);
            // cout << var << endl;
            if ( is_member(var, NULLABLES_) || p == P_.end() )
                continue;

            for (auto r: p->second)
            {
                // Setv union_ = unordered_set<V_t>(r.begin(), r.end()) | NULLABLES_;

                if (belongs(r, NULLABLES_))
                {
                    Old_Nullables.insert(var);
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
    // cout << Old_Nullables << ' ' << NULLABLES_ << endl;

    // perform substitution on the nullable variables
    for (auto var: V_)
    {
        Bodies varset      = P_[var];
        Bodies temp_varset = varset;
        while (!temp_varset.empty())
        {
            // cout << temp_varset << endl;
            // auto old_symlis = *temp_varset.begin();
            auto symlis = *temp_varset.begin();;
            temp_varset.erase(temp_varset.begin());

            // cout << symlis << endl;
            auto p = symlis.begin();
            while (p != symlis.end())
            {
                auto sym = *p;
                auto q = p;
                p++;
                
                if (sym.index() == 1)
                    continue;
                
                // cout << "Symbols " << sym << endl;
                V_t var = get<0>(sym);
                
                if (!(is_member(var, NULLABLES_)))
                {
                    // cout << true << endl;
                    continue;
                }
                
                // auto new_symlis = symlis;
                // auto it = find(symlis.rbegin(), symlis.rend(), *q);
                // new_symlis.remove(*q);

                auto new_symlis = list<Symbol>(symlis.begin(), q);
                q++;
                new_symlis.insert(new_symlis.end(), q, symlis.end());

                // cout << "new: [";
                // for (auto x: new_symlis)
                // {
                //     cout << x;
                // }
                // cout << "] old: [";
                // for (auto x: symlis)
                // {
                //     cout << x;
                // }
                // cout << "]" << endl;

                if (!new_symlis.empty())
                {
                    temp_varset.insert(new_symlis);
                    varset.insert(new_symlis);
                    // cout << temp_varset << endl;
                }
            }
        }
        
        P_[var] = varset;
    }

    NULLABLES_ = {S_};
    
}

template <typename V_t, typename S_t>
void CFG<V_t, S_t>::remove_unit_productions()
{
    unordered_map<V_t, unordered_set<V_t>> unit_production;
    unordered_set<V_t> Closed_List;

    unordered_set<list<V_t>> solutions;
    
    stack<pair<V_t,list<V_t> > > fringe;
    fringe.push({S_, {S_}});

    while (!fringe.empty())
    {
        auto top = fringe.top();
        fringe.pop();
        
        V_t varKey = top.first;
        list<V_t> path = top.second;
        
        auto it = P_.find(varKey);
        if (it == P_.end()) continue;

        bool flag = false;
        for (auto symlist: it->second)
        {
            
            if (symlist.size() != 1)
                continue;

            auto p = symlist.begin();
            int index = p->index();
            if (index == 1) continue;

            V_t var = get<0>(*p);
            list<V_t> pathcopy = path;
            
            pathcopy.push_back(var);
            fringe.push({var, pathcopy});
            flag = true;
        }
        if (!flag) solutions.insert(path);
    }
    // cout << "solutions: " << solutions << endl;

    for (auto sol: solutions)
    {
        auto p = sol.begin();
        auto q = p;
        q++;
        while (q != sol.end())
        {
            V_t var = *p;
            // unordered_set<V_t> new_set;
            // p++;
            unit_production[var] |= unordered_set<V_t>(q, sol.end());
            q++;
            p++;
        }
    }
    // cout << unit_production << endl;

    for (auto pairr: unit_production)
    {
        auto sets = pairr.second;
        auto setKey = pairr.first;
        
        // cout << "sets: " << sets << endl;
        for (auto var: sets)
        {
            Bodies b;
            auto it = P_.find(var);
            
            if (it != P_.end())
            {
                // cout <<"what is this: " << it->second << endl;
                for (auto varlis: it->second)
                {
                    if (varlis.size() != 1)
                    {
                        b.insert(varlis);
                        continue;
                    }

                    auto p = *(varlis.begin());
                    int index = p.index();
                    
                    if (index)
                    {
                        b.insert(varlis);
                    }
                }
                P_[setKey] |= b;

                // std::variant<V_t, S_t> v;

                cout << "variant: " <<  typeid(var).name() << endl;
                std::variant<V_t, S_t> v(std::in_place_index<0>, var);
                
                // // v.emplace<0>(var);
                // std::variant<V_t, S_t> v = var;
                auto x = P_[setKey].find({v});
                if (x != P_[setKey].end())
                {
                    P_[setKey].erase(x);
                }
            }
            // cout << setKey << ' ' << b << endl;
        }
    }
    // cout << P_ 

}

template <typename V_t, typename S_t>
void CFG<V_t, S_t>::remove_useless_variables()
{
    // unordered_set<V_t> TERMINALS_;
    // for (auto var: V_)
    // {
    //     auto it = P_.find(var);

    //     if (it == P_.end()) continue;
    
    //     Bodies sentinals = it->second;
    //     for (auto sent: sentinals)
    //     {
    //         bool flag = false;
    //         auto p = sent.begin();
    //         while (p != sent.end())
    //         {
    //             auto sym = *p;
    //             int index = sym.index();
    //             if (index)
    //             {
    //                 TERMINALS_.insert(var);
    //                 flag = true;
    //             }
    //             p++;
    //         }
            
    //         if (flag) break;
    //     }
    // }

    cout << "TERMINALS: " << (TERMINALS_ | NULLABLES_) << endl;

    Setv T = (TERMINALS_ | NULLABLES_);

    while (true)
    {
        Setv X = V_ - T;
        Setv oldT = T;
        for (auto var: X)
        {
            bool flag = false;
            auto it = P_.find(var);
            if (it != P_.end())
            {
                for (auto symlis: it->second)
                {
                    auto p = symlis.begin();

                    flag = true;
                    while (p != symlis.end())
                    {
                        auto sym = *p;
                        int index = sym.index();

                        if (index == 0)
                        {
                            V_t var_ = get<0>(sym);
                    
                            if (!is_member(var_, T))
                            {
                                flag = false;
                                break;
                            }
                        }
                        p++;
                    }
                    if (flag) break;
                }
            }
            if (flag) oldT.insert(var);
        }

        if (T == oldT) break;
        T = oldT;
    }

    cout << T << endl;
    cout << "USELESS_ " << V_ - T << endl;
    Setv U = V_ - T;

    V_ = T;
    for (auto var: V_)
    {
        auto it = P_.find(var);
        if (it == P_.end()) continue;

        for (auto symlis: it->second)
        {
            auto p = symlis.begin();

            while (p != symlis.end())
            {
                auto sym = *p;
                int index = sym.index();

                if (index == 0)
                {
                    V_t var = get<0>(sym);

                    if ()
                }
            }
            
        }
    }
    
}

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
