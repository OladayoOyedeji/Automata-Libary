#ifndef REGULAR_LANGUAGE_H
#define REGULAR_LANGUAGE_H

#include "DFA.h"
#include "NFA.h"


template <typename Q_t, typename S_t>
std::set<Q_t> epsilon_closure(const std::set<Q_t>& start) const
{
    std::set<Q_t> done;
    std::set<Q_t> boundary = start;

    while (!boundary.empty())
    {
        std::set<Q_t> next;

        for (const auto& q : boundary)
        {
            done.insert(q);

            auto it = delta.find({q, ""});
            if (it == delta.end())
                continue;

            for (const auto& p : it->second)
            {
                if (done.count(p) == 0 && boundary.count(p) == 0)
                    next.insert(p);
            }
        }

        boundary = std::move(next);
    }

    return done;
}

template <typename Q_t, typename S_t>
std::set<Q_t> epsilon_closure(const Q_t& q) const
{
    return epsilon_closure(std::set<Q_t>{q});
}

template <typename Q_t, typename S_t>
DFA<std::set<Q_t>> powerset_construction() const
{
    using S = std::set<Q_t>;

    const S start = epsilon_closure(q0_);

    std::set<S> states;
    std::set<S> accepting;
    std::map<std::pair<S, std::string>, S> transitions;

    std::stack<S> fringe;
    fringe.push(start);
    states.insert(start);

    while (!fringe.empty())
    {
        S top = fringe.top();
        fringe.pop();

        // accepting if it contains any NFA accepting state
        for (const auto& q : top)
        {
            if (F_.count(q))
            {
                accepting.insert(top);
                break;
            }
        }

        for (const auto& a : alphabet_)
        {
            S moved;
            for (const auto& q : top)
            {
                auto it = delta.find({q, a});
                if (it != delta.end())
                    moved.insert(it->second.begin(), it->second.end());
            }

            S next = epsilon_closure(moved);
            transitions[{top, a}] = next;

            if (states.insert(next).second)   // true if newly discovered
                fringe.push(next);
        }
    }

    return DFA<S>(states, alphabet_, transitions, start, accepting);
}


#endif
