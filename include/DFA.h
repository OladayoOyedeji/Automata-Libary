#ifndef DFA_H
#define DFA_H


template <typename S_t, typename Q_t>
struct DFA
{
    DFA(const std::set<S_t>&, const std::set<Q_t>&,
        const std::set<Q_t>&, const Q_t&,
        const std::unordered_map< std::pair<Q_t, S_t>, Q_t>&);

    bool operator()(const std::vector<S_t>&word, bool verbose=false)
    Q_t q0_;
    std::set< Q_t > Q_;
    std::set< S_t > alphabet_;
    std::unordered_map< std::pair<Q_t, S_t>, Q_t> delta_;
    std::set< Q_t > F_;
    
};

#endif
