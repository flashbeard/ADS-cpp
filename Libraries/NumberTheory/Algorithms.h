/*
 * Created by flashbeard.
 * On 31.12.2021
 * number-theory
 */

#ifndef TEST_ALGORITHMS_H
#define TEST_ALGORITHMS_H

#include <type_traits>
#include <numeric>
#include <algorithm>

#include "Modular.h"

namespace nt {

    // CHECK: (DS) - Why do we need this?
    using std::gcd;
    using std::lcm;

    template<class M, class N, class Type = std::common_type_t<M, N>>
    constexpr Type power(M a, N n) {
        static_assert(std::is_integral_v<M>, "power base must be an integer");
        static_assert(!std::is_same_v<std::remove_cv_t<M>, bool>, "power base must not be a boolean");

        static_assert(std::is_integral_v<N>, "power exponent must be an integer");
        static_assert(!std::is_same_v<std::remove_cv_t<N>, bool>, "power exponent must not be a boolean");

        static_assert(n >= 0, "power exponent must not be negative");

        Type result = 1;

        while (n) {
            if (n & 1) {
                result *= a;
                --n;
            } else {
                a *= a;
                n /= 2;
            }
        }

        return result;
    }

    template<class M, class N, class K>
    constexpr std::common_type_t<M, N, K> power(M a, N n, K mod) {
        static_assert(std::is_integral_v<K>, "power modulo must be an integer");
        static_assert(!std::is_same_v<std::remove_cv_t<K>, bool>, "power modulo must not be a boolean");

        // CHECK: (DS) - Is it true, that (a ^ n) mod m equals to (a mod m) ^ (n mod m)
        Modular modular_a(a, mod), modular_n(n, mod);
        return power(modular_a, modular_n);
    }

}

#endif //TEST_ALGORITHMS_H
