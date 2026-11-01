#ifndef REGEX_H
#define REGEX_H

#include "NFA.h"

typedef unordered_map<int, unordered_set<int>> Mapi;
typedef unordered_map<pair<int,char>, unordered_set<int>> Mapc;
typedef DFA<unordered_set<int>, char> DFAic;
typedef DFA<int, char> DFAi;
typedef unordered_set<int> Seti;
typedef unordered_set< Seti > SuperSeti;


class RegEx
{
public:
    RegEx(char c)
        : N_({c}, {0, 1}, {1}, 0,
             Mapc{ { {0, c}, {1} } },   // 0 --c--> 1
             Mapi{}), regex_()
    {
        // cout << N_ << endl;
        regex_ += c;
    }
    RegEx(const string & x)
    {
        if (x.size() == 1)
        {
            *this = RegEx(x[0]);
            return;
        }
        cout << N_ << endl;
        for (int i = 0; i < x.size(); ++i)
        {
            *this &= RegEx(x[i]);
        }
    }
    RegEx(const NFA<int, char> & N)
        : N_(N)
    {}
    RegEx(const RegEx & r)
        : N_(r.N_)
    {}
    RegEx & operator=(const RegEx & r)
    {
        N_ = r.N_;
        regex_ = r.regex_;

        return *this;
    }
    // union
    RegEx & operator|=(const RegEx & r)
    {
        N_ = N_.union_(r.N_);
        return *this;
    }
    const RegEx operator|(const RegEx & r) const
    {
        RegEx R(*this);
        R |= r;
        return R;
    }
    // klene star and klene star - {e}
    RegEx operator+() const
    {
        NFA<int, char> N = N_.KleneStar();
        N.F_.erase(N_.q0_);

        return RegEx(N);
    }
    RegEx operator*() const
    {
        return RegEx(N_.KleneStar());
    }
    // concatenation
    RegEx & operator&=(const RegEx & r)
    {
        N_ = N_.concatenate(r.N_);
        regex_ += r.regex_;
        return *this;
    }

    RegEx operator&(const RegEx & r) const
    {
        RegEx R(*this);
        R &= r;
        return R;
    }

    DFAic powerset_construction() const
    {
        Seti q0 = N_.epsilon_closure({N_.q0_});
        int i = 0;
        cout << i << " : " << q0 << endl;
        // int x;
        // cin >> x;
        SuperSeti Q;
        SuperSeti F;
        unordered_map<pair<Seti, char>, Seti> delta;

        stack<Seti> fringe;
        fringe.push(q0);

        while (!fringe.empty())
        {
            Seti top = fringe.top();
            fringe.pop();
            Q.insert(top);
            
            Seti intersect = top & N_.F_;
            F.insert(intersect);

            for (auto a: N_.sigma_)
            {
                Seti next;
                for (auto q: top)
                {
                    auto it = N_.delta_.find({q, a});
                    if (it != N_.delta_.end())
                        next |= it->second;
                }

                next = N_.epsilon_closure(next);
                cout << i << " : " << next << endl;
                // int x;
                // cin >> x;
                delta[{top, a}] = next;

                if (!is_member(next, Q))
                {
                    fringe.push(next);
                }
            }
        }

        DFAic M(N_.sigma_, Q, F, q0, delta);

        // cout << M.minimizeDFA() << endl;
        
        return M;
    }
    
    NFA<int, char> N_;
    string regex_;
    
};


ostream & operator<<(ostream & cout, const RegEx & r)
{
    cout << "Regular expression: " << r.regex_ << endl;
    cout << r.N_;
    return cout;
}

#endif
