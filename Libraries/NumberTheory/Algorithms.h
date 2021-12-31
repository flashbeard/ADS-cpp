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

    // FIXME: (DS) - Why do we need this?
    using std::gcd;
    using std::lcm;

    template<class T, class U, class K = std::common_type_t<T, U>>
    constexpr K power(T a, U n) {
        static_assert(std::is_integral_v<K>, "power arguments must be integer types");
        static_assert(!std::is_same_v<std::remove_cv_t<K>, bool>, "power arguments must not be boolean types");

        static_assert(n >= 0, "power exponent must not be negative");

        K result = 1;

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

    template<class T, class U, class V, class K = std::common_type_t<T, U, V>>
    constexpr K power(T a, U n, V mod) {
        static_assert(std::is_integral_v<V>, "power modulo must be integer types");
        static_assert(!std::is_same_v<std::remove_cv_t<V>, bool>, "power modulo must not be boolean types");

        // FIXME: (DS) - Is it true, that (a ^ n) mod m equals to (a mod m) ^ (n mod m)
        Modular modular_a(a, mod), modular_n(n, mod);
        return power(modular_a, modular_n);
    }

    template<class T>
    bool is_power_of_2(T x) {
        static_assert(std::is_integral_v<T>, "number must be integer type");
        static_assert(!std::is_same_v<std::remove_cv_t<T>, bool>, "number must not be boolean type");

        return x && !(x & (x - 1));
    }

    template<class T>
    bool is_prime(T x) {
        if (x == 2 || x == 3 || x == 5 || x == 7 || x == 11 || x == 13 || x == 17 || x == 23 || x == 29) return true;
        if (x < 30 || (x & 1) == 0) return false;
        T d = 3;
        while (d * d <= x) {
            if (x % d == 0) return false;
        }
        return true;
    }

    // TODO: add primality test - Solovay-Strassen - O(k lon^3 n)
    // https://www.geeksforgeeks.org/primality-test-set-4-solovay-strassen/?ref=lbp

    template<class T, class U, class K = std::common_type_t<T, U>>
    constexpr K gcd(T a, U b) {
        static_assert((std::is_integral_v<K>), "gcd arguments must be integer types");

        static_assert((!std::is_same_v<std::remove_cv_t<K>, bool>), "gcd arguments must not be bool types" );

        if (a < 0) a = -a;
        if (b < 0) b = -b;
        while (b) {
            a %= b;
            if (!a) return b;
            b %= a;
        }
        return a;
    }
}

#endif //TEST_ALGORITHMS_H
