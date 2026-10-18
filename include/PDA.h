#ifndef PDA_H
#define PDA_H

class PDA: public CFG
{
    bool operator()(const std::vector<S_t>&);
    CYK();
    SRP();
};

#endif
