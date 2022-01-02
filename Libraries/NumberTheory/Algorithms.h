/*
 * Created by flashbeard.
 * number-theory
 */

#ifndef TEST_ALGORITHMS_H
#define TEST_ALGORITHMS_H

#include <type_traits>
#include <numeric>
#include <algorithm>

#include "Functions.h"
#include "Modular.h"
#include "nt_type_traits.h"

namespace nt {

    template<class T, class U>
    constexpr T power(T a, U n) {
        static_assert(is_integral_v<T>, "power arguments must be integer types");

        if (n < 0) {
            throw std::runtime_error("power exponent must not be negative");
        }

        return funcs::power(a, n);
    }

    template<class T>
    bool is_power_of_2(const T &x) {
        static_assert(is_integral_v<T>, "is_power_of_2 arguments must be integer types");

        return x && !(x & (x - 1));
    }

    template<class T>
    bool is_prime(const T &x) {
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
        static_assert(is_integral_v<T>, "gcd arguments must be integer types");

        if (a < 0) a = -a;
        if (b < 0) b = -b;
        while (b) {
            a %= b;
            if (!a) return b;
            b %= a;
        }
        return a;
    }

    // FIXME: Need to be done
    template<typename ... Args>
    constexpr auto gcd(Args... a) {
//        return gcd(a, ...);
    }
}

#endif //TEST_ALGORITHMS_H
