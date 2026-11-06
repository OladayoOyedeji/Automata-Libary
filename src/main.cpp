#include "DFA.h"
#include "NFA.h"
#include "CFG.h"
#include "Regex.h"

// class Var: public string
// {};

// class Term: public string
// {};

// ostream & operator<<(ostream & cout, const Var & v)
// {
//     cout << v
    
//     return cout;
// }

int main()
{
    typedef string Q_t;
    typedef string S_t;
    typedef pair< Q_t, S_t > Q_t_S_t;
    typedef unordered_map< Q_t_S_t, Q_t > D_t;
    typedef unordered_map< Q_t_S_t, unordered_set<Q_t> > DN_t;
    typedef unordered_map<Q_t, unordered_set<Q_t>> ET_t;
    
   //  // Basic construction
//     cout << RegEx("") << endl;            // empty string (epsilon): edge case
//     cout << RegEx("z") << endl;
//     cout << RegEx("abcdef") << endl;

// // Union
//     cout << (RegEx("a") | RegEx("a")) << endl;          // same operand twice
//     cout << (RegEx("a") | RegEx("b") | RegEx("c")) << endl;   // chained
//     cout << (RegEx("ab") | RegEx("")) << endl;          // union with epsilon

// // Concatenation
//     cout << (RegEx("a") & RegEx("b")) << endl;
//     cout << (RegEx("ab") & RegEx("cd")) << endl;
//     cout << (RegEx("") & RegEx("a")) << endl;           // epsilon is identity
//     cout << (RegEx("a") & RegEx("")) << endl;

// // Star
//     cout << *RegEx("a") << endl;
//     cout << *RegEx("") << endl;                         // epsilon* is epsilon
//     cout << *(*RegEx("a")) << endl;                     // star of star
//     cout << *(RegEx("a") | RegEx("b")) << endl;         // (a|b)*

// // Plus
//     cout << +RegEx("a") << endl;
//     cout << +(RegEx("a") | RegEx("b")) << endl;         // (a|b)+
//     cout << +(*RegEx("a")) << endl;                     // (a*)+

// // Precedence and nesting
//     cout << ((RegEx("a") | RegEx("b")) & RegEx("c")) << endl;   // (a|b)c
    
//     RegEx r = ((RegEx("a") | RegEx("b")) & RegEx("c"));
//     cout << r << endl;
//     // cout << r.N_.epsilon_closure({0}) << endl;
//     cout << r.powerset_construction() << endl;
//     // cout << (RegEx("a") | (RegEx("b") & RegEx("c"))) << endl;   // a|bc
//     // cout << *(RegEx("ab") | RegEx("cd")) << endl;
//     // cout << (*RegEx("a") & *RegEx("b")) << endl;                // a*b*
//    //  CFG<string, string> G("S");
// //     cout << G << endl;
    typedef variant<string, string> Symbol;
    typedef list<Symbol>            Body;

//     std::variant<string, string> x;
//     std::variant<string, string> y;
//     x.emplace<0>("A");
    
//     y.emplace<0>("C");

    
//     Body body = {x, y, x};
//     G.add_production("S", body);
    
//     x.emplace<1>("a");
//     y.emplace<0>("A");
//     body = {x, y, x};
//     G.add_production("A", body);

//     y.emplace<0>("B");
//     body = {y};
//     G.add_production("A", body);

//     x.emplace<0>("C");
//     body = {x};
//     G.add_production("A", body);

//     x.emplace<1>("b");
//     y.emplace<0>("B");
//     body = {x, y};
//     G.add_production("B", body);
    
//     body = {x};
//     G.add_production("B", body);

//     x.emplace<1>("c");
//     y.emplace<0>("C");
//     body = {x, y};
//     G.add_production("C", body);

//     G.add_e_transition("C");
    
//     cout << G << endl;
    
//     // G.remove_E_productions();
//     // cout << G << endl;
//     // G.remove_unit_productions();
// // cout << G << endl;

//     G.Normalize();
//     cout << G << endl;

    CFG<string, string> G("S");
    
    std::variant<string, string> x;
    std::variant<string, string> y;
    x.emplace<1>("a");
    y.emplace<0>("A");
    Body body = {x, y};

    G.add_production("S",body);
    
    x.emplace<0>("C");
    y.emplace<0>("B");
    body = {y, x};
    G.add_production("S",body);

    x.emplace<1>("b");
    body = {x};
    G.add_production("A", body);

    body = {x, y};
    G.add_production("A", body);

    y.emplace<1>("a");
    x.emplace<0>("B");
    body = {y, x};

    G.add_production("B", body);
    G.add_e_transition("B");

    x.emplace<0>("C");
    body = {y, x};
    
    G.add_production("C", body);
    x.emplace<0>("D");
    body = {x};
    
    G.add_production("C", body);

    y.emplace<1>("b");
    body = {y,x};
    G.add_production("D", body);
    
    cout << G << endl;

    G.remove_useless_variables();
    
    return 0;
}
