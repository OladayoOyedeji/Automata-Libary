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
 * @class NFA
 * @brief Nondeterministic Finite-State Machine
 *
 * Represents a nondeterministic finite-state machine parameterized
 * by its state type and input symbol type.
 *
 * @tparam Q_t State type.
 * @tparam S_t Input symbol type.
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
    using Map = unordered_map<Q_t, SetUq>;
    using MapUi = unordered_map<int, SetUi>;
public:
 /**
 * @brief An NFA contains the alphabets, the set of states, the Final states which is
 a subset of the set of states, the start state, the transition function and the epsilon
  transition function.
 */
    NFA(const SetUs & sigma, const SetUq & Q,
        const SetUq & F,     const Q_t   & q0,
        const Mapt  & delta, const Map   & epDt);
    NFA(const NFA & N);
    NFA()
        : q0_(0), Q_({0}), sigma_({}), F_({0})
    {}

    NFA operator=(const NFA & N)
    {
        sigma_ = N.sigma_;
        Q_     = N.Q_;
        F_     = N.F_;
        q0_    = N.q0_;
        delta_ = N.delta_;
        epDt_  = N.epDt_;

        return *this;
    }
    bool operator()(const vector<S_t> & word, bool verbose=false);
    NFA<int, S_t> union_(const NFA & N) const;
    NFA<int, S_t> concatenate(const NFA & N) const;
    NFA<int, S_t> KleneStar() const;
    NFA<int, S_t> baseNFA(int start=0) const;
    SetUq         epsilon_closure(const SetUq & S) const;
    void insert(const S_t & a)
    {
        int size = Q_.size();
        if (size == 0)
        {
            q0_ = size;
        }
        Q_.insert(q0_);
        
    }
    
    int size() const
    {
        return Q_.size();
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
                   const Mapt  & delta, const Map   & epDt)
    : q0_(q0), Q_(Q), sigma_(sigma), delta_(delta), F_(F), epDt_(epDt)
{}

template <typename Q_t, typename S_t>
NFA<Q_t, S_t>::NFA(const NFA & N)
    : q0_(N.q0_), Q_(N.Q_), sigma_(N.sigma_), delta_(N.delta_), F_(N.F_), epDt_(N.epDt_)
{}

/**
 * @brief I implemented the word processing with a depth first graph search, i dont think i used the
 processing because the word processing is much slower than the dfa search.
 * @name Word Processor
 */
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

/**
 * @brief The union is connecting both NFA's to a start state with a epsilon
 * @name Union Operation
 */
template <typename Q_t, typename S_t>
NFA<int, S_t> NFA<Q_t, S_t>::union_(const NFA & N) const
{
    NFA<int, S_t> N0 =   baseNFA(1);
    int size_ = size();
    NFA<int, S_t> N1 = N.baseNFA(size_ + 1);
    cout << N0 << endl;
    cout << N1 << endl;
    int q0 = 0;
    SetUi Q = N0.Q_ | N1.Q_;
    Q.insert(q0);
    
    SetUs sigma = N0.sigma_ | N1.sigma_;
    Mapi delta = N0.delta_;
    delta.merge(N1.delta_);

    MapUi epDt = N0.epDt_;
    epDt.merge(N1.epDt_);
    
    SetUi F = N0.F_ | N1.F_;

    // cout << size_ << ' ' << N1.q0_ << endl;
    // cout << N1 << endl;

    epDt[q0] = {N0.q0_, N1.q0_};
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
            // cout << delta[{q0, a}] << ' ' << it->second<< endl;
            delta[{q0, a}] = delta[{q0, a}] | it->second;
        }
    }

    return NFA<int, S_t>(sigma, Q, F, q0, delta, epDt);
}

/**
 * @brief Concatenation is the connection of the final states of the first NFA to the
 start states of the second NFA with epsilons connections, there is an extra state that
 both are connected to
 * @name Concatenate Operation
 */
template <typename Q_t, typename S_t>
NFA<int, S_t> NFA<Q_t, S_t>::concatenate(const NFA & N) const
{
    NFA<int, S_t> N0 =   baseNFA(0);
    NFA<int, S_t> N1 = N.baseNFA(size());

    cout << "N0: " << N0 << endl;
    cout << "N1: " << N1 << endl;
    
    SetUi Q = N0.Q_ | N1.Q_;
    
    SetUs sigma = N0.sigma_ | N1.sigma_;
    // cout << sigma << endl;
    Mapi delta = N0.delta_;
    delta.merge(N1.delta_);
    
    MapUi epDt = N0.epDt_;
    epDt.merge(N1.epDt_);
    
    SetUi F = N1.F_;

    for (auto q: N0.F_)
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
        epDt[q].insert(N1.q0_);
    }

    return NFA<int, S_t>(sigma, Q, F, N0.q0_, delta, epDt);
}

/**
 * @brief I implemented a KleneStar by connecting the final states to the start state and
 making the start state a final state.
 * @name Klene Star Operation
 */
template <typename Q_t, typename S_t>
NFA<int, S_t> NFA<Q_t, S_t>::KleneStar() const
{
    NFA<int, S_t> N =   baseNFA(1);
    cout << N << endl;
    int q0 = 0;
    SetUi Q = N.Q_;
    Q.insert(q0);
    
    SetUs sigma = N.sigma_;
    Mapi delta = N.delta_;

    MapUi epDt = N.epDt_;
    epDt[q0] = {N.q0_};
    
    SetUi F = N.F_;

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
        epDt[q].insert(q0);
    }
    cout << F << ' ' << q0 << endl;
    F.insert(q0);

    return NFA<int, S_t>(sigma, Q, F, q0, delta, epDt);
}

/**
 * @brief For most of the application of the NFA i needed to represent the states as integers
 to make it easier to add and to do operations, so this method is to convert the complex representation
 of states to an integer state with a starting point
 * @name Base NFA
 */
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

    Mapi delta;
    unordered_map<int, unordered_set<int>> epDt;

    for (auto q: Q_)
    {
        int key = mapper[q];
        for (auto a: sigma_)
        {
            
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
        auto it = epDt_.find(q);
        if (it != epDt_.end())
        {
            for (auto p: it->second)
            {
                int value = mapper[p];
                epDt[key].insert(value);
            }
        }
    }

    return NFA<int, S_t>(sigma_, Q, F, start, delta, epDt);
}

/**
 * @brief epsilon closure is to compute all the states that a set of states can reach with
 epsilon transitions
 * @name Epsilon Closure
 */
template <typename Q_t, typename S_t >
typename NFA<Q_t, S_t>::SetUq
NFA<Q_t, S_t>::epsilon_closure(const SetUq & S) const
{
    SetUq DONE;
    SetUq BOUNDARY = S;
    SetUq NEW;

    while (true)
    {
        cout << "oh no" << endl;
        for (auto q: BOUNDARY)
        {
            auto it = epDt_.find(q);
            if (it != epDt_.end())
            {
                SetUq s = it->second;
                SetUq n = DONE | BOUNDARY;
                NEW = s - n;
            }
        }
        DONE |= BOUNDARY;

        if (NEW.empty())
            break;
        else
        {
            BOUNDARY |= NEW;
        }
    }

    return DONE;
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
    cout << "\tEpsilon transition: " << N.epDt_ << endl;

    return cout;
}

#endif
