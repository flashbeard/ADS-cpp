#ifndef TEST_ALGORITHMS_H
#define TEST_ALGORITHMS_H

#include <type_traits>
#include <numeric>
#include <algorithm>

#include "Modular.h"

namespace nt {

    using std::gcd;
    using std::lcm;

    template<class M, class N, class Type = std::common_type_t<M, N>>
    constexpr Type binpow(M a, N n) {
        static_assert(std::is_integral_v<M>, "binpow arguments are integers");
        static_assert(!std::is_same_v<std::remove_cv_t<M>, bool>, "binpow arguments are not bools");

        static_assert(std::is_integral_v<N>, "binpow arguments are integers");
        static_assert(!std::is_same_v<std::remove_cv_t<N>, bool>, "binpow arguments are not bools");

        Type result = 1;

        while (n) {
            if (n % 2) {
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
    constexpr std::common_type_t<M, N, K> binpow(M a, N n, K mod) {
        static_assert(std::is_integral_v<K>, "binpow arguments are integers");
        static_assert(!std::is_same_v<std::remove_cv_t<K>, bool>, "binpow arguments are not bools");

        Modular modular_a(a, mod), modular_n(n, mod);
        return binpow(modular_a, modular_n);
    }

}

#endif //TEST_ALGORITHMS_H
