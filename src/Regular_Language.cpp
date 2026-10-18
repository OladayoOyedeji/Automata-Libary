#include "Regular_Language.h"

class epsilon: public S_t
{};

DFA<S_t, T_t> RL::compDFA(const DFA<S_t, T_t> & M)
{
    std::set< Q_t > F;
    for (auto a: M.Q_)
    {
        if (M.F_.find(a) == M.F_.end())
        {
            F.insert(a);
        }
    }
    M.F_ = F;
    
    return M;
}

DFA<S_t, T_t> RL::intersection_(const DFA<S_t, T_t> & M0,
                                const DFA<S_t, T_t> & M1) const
{
    
}

NFA<S_t, T_t> RL::union_(const NFA<S_t, T_t> & N0,
                         const NFA<S_t, T_t> & N1) const
{
    // add
    Q_t q0 = q;

    std::unordered_map< std::pair<Q_t, S_t>, Q_t> d;

    std::set<S_t> Q(N0.Q_);
    Q.union(N1.Q_);
    Q.insert(q);
    
    for (auto a: q0.Q_)
    {
        d[{q, epsilon}] = a;
    }
    
}

NFA<S_t, T_t> RL::concatenation(const NFA<S_t, T_t> & N0,
                                const NFA & N1) const
{
    for (auto a: N0.F_)
    {
        d[{a, epsilon}] = N1.q0_;
    }
}

NFA<S_t, T_t> RL::KleneStar(const NFA<S_t, T_t> & N) const
{
    Q_t q0;

    d[{q0, epsilon}] = N.q0;

    for (auto a: N.F_)
    {
        d[{a, epsilon}] = q0;
    }

    std::set< Q_t > s(N.F_);
    s.insert(q0);
}

NFA<S_t, T_t> RL::to(const DFA<S_t, T_t> &);
DFA<S_t, T_t> RL::to(const NFA<S_t, T_t> &);
DFA<S_t, T_t> RL::mini(const DFA<S_t, T_t> &);
