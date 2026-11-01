#ifndef DFA_H
#define DFA_H

#include "STL_util.h"

template < typename Q_t, typename S_t >
class DFA;

template < typename Q_t, typename S_t >
void walk(const pair<Q_t, Q_t> & qi,
          const unordered_map< pair<Q_t, S_t>, Q_t> & delta0,
          const unordered_map< pair<Q_t, S_t>, Q_t> & delta1,
          unordered_map< pair< pair<Q_t, Q_t> , S_t>,
          pair<Q_t, Q_t> > & delta_, const unordered_set<S_t> & alphabet,
          unordered_set< pair<Q_t, Q_t> > & states);

template <typename Q_t, typename S_t>
class DFA
{
    using SetUq = unordered_set<Q_t>;
    using SetUs = unordered_set<S_t>;
    using SuperSetUq = unordered_set< SetUq >;
    using PairQS = pair<Q_t, S_t>;
    using PairQQ = pair<Q_t, Q_t>;
    using Mapt = unordered_map<PairQS, Q_t>;
public:
    DFA(const SetUs & sigma, const SetUq & Q,
        const SetUq & F,     const Q_t   & q0,
        const Mapt  & delta);
    DFA(const DFA & M);
    bool operator()(const vector<S_t> & word, bool verbose=false);
    SuperSetUq Moore_partition_(const SuperSetUq & P) const;
    SuperSetUq Moore_partition(const SetUq & S, const SuperSetUq & P);
    DFA<SetUq, S_t> minimizeDFA() const;
    DFA<PairQQ, S_t> intersection_(const DFA & M) const;
    DFA<PairQQ, S_t> union_(       const DFA & M) const;
    DFA & compDFA();
    DFA<int, S_t> baseDFA() const;
    
    Q_t   q0_;
    SetUq Q_;
    SetUs sigma_;
    Mapt  delta_;
    SetUq F_;
};

template <typename Q_t, typename S_t>
DFA<Q_t, S_t>::DFA(const SetUs & sigma, const SetUq & Q,
    const SetUq & F,     const Q_t   & q0,
    const Mapt  & delta)
    : q0_(q0), Q_(Q), sigma_(sigma), delta_(delta), F_(F)
{}

template <typename Q_t, typename S_t>
DFA<Q_t, S_t>::DFA(const DFA & M)
    : q0_(M.q0_), Q_(M.Q_), sigma_(M.sigma_), delta_(M.delta_), F_(M.F_)
{}

template <typename Q_t, typename S_t>
bool DFA<Q_t, S_t>::operator()(const vector<S_t> & word, bool verbose)
{
    Q_t q = q0_;
    std::cout << word << std::endl;
    
    for (int i = 0; i < word.size(); ++i)
    {
        q = delta_[{q,word[i]}];
        
        if (verbose)
        {
            vector<S_t> v(word.begin() + i + 1, word.end());
            std::cout << std::pair<Q_t, std::vector<S_t>>(q, v) << " " << word[i] << std::endl;
        }
    }

    return is_member(q, F_);
}

template <typename Q_t, typename S_t>
typename DFA<Q_t, S_t>::SuperSetUq
DFA<Q_t, S_t>::Moore_partition_(const SuperSetUq & P) const
{
    SuperSetUq next;
    for (auto & block: P)
    {
        SuperSetUq peices = partition(block, P);
        next.insert(peices);
    }
    return next;
}

template <typename Q_t, typename S_t>
typename DFA<Q_t, S_t>::SuperSetUq
DFA<Q_t, S_t>::Moore_partition(const SetUq & S, const SuperSetUq & P)
{
    vector<SetUq> vecP(P.begin(), P.end());
    int i = 0;

    unordered_map< unordered_set<int>, SetUq> P_k;

    for (auto q: S)
    {
        unordered_set<int> index;

        for (auto a: sigma_)
        {
            Q_t qf = delta_[{q, a}];
            for (int i = 0; i < vecP.size(); ++i)
            {
                if (is_member(qf, vecP[i]))
                {
                    index.insert(i);
                    break;
                }
            }
        }
        P_k[index].insert(q);
    }

    SuperSetUq result;
    for (auto & kv: P_k)
    {
        result.insert(kv.second);
    }

    return result;
}

template <typename Q_t, typename S_t>
DFA<typename DFA<Q_t, S_t>::SetUq, S_t> DFA<Q_t, S_t>::minimizeDFA() const
{
    SuperSetUq P;
    P.insert(Q_ - F_); P.insert(F_);

    while (true)
    {
        SuperSetUq P_t = Moore_partition(P);

        if (P_t == P)
            break;

        P = P_t;
    }

    SuperSetUq Q = P;
    SetUq q0;

    // return DFA< SetUq, S_t>(sigma_, Q, F, q0, delta);
}

template <typename Q_t, typename S_t>
DFA<typename DFA<Q_t, S_t>::PairQQ, S_t>
DFA<Q_t, S_t>::intersection_(const DFA & M) const
{
    SetUs       sigma = sigma_ & M.sigma_;
    unordered_set<PairQQ> states;
    unordered_set<PairQQ> final_ = F_ * M.F_;
    PairQQ      start = PairQQ(q0_, M.q0_);
    unordered_map< pair< PairQQ, S_t >, PairQQ > delta;

    walk<Q_t, S_t>(start, delta_, M.delta_, delta, sigma, states);

    return DFA< PairQQ, S_t >(sigma, states, final_, start, delta);
}

template <typename Q_t, typename S_t>
DFA<typename DFA<Q_t, S_t>::PairQQ, S_t>
DFA<Q_t, S_t>::union_(const DFA & M) const
{
    SetUs       sigma = sigma_ & M.sigma_;
    unordered_set<PairQQ> states;
    unordered_set<PairQQ> final_ = (F_ * M.Q_) | (M.F_ * Q_);
    PairQQ      start = PairQQ(q0_, M.q0_);
    unordered_map< pair< PairQQ, S_t >, PairQQ > delta;

    walk<Q_t, S_t>(start, delta_, M.delta_, delta, sigma, states);

    return DFA< PairQQ, S_t >(sigma, states, final_, start, delta);
} 

template <typename Q_t, typename S_t>
DFA<Q_t, S_t> & DFA<Q_t, S_t>::compDFA()
{
    F_ = Q_ - F_;

    return *this;
}

template <typename Q_t, typename S_t>
DFA<int, S_t> baseDFA(int start) const
{
    unordered_map<Q_t, int> mapper;
    SetUi Q;
    Q.insert(start);
    mapper[q0_] = start;

    SetUq Q_i = Q_;
    Q_i.erase(q0_);
    
    SetUi F;
    if (is_member(q0_, F_))
        F.insert(start);

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
    unordered_map< pair<int, S_t>, int> delta;

    for (auto q: Q_)
    {
        int key = mapper[q];
        for (auto a: sigma_)
        {
            auto it = delta_.find({q, a});
            if (it != delta_.end())
            {
                auto p = it->second;
                int value = mapper[p];
                delta[{key, a}].insert(value);
            }
        }
    }

    return DFA<int, S_t>(sigma_, Q, F, start, delta);
}

template < typename Q_t, typename S_t>
void walk(const pair<Q_t, Q_t> & qi,
          const unordered_map< pair<Q_t, S_t>, Q_t> & delta0,
          const unordered_map< pair<Q_t, S_t>, Q_t> & delta1,
          unordered_map< pair< pair<Q_t, Q_t> , S_t>,
          pair<Q_t, Q_t> > & delta_, const unordered_set<S_t> & sigma,
          unordered_set< pair<Q_t, Q_t> > & states)
{
    if (is_member(qi, states))
    {
        return;
    }
    else
    {
        states.insert(qi);
        
        Q_t a = qi.first;
        Q_t b = qi.second;
        
        for (auto p: sigma)
        {
            pair<Q_t, S_t> key0 =  pair<Q_t, S_t>(a, p);
            pair<Q_t, S_t> key1 =  pair<Q_t, S_t>(b, p);

            Q_t value = delta0.at(key0);
            
            pair<Q_t, Q_t> qi1 = pair<Q_t, Q_t>(delta0.at(key0), delta1.at(key1)); 
            delta_[{qi, p}] = qi1;
            
            walk(qi1, delta0, delta1, delta_, sigma, states);
        }
    }
}

template <typename S_t, typename Q_t >
ostream & operator<<(ostream & cout, const DFA< S_t, Q_t > & M)
{
    cout << "M:" << endl;
    cout << "\tSigma: " << M.sigma_ << endl;
    cout << "\tStart State: " << M.q0_ << endl;
    cout << "\tSet of States: " << M.Q_ << endl;
    cout << "\tTransitions: " << M.delta_ << endl;
    cout << "\tFinal States: " << M.F_ << endl;

    return cout;
}

#endif
