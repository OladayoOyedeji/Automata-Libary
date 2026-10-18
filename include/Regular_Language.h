#ifndef REGULAR_LANGUAGE_H
#define REGULAR_LANGUAGE_H

class RL
{
    
    DFA compDFA(const DFA&);
    DFA intersection_(const DFA&, const DFA&) const;
    DFA union_(const DFA&, const DFA&) const;
    DFA concatenation(const DFA&, const DFA&) const;
    DFA KleneStar(const DFA&) const;

    NFA to(const DFA&);
    DFA to(const NFA&);
    DFA mini(const DFA&);
    
};

#endif
