#ifndef CFG_H
#define CFG_H

class CFG
{
    std::set<S_t> V_;
    std::set<S_t> sigma_;
    std::unordered_map<V_t, S_t> P_;
    S_t S_;
};

#endif
