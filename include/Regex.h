#ifndef REGEX_H
#define REGEX_H

#include "Regular_Language.h"

class RegEx
{
public:
    RegEx(string & x)
    {
        NFA<int, char> N;
        for (int i = 0; i < x.size(); ++i)
        {
            N.insert(x[i]);
        }
    }
    RegEx(const RegEx & r)
        : N_(r.N_)
    {}
    // union
    RegEx & operator|=(const RegEx & r)
    {
        N_ = N_.union_(r.N);
        return *this;
    }
    const RegEx & operator|(const RegEx & r) const
    {
        RegEx R(*this);
        R |= r:
        return R;
    }
    // klene star and klene star - {e}
    RegEx & operator+ ()
    {
        N_.KleneStar();
        N_.F_.erase(N_.q0_);

        return *this;
    }
    RegEx & operator* ()
    {
        N_.KleneStar();
        return *this;
    }
    // concatenation
    RegEx & operator-=(const RegEx & r)
    {
        N_.concatenate(r.N_);
        return *this;
    }

    RegEx operator-(const RegEx & r)
    {
        RegEx R(*this);
        R -= r;
        return R;
    }
    
    NFA<int, char> N_;
    
};


#endif
