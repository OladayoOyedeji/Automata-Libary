/**
 * @file NFA.h
 * @author Oladayo Oyedeji
 * @date 10/29/2026
 * @brief NFA implementation
 *
 * @details NFA is different from a DFA in the sense that
 the transition is not deterministics hence the name
 */
#ifndef NFA_H
#define NFA_H

#include "STL_util.h"
/**
 * @brief Q_t is the type for the for states and the S_t
 is the type for transitions
 */

template <typename Q_t, typename S_t>
class NFA
{
/**
 * @brief i cant use a typedef for templates so i am using "using"
 */
    using SetUq = unordered_set<Q_t>;
    using SetUs = unordered_set<S_t>;
    using SuperSetUq = unordered_set< SetUq >;
    using PairQS = pair<Q_t, S_t>;
    using PairQQ = pair<Q_t, Q_t>;
    using PairS = pair<int, S_t>;
    using Mapt = unordered_map<PairQS, SetUq>;
    using SetUi = unordered_set<int>;
    using Mapi = unordered_map<PairS, SetUi>;
    using Map = unordered_map<Q_t, Q_t>;
public:
    NFA(const SetUs & sigma, const SetUq & Q,
        const SetUq & F,     const Q_t   & q0,
        const Mapt  & delta, const Map   & epDt);
    NFA(const NFA & N);

    bool operator()(const vector<S_t> & word, bool verbose=false);
    NFA<int, S_t> union_(const NFA & N) const;
    NFA<int, S_t> concatenate(const NFA & N) const;
    NFA<int, S_t> KleneStar() const;
    NFA<int, S_t> baseNFA(int start=0) const;
    SetUq         epsilon_closure() const;
    void insert(const S_t & a)
    {
        int size = Q_.size();
    }
    
    int size() const
    {
        return q0_.size();
    }
    
    Q_t   q0_;
    SetUq Q_;
    SetUs sigma_;
    Mapt  delta_;
    SetUq F_;
    Map   epDt_;
    
};

template <typename Q_t, typename S_t>
NFA<Q_t, S_t>::NFA(const SetUs & sigma, const SetUq & Q,
    const SetUq & F,     const Q_t   & q0,
    const Mapt  & delta)
    : q0_(q0), Q_(Q), sigma_(sigma), delta_(delta), F_(F), epDt_(epDt)
{}

template <typename Q_t, typename S_t>
NFA<Q_t, S_t>::NFA(const NFA & N)
    : q0_(N.q0_), Q_(N.Q_), sigma_(N.sigma_), delta_(N.delta_), F_(N.F_), epDt_(epDt)
{}

template <typename Q_t, typename S_t>
bool NFA<Q_t, S_t>::operator()(const vector<S_t> & word, bool verbose)
{
    queue< pair<Q_t, int > > fringe;
    fringe.push({q0_, 0});
    int size = word.size();

    SetUq closed_list;
    int i = 0;
    while (fringe.empty())
    {
        pair<Q_t, int> top = fringe.pop();
        Q_t q = top.first;
        int depth = top.second;

        closed_list.insert(q);

        if (depth >= size)
        {
            if (is_member(q, F_))
                return true;
            continue;
        }
        auto it = delta_.find({q, word[i]});

        if (it != delta_.end())
        {
            SetUq children_ = it->second;
            for (auto qi: children_)
            {
                if (!is_member(qi, closed_list))
                {
                    fringe.put({qi, depth+1});
                }
            }
        }
        else
        {
            auto it = epDt_.find(q);
            for (auto p: it->second)
            {
                if (it != delta_.end())
                {
                    if (!is_member(p, closed_list))
                        fringe.put({p, depth});
                }
            }
        }
    }
    return true;
}

template <typename Q_t, typename S_t>
NFA<int, S_t> NFA<Q_t, S_t>::union_(const NFA & N) const
{
    NFA<int, S_t> N0 =   baseNFA(1);
    int size_ = size();
    NFA<int, S_t> N1 = N.baseNFA(size_);
    
    int q0 = 0;
    SetUi Q = N0.Q_ | N1.Q_;
    Q.insert(q0);
    
    SetUs sigma = N0.sigma_ | N1.sigma_;
    Mapi delta = N0.delta_;
    delta.merge(N1.delta_);
    SetUi F = N0.F_ | N1.F_;

    cout << size_ << ' ' << N1.q0_ << endl;
    cout << N1 << endl;
    for (auto a: sigma)
    {
        // delta[{q0, a}] |= delta_[{  q0_, a}] | N.delta_[{N.q0_, a}];
        auto it = N0.delta_.find({N0.q0_, a});
        if (it != N0.delta_.end())
        {
             delta[{q0, a}] = delta[{q0, a}] | it->second;
        }
        it = N1.delta_.find({N1.q0_, a});
        //cout << it->second << endl;
        if (it != N1.delta_.end())
        {
            cout << delta[{q0, a}] << ' ' << it->second<< endl;
            delta[{q0, a}] = delta[{q0, a}] | it->second;
        }
    }

    return NFA<int, S_t>(sigma, Q, F, q0, delta);
}

template <typename Q_t, typename S_t>
NFA<int, S_t> NFA<Q_t, S_t>::concatenate(const NFA & N) const
{
    NFA<int, S_t> N0 =   baseNFA(0);
    NFA<int, S_t> N1 = N.baseNFA(size());
    
    int q0 = 0;
    SetUi Q = N0.Q_ | N1.Q_;
    
    SetUs sigma = sigma_;
    Mapi delta = N0.delta_;
    delta.merge(N1.delta_);
    SetUi F = N1.F_;

    for (auto q: N1.F_)
    {
        for (auto a: sigma)
        {
            // delta[{q,  a}] = delta[{q,a}] | N.delta_[{N.q0_, a}];
            auto it = N1.delta_.find({N1.q0_, a});

            if (it != N1.delta_.end())
            {
                delta[{q, a}] |= it->second;
            }
        }
    }

    return NFA<int, S_t>(sigma, Q, F, q0, delta);
}

template <typename Q_t, typename S_t>
NFA<int, S_t> NFA<Q_t, S_t>::KleneStar() const
{
    NFA<int, S_t> N =   baseNFA(1);
    
    int q0 = 0;
    SetUi Q = N.Q_;
    Q.insert(q0);
    
    SetUs sigma = N.sigma_;
    Mapi delta = N.delta_;
    SetUi F = N.F_;
    
    F.insert(q0);

    for (auto q: F)
    {
        for (auto a: sigma)
        {
            // delta[{q, a}] = delta[{q, a}] | delta_[{q0_, a}];
            auto it = N.delta_.find({N.q0_, a});

            if (it != N.delta_.end())
            {
                delta[{q, a}] |= it->second;
            }
        }
    }

    return NFA<int, S_t>(sigma, Q, F, q0, delta);
}

template <typename Q_t, typename S_t>
NFA<int, S_t> NFA<Q_t, S_t>::baseNFA(int start) const
{
    unordered_map<Q_t, int> mapper;
    unordered_set<int> Q;
    Q.insert(start);
    mapper[q0_] = start;
    
    SetUq Q_i = Q_;
    Q_i.erase(q0_);
    unordered_set<int> F;
    int i = start + 1;
    for (auto p: Q_i)
    {
        mapper[p] = i;
        Q.insert(i);
        if (is_member(p, F_))
        {
            F.insert(i);
        }
        i++;
    }

    Mapi delta;

    for (auto q: Q_)
    {
        for (auto a: sigma_)
        {
            int key = mapper[q];
            auto it = delta_.find({q, a});
            if (it != delta_.end())
            {
                for (auto p: it->second)
                {
                    int value = mapper[p];
                    delta[{key,a}].insert(value);
                }
            }
        }
    }

    return NFA<int, S_t>(sigma_, Q, F, start, delta);
}
template <typename Q_t, typename S_t >
NFA<int, S_t>::SetUq
NFA<int, S_t>::epsilon_closure() const
{
}

template <typename Q_t, typename S_t >
ostream & operator<<(ostream & cout, const NFA< Q_t, S_t > & N)
{
    cout << "N:" << endl;
    cout << "\tSigma: " << N.sigma_ << endl;
    cout << "\tStart State: " << N.q0_ << endl;
    cout << "\tSet of States: " << N.Q_ << endl;
    cout << "\tTransitions: " << N.delta_ << endl;
    cout << "\tFinal States: " << N.F_ << endl;

    return cout;
}

#endif
