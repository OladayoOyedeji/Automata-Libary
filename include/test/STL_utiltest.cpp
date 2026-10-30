#include <iostream>
#include "STL_util.h"

int main()
{
// * std::string to_string(T) where T = int, double, char, bool,
// const std::string &
// const std::pair< X, Y > &
// const std::vector< X > &
// const std::list< X > & ... TODO
// const std::unordered_map< X, Y > &
// const std::unordered_set< X > &

// * std::hash< T > where T = int, double, char, bool,
// const std::string &
// const std::pair< X, Y > &
// const std::vector< X > &
// const std::list< X > & ... TODO
// const std::unordered_map< X, Y > &
// const std::unordered_set< X > &

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

// * set intersection
// If X, Y, Z are containers of type std::set< T >, std::unordered_set< T >
// X &= Y
// Z = X & Y
// (See also C++ std::set_intersection)

// * set difference
// If X, Y, Z are containers of type std::set< T >, std::unordered_set< T >
// X -= Y
// Z = X - Y
// (See also C++ std::set_difference)

// * cross product
// If X, Y, Z are containers of type std::set< T >, std::unordered_set< T >
// Z = crossprod(X, Y)

// * powerset
// If X, Z are containers of type of std::set< T >, std::unordered_set< T >
//                Z = powset(X);

    std::unordered_set<int> set0;
    set0.insert(4);set0.insert(5);set0.insert(6);set0.insert(7);set0.insert(8);set0.insert(9);
    std::unordered_set<int> set1;
    set1.insert(0);set1.insert(1);set1.insert(2);set1.insert(3);set1.insert(4);set1.insert(5);

    std::cout << "set 0: " <<  set0 << " set 1: " << set1 << std::endl;

    std::unordered_set<int> set2 = set0;
    set_difference_(set2, set1);
    std::cout << "set 2: " << set2 << std::endl;
    std::unordered_set<int> set3 = set0;
    set_intersection_(set3, set1);
    std::cout << "set 3: " << set3 << std::endl;
    
    std::unordered_set<std::pair<int, int> > set4;
    
    crossprod_(set4, set0, set1);
    std::cout << "set 4: " << set4 << std::endl;

    // std::unordered_set< std::unordered_set<int> > p = powset_(set0);
    std::unordered_set<std::unordered_set<int>> p =
    powset_<std::unordered_set<std::unordered_set<int>>>(set0);
    std::cout << "powerset: " << p << std::endl;
}
