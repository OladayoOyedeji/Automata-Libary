#ifndef STL_UTIL_H
#define STL_UTIL_H

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
#include <variant>

using namespace std;

//=============================================================================
// Prototypes
//
// * std::string to_string(T) where T = int, double, char, bool,
// const std::string &
// const std::pair< X, Y > &
// const std::vector< X > &
// const std::list< X > & ... TODO
// const std::unordered_map< X, Y > &
// const std::unordered_set< X > &
//
// * std::hash< T > where T = int, double, char, bool,
// const std::string &
// const std::pair< X, Y > &
// const std::vector< X > &
// const std::list< X > & ... TODO
// const std::unordered_map< X, Y > &
// const std::unordered_set< X > &
//
// * set membership
// If C is a container and e is an element, use one of the following for
// membership check:
// has< T, S > (C, e)
// is_member< S, T >(e, C) where T is a container of S-values and
// T::find(const S &) is defined
// * set union
// If X, Y, Z are containers of type std::set< T >, std::unordered_set< T >
// X |= Y // X = X union with Y
// Z = X | Y // Z = X union with Y
// (See also std::set_union)
//
// * set intersection
// If X, Y, Z are containers of type std::set< T >, std::unordered_set< T >
// X &= Y
// Z = X & Y
// (See also C++ std::set_intersection)
//
// * set difference
// If X, Y, Z are containers of type std::set< T >, std::unordered_set< T >
// X -= Y
// Z = X - Y
// (See also C++ std::set_difference)
//
// * cross product
// If X, Y, Z are containers of type std::set< T >, std::unordered_set< T >
// Z = crossprod(X, Y)
//
// * powerset
// If X, Z are containers of type of std::set< T >, std::unordered_set< T >
// Z = powset(X)
//
//=============================================================================

//-----------------------------------------------------------------------------
// Prototypes of std::to_string extensions
//-----------------------------------------------------------------------------
namespace std
{
    std::string to_string(char c);
    
    std::string to_string(const std::string & x);
    
    template < typename S, typename T >
    std::string to_string(const std::pair< S, T > & x);

    template < typename S, typename T >
    std::string to_string(const std::variant< S, T > & x);
    
    template < typename T >
    std::string container_to_string_(const T & x,
                                     const std::string & begin,
                                     const std::string & end);
    
    template < typename T >
    std::string to_string(const std::vector< T > & x);
    
    template < typename T >
    std::string to_string(const std::list< T > & x);
    
    template < typename T >
    std::string to_string(const std::unordered_set< T > & x);
    
    template < typename S, typename T, typename LESS >
    std::string to_string(const std::unordered_map< S, T > & x,
                          const LESS & less);
    
    template < typename S, typename T >
    std::string to_string(const std::unordered_map< S, T > & x);
    
    template < typename T, typename LESS >
    std::string to_string(const std::unordered_set< T > & x,
                          const LESS & less);
    template < typename T, typename LESS >
    std::string to_string(const std::unordered_set< T > & x);
    
    template <typename T >
    std::string to_string(const std::set< T > & x);
    
    template < typename T, typename LESS >
    std::string to_string(const std::set< T > & x, const LESS & less);
};

//-----------------------------------------------------------------------------
// Prototypes of std::hash extensions
//-----------------------------------------------------------------------------
namespace std
{
    template < typename S, typename T >
    struct hash< std::pair< S, T > >;

    // template < typename S, typename T >
    // struct hash< std::variant< S , T > >;
    
    template < typename T >
    struct hash< std::vector< T > >;
    
    template < typename T >
    struct hash< std::list< T > >;
    
    template < typename T >
    struct hash< std::unordered_set< T > >;
    
    template < typename S, typename T, typename H >
    struct hash< std::unordered_map< S, T, H > >;
    
    template < typename S >
    struct hash< std::set< S > >;
}

//-----------------------------------------------------------------------------
// Prototypes of operator<<
//-----------------------------------------------------------------------------
template < typename T0, typename T1 >
std::ostream & operator<<(std::ostream & cout, const std::pair< T0, T1 > & x);

template < typename T >
std::ostream & operator<<(std::ostream & cout, const std::vector< T > & s);

template < typename T >
std::ostream & operator<<(std::ostream & cout, const std::list< T > & s);

template < typename T >
std::ostream & operator<<(std::ostream & cout,
                          const std::unordered_set< T > & s);

template < typename S, typename T >
std::ostream & operator<<(std::ostream & cout,
                          const std::unordered_map< S, T > & s);

template < typename T >
std::ostream & operator<<(std::ostream & cout, const std::set< T > & s);

//-----------------------------------------------------------------------------
// Prototypes of membership
//-----------------------------------------------------------------------------
template < typename S, typename T >
bool is_member(const S & e, const T & X);

template < typename S, typename T >
bool has(const S & X, const T & e);

//-----------------------------------------------------------------------------
// Prototypes of set union
//-----------------------------------------------------------------------------
template < typename T >
void set_union(T & x, const T & y);

template < typename T >
std::set< T > & operator|=(std::set< T > & x, const std::set< T > & y);

template < typename T >
std::set< T > operator|(const std::set< T > & x, const std::set< T > & y);

template < typename T >
std::unordered_set< T > & operator|=(std::unordered_set< T > & x,
                                     const std::unordered_set< T > & y);

template < typename T >
std::unordered_set< T > operator|(const std::unordered_set< T > & x,
                                  const std::unordered_set< T > & y);

//-----------------------------------------------------------------------------
// Prototypes of set intersection
//-----------------------------------------------------------------------------
template < typename T >
void set_intersection_(T & xs, const T & ys);

template < typename T >
std::set< T > & operator&=(std::set< T > & xs, const std::set< T > & ys);

template < typename T >
std::set< T > operator&(const std::set< T > & x, const std::set< T > & y);

template < typename T >
std::unordered_set< T > operator&=(std::unordered_set< T > & xs,
                                   const std::unordered_set< T > & ys);

template < typename T >
std::unordered_set< T > operator&(const std::unordered_set< T > & x,
                                  const std::unordered_set< T > & y);

//-----------------------------------------------------------------------------
// Prototypes for set difference
//-----------------------------------------------------------------------------
template < typename T0, typename T1 >
void set_difference_(T0 & X, const T1 & Y);

template < typename T >
const std::unordered_set< T > & operator-=(std::unordered_set< T > & X,
                                           const std::unordered_set< T > & Y);

template < typename T >
std::unordered_set< T > operator-(const std::unordered_set< T > & X,
                                  const std::unordered_set< T > & Y);

template < typename T >
const std::set< T > & operator-=(std::set< T > & X, const std::set< T > & Y);

template < typename T >
std::set< T > operator-(const std::set< T > & X, const std::set< T > & Y);

//-----------------------------------------------------------------------------
// Prototypes of cross product
//-----------------------------------------------------------------------------
template < typename T0, typename T1, typename T2 >
void crossprod_(T0 & ret, const T1 & xs, const T2 & ys);

template < typename T0, typename T1 >
std::set< std::pair< T0, T1 > > crossprod(const std::set< T0 > & xs,
                                          const std::set< T1 > & ys);

template < typename T0, typename T1 >
std::unordered_set< std::pair< T0, T1 > > crossprod(
    const std::unordered_set< T0 > & xs,
    const std::unordered_set< T1 > & ys);

template < typename T0, typename T1 >
std::set< std::pair< T0, T1 > > operator*(const std::set< T0 > & xs,
                                          const std::set< T1 > & ys);

template < typename T0, typename T1 >
std::unordered_set< std::pair< T0, T1 > > operator*(const std::unordered_set< T0 > & xs,
                                          const std::unordered_set< T1 > & ys);

//-----------------------------------------------------------------------------
// Prototypes of powerset
//-----------------------------------------------------------------------------
template < typename T0, typename T1 >
T0 powset_(const T1 & X);

template < typename T >
std::set< std::set< T > > powset(const std::set< T > & X);

template < typename T >
std::unordered_set< std::unordered_set< T > > powset(
    const std::unordered_set< T > & X);

//=============================================================================
// Implementations
//=============================================================================
//-----------------------------------------------------------------------------
// Implementation of std::to_string extensions
//-----------------------------------------------------------------------------
namespace std
{
    inline
    std::string to_string(char c)
    {
        return {c};
    }
    
    inline
    std::string to_string(const std::string & x)
    {
        return x;
    }
    
    // template <typename T>
    // std::string to_string(const T & x)
    // {
        
    // }
    
    template <typename S, typename T>
    std::string to_string(const std::pair< S, T > & x)
    {
        return std::string("(") +
            to_string(x.first) + ", " + to_string(x.second) +
            ")";
    }

    template < typename S, typename T >
    std::string to_string(const std::variant< S, T > & x)
    {
        if (x.index() == 0)
            return "<" + to_string(std::get<0>(x)) + ":0>";
        else
            return "<" + to_string(std::get<1>(x)) + ":1>";
    }

    template < typename T >
    std::string container_to_string_(const T & x,
                                     const std::string & begin,
                                     const std::string & end)
    {
        std::string ret = begin;
        std::string delim = "";
        for (auto p: x)
        {
            ret += delim;
            ret += to_string(p);
            delim = ", ";
        }
        ret += end;

        return ret;
    }
    
    template < typename T >
    std::string to_string(const std::vector< T > & x)
    {
        return container_to_string_(x, "[", "]");
    }

    template < typename T >
    std::string to_string(const std::list< T > & x)
    {
        return container_to_string_(x, "[", "]");
    }

    
    // template < typename S, typename T, typename LESS >
    // std::string to_string(const std::unordered_map< S, T > & x,
    //                       const LESS & less)
    // {
        
    // }
    
    template < typename S, typename T >
    std::string to_string(const std::unordered_map< S, T > & x)
    {
        return container_to_string_(x, "{", "}");
    }
    
    // template < typename T, typename LESS >
    // std::string to_string(const std::unordered_set< T > & x,
    //                       const LESS & less)
    // {
    //     return container_to_string_(x, "{", "}");
    // }

    template < typename T >
    std::string to_string(const std::unordered_set< T > & x)
    {
        return container_to_string_(x, "{", "}");
    }
    
    template <typename T >
    std::string to_string(const std::set< T > & x)
    {
        return container_to_string_(x, "{", "}");
    }
    
    template < typename T, typename LESS >
    std::string to_string(const std::set< T > & x, const LESS & less)
    {
        return container_to_string_(x, "{", "}");
    }
}

//-----------------------------------------------------------------------------
// Implementations of extensions to std::hash
//-----------------------------------------------------------------------------
namespace std
{
    template < typename S, typename T >
    struct hash< std::pair< S, T > >
    {
        size_t operator()(const std::pair< S, T > & x) const
        {
            std::hash< std::string > hasher;
            return hasher(std::to_string(x));
        }
    };

    // template < typename S, typename T >
    // struct hash< std::variant< S , T > >
    // {
    //     size_t operator()(const std::pair< S, T > & v) const
    //     {
    //         std::hash< std::string > hasher;
    //         string x = to_string(get<0>(v));
    //         x += to_string(get<1>(v));
            
    //         return hasher(x);
    //     }
    // };
    
    template < typename T >
    struct hash< std::vector< T > >
    {
        size_t operator()(const std::vector< T > & x) const
        {
            std::hash< std::string > hasher;
            return hasher(std::to_string(x));
        }
    };

    template < typename T >
    struct hash< std::list< T > >
    {
        size_t operator()(const std::list< T > & x) const
        {
            std::hash< std::string > hasher;
            return hasher(std::to_string(x));
        }
    };

    template < typename T >
    struct hash< std::unordered_set< T > >
    {
        size_t operator()(const std::unordered_set< T > & x) const
        {
            std::hash< std::string > hasher;
            return hasher(std::to_string(x));
        }
    };

    template < typename S, typename T, typename H >
    struct hash< std::unordered_map< S, T, H > >
    {
        size_t operator()(const std::unordered_map< S, T, H > & x) const
        {
            std::hash< std::string > hasher;
            return hasher(std::to_string(x));
        }
    };
    
    template < typename S >
    struct hash< std::set< S > >
    {
        size_t operator()(const std::set< S > & x) const
        {
            std::hash< std::string > hasher;
            return hasher(std::to_string(x));
        }
    };
}

//-----------------------------------------------------------------------------
// Implementation of memberships
//-----------------------------------------------------------------------------
template < typename S, typename T >
bool is_member(const S & e,
               const T & X)
{
    return X.find(e) != X.end();
}

//-----------------------------------------------------------------------------
// Implementations of set difference
//-----------------------------------------------------------------------------
template < typename T0, typename T1 >
void set_difference_(T0 & X, const T1 & Y)
{
    for (auto p: Y)
    {
        if (is_member(p, X))
            X.erase(p);
    }
}

template < typename T >
const std::unordered_set< T > & operator-=(std::unordered_set< T > & X,
                                           const std::unordered_set< T > & Y)
{
    set_difference_(X, Y);
    return X;
}

template < typename T >
std::unordered_set< T > operator-(const std::unordered_set< T > & X,
                                  const std::unordered_set< T > & Y)
{
    unordered_set<T> diff = X;
    diff -= Y;
    return diff;
}

template < typename T >
const std::set< T > & operator-=(std::set< T > & X, const std::set< T > & Y)
{
    set_difference_(X, Y);
    return X;
}

template < typename T >
std::set< T > operator-(const std::set< T > & X, const std::set< T > & Y)
{
    std::set<T> diff = X;
    diff -= Y;
    return diff;
}

//-----------------------------------------------------------------------------
// Implementations of set union
//-----------------------------------------------------------------------------
template < typename T >
void set_union(T & x, const T & y)
{
    for (auto p: y)
    {
        x.insert(p);
    }
}
template < typename T >
std::set< T > & operator|=(std::set< T > & x, const std::set< T > & y)
{
    set_union(x, y);
    return x;
}

template < typename T >
std::set< T > operator|(const std::set< T > & x, const std::set< T > & y)
{
    std::set<T> union_ = x;
    union_ |= y;
    return union_;
}

template < typename T >
std::unordered_set< T > & operator|=(std::unordered_set< T > & x,
                                     const std::unordered_set< T > & y)
{
    set_union(x, y);
    return x;
}

template < typename T >
std::unordered_set< T > operator|(const std::unordered_set< T > & x,
                                  const std::unordered_set< T > & y)
{
    std::unordered_set<T> union_ = x;
    union_ |= y;
    return union_;
}

//-----------------------------------------------------------------------------
// Implementations of set intersection
//-----------------------------------------------------------------------------
template < typename T >
void set_intersection_(T & xs, const T & ys)
{
    T x = xs;
    xs.clear();
    for (auto p: x)
    {
        if (is_member(p, ys))
            xs.insert(p);
    }
}

template < typename T >
std::set< T > & operator&=(std::set< T > & xs, const std::set< T > & ys)
{
    set_intersection_(xs, ys);
    return xs;
}

template < typename T >
std::set< T > operator&(const std::set< T > & xs, const std::set< T > & ys)
{
    std::set<T> intersect_ = xs;
    intersect_ &= ys;
    return intersect_;
}

template < typename T >
std::unordered_set< T > operator&=(std::unordered_set< T > & xs,
                                   const std::unordered_set< T > & ys)
{
    set_intersection_(xs, ys);
    return xs;
}

template < typename T >
std::unordered_set< T > operator&(const std::unordered_set< T > & xs,
                                  const std::unordered_set< T > & ys)
{
    std::unordered_set<T> intersect_ = xs;
    intersect_ &= ys;
    return intersect_;
}

//------------------------------------------------------------------------------
// Implementations of cross product
//------------------------------------------------------------------------------
template < typename T0, typename T1, typename T2 >
void crossprod_(T0 & ret, const T1 & xs, const T2 & ys)
{
    for (auto p: xs)
    {
        for (auto q: ys)
        {
            ret.insert(std::make_pair(p, q));
        }
    }
}

template < typename T0, typename T1 >
std::set< std::pair< T0, T1 > > crossprod(const std::set< T0 > & xs,
                                          const std::set< T1 > & ys)
{
    std::set< std::pair< T0, T1 > > cP;
    crossprod_(cP, xs, ys);

    return cP;
}

template < typename T0, typename T1 >
std::unordered_set< std::pair< T0, T1 > > crossprod(
    const std::unordered_set< T0 > & xs,
    const std::unordered_set< T1 > & ys)
{
    std::unordered_set< std::pair< T0, T1 > > cP;
    crossprod_(cP, xs, ys);

    return cP;
}

template < typename T0, typename T1 >
std::set< std::pair< T0, T1 > > operator*(const std::set< T0 > & xs,
                                          const std::set< T1 > & ys)
{
    return crossprod(xs, ys);
}

template < typename T0, typename T1 >
std::unordered_set< std::pair< T0, T1 > > operator*(const std::unordered_set< T0 > & xs,
                                                    const std::unordered_set< T1 > & ys)
{
    return crossprod(xs, ys);
}
//-----------------------------------------------------------------------------
// Implementations of powerset
//-----------------------------------------------------------------------------
template < typename T0, typename T1 >
T0 powset_(const T1 & X)
{
    T0 ret;
    std::vector<typename T1::value_type> elements(X.begin(), X.end());

    int n = elements.size();
    
    for (int mask = 0; mask < (1 << n); ++mask)
    {
        T1 subset;

        for (int i = 0; i < n; ++i)
        {
            if (mask & (1 << i))
                subset.insert(elements[i]);
        }

        ret.insert(subset);
    }

    return ret;
}

template < typename T >
std::set< std::set< T > > powset(const std::set< T > & X)
{
    return powset_(X);
}

template < typename T >
std::unordered_set< std::unordered_set< T > > powset(
    const std::unordered_set< T > & X)
{
    return powset_(X);
}

//-----------------------------------------------------------------------------
// Implementations of operator<<
//-----------------------------------------------------------------------------
template < typename T0, typename T1 >
std::ostream & operator<<(std::ostream & cout, const std::pair< T0, T1 > & x)
{
    cout << std::to_string(x);
    return cout;
}

template < typename T >
std::ostream & operator<<(std::ostream & cout, const std::vector< T > & s)
{
    cout << to_string(s);
    return cout;
}

// template < typename T >
// std::ostream & operator<<(std::ostream & cout, const std::list< T > & s)
// {
//     cout << to_string(s);
//     return cout;
// }

template < typename T >
std::ostream & operator<<(std::ostream & cout,
                          const std::unordered_set< T > & s)
{
    cout << to_string(s);
    return cout;
}

template < typename S, typename T >
std::ostream & operator<<(std::ostream & cout,
                          const std::unordered_map< S, T > & s)
{
    cout << to_string(s);
    return cout;
}

template < typename T >
std::ostream & operator<<(std::ostream & cout, const std::set< T > & s)
{
    cout << to_string(s);
    return cout;
}

#endif
