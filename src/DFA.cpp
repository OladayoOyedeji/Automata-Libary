#include "DFA.h"

DFA::DFA(const std::unordered_set<S_t>& Sigma, const std::unordered_set<Q_t>& States,
        const std::unordered_set<Q_t>& Final, const Q_t& start,
        const std::unordered_map< std::pair<Q_t, S_t>, Q_t>& transitions)
    : q0_(start), Q_(States), alphabet_(Sigma), delta_(transitions), F_(Final)
{}

DFA::DFA(const DFA & M)
    : q0_(M.q0_), Q_(M.Q_), alphabet_(M.alphabet_), delta_(M.delta_), F_(M.F_)
{}

DFA & DFA::compDFA()
{
    F_ = Q_ - F_;
    
    return *this;
}

const DFA & intersection_(const DFA & M)
{
    std::unordered_set<S_t> sigma = M.alphabet_ & alphabet_;
    std::unordered_set< std::pair<Q_t, Q_t> > states;
    std::unordered_set< std::pair<Q_t, Q_t> > final_ = F_ * M.F_;
    std::pair<Q_t, Q_t> start = std::pair<Q_t, Q_t>(q0_, M.q0_);
    std::unordered_map< std::pair< std::pair<Q_t, Q_t> , S_t>,
                        std::pair<Q_t, Q_t>> transitions;

    walk(start, delta_, M.delta_, transitions, states);
    return DFA(sigma, states, final_, start, transitions);
}

const DFA & DFA::union_(const DFA & M) const
{
    std::unordered_set<S_t> sigma = M.alphabet_ & alphabet_;
    std::unordered_set< std::pair<Q_t, Q_t> > states;
    std::unordered_set< std::pair<Q_t, Q_t> > final_ = (F_ * M.Q_) | (M.F_ * Q_);
    std::pair<Q_t, Q_t> start = std::pair<Q_t, Q_t>(q0_, M.q0_);
    std::unordered_map< std::pair< std::pair<Q_t, Q_t> , S_t>,
                        std::pair<Q_t, Q_t>> transitions;

    walk(start, delta_, M.delta_, transitions, states);
    return (sigma, states, final_, start, transitions);
}

template < typename Q_t, typename S_t>
void walk(const std::pair<Q_t, Q_t> & qi,
          const std::unordered_map< std::pair<Q_t, S_t>, Q_t> & delta0,
          const std::unordered_map< std::pair<Q_t, S_t>, Q_t> & delta1,
          std::unordered_map< std::pair< std::pair<Q_t, Q_t> , S_t>,
          std::pair<Q_t, Q_t> > & delta_, const std::unordered_set<S_t> & alphabet,
          const std::unordered_set< std::pair<Q_t, Q_t> > & states)
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
        
        for (auto p: alphabet_)
        {
            delta_[{qi, p}] = qi1;
            
            std::pair<Q_t, Q_t> qi1 = std::pair<Q_t, Q_t>(delta0[{a, p}], delta1[{b, p}]);
            delta_[{qi, p}] = qi1;
            
            walk(qi1, delta0, delta1, delta_, alphabet, states);
        }
    }
}


bool DFA::operator()(const std::vector<S_t>&word, bool verbose)
{
    Q_t q = q0_;
    std::cout << word << std::endl;
    
    for (int i = 0; i < word.size(); ++i)
    {
        q = delta[{q,word[i]}];
        
        if (verbose)
        {
            vector<S_t> v(word.begin() + i, word.end());
            std::cout << {q, v} << std::endl;
        }
    }

    return is_member(q, F_);
}
