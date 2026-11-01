#include "DFA.h"
#include "NFA.h"
#include "Regex.h"

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
    
    RegEx r = ((RegEx("a") | RegEx("b")) & RegEx("c"));
    cout << r << endl;
    // cout << r.N_.epsilon_closure({0}) << endl;
    cout << r.powerset_construction() << endl;
    // cout << (RegEx("a") | (RegEx("b") & RegEx("c"))) << endl;   // a|bc
    // cout << *(RegEx("ab") | RegEx("cd")) << endl;
    // cout << (*RegEx("a") & *RegEx("b")) << endl;                // a*b*
    
    return 0;
}
