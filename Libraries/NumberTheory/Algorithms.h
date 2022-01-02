/*
 * Created by flashbeard.
 * number-theory
 */

#ifndef TEST_ALGORITHMS_H
#define TEST_ALGORITHMS_H

#include <type_traits>
#include <numeric>
#include <algorithm>
#include <vector>

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
        static_assert(is_integral_v<T>, "is_power_of_2 argument must be integer type");

        return x && !(x & (x - 1));
    }

    template<class T>
    bool is_prime(const T &x) {
        if (x == 2 || x == 3 || x == 5 || x == 7 || x == 11 || x == 13 || x == 17 || x == 23 || x == 29) return true;
        if (!(x % 2) || !(x % 3) || !(x % 5) || !(x % 7) || !(x % 11) || !(x % 13) || !(x % 17) || !(x % 23) ||
            !(x % 29))
            return false;
        if (x < 30 || (x & 1) == 0) return false;
        T d = 31;
        while (d * d <= x) {
            if (x % d == 0) return false;
            d += (d + 2 % 3) ? 2 : 4;
        }
        return true;
    }

    // TODO: (DP) - add primality test - Solovay-Strassen - O(k lon^3 n)
    // https://www.geeksforgeeks.org/primality-test-set-4-solovay-strassen/?ref=lbp

    template<class U, class... Args, class K = std::common_type_t<U, Args...>>
    constexpr K gcd(U a, Args &&...args) {
        static_assert((is_integral_v<std::remove_reference_t<decltype(args)>> && ... && is_integral_v<U>),
                      "gcd arguments must be integer types");
        K result = a;
        (funcs::gcd(result, std::forward<Args>(args)), ...);
        return result;
    }

    template<class U, class... Args, class K = std::common_type_t<U, Args...>>
    constexpr K lcm(U a, Args &&...args) {
        static_assert((is_integral_v<std::remove_reference_t<decltype(args)>> && ... && is_integral_v<U>),
                      "lcm arguments must be integer types");
        K result = a;
        (funcs::lcm(result, std::forward<Args>(args)), ...);
        return result;
    }

    template<auto M, class T>
    constexpr void compute_factorials(std::vector<Modular<M>> &fact, T &cnt) {
        fact.assign(cnt + 1);
        for (T i = 1; i <= cnt; ++i) {
            fact[i] = fact[i - 1] * i;
        }
    }

    template<auto M, class T>
    constexpr void compute_factorials(std::vector<Modular<M>> &fact, std::vector<Modular<M>> &inv, T &cnt) {
        factorials(fact, cnt);
        inv.assign(cnt + 1);
        for (T i = 0; i <= cnt; ++i) {
            inv[i] = power(fact[i], Modular<M>().mod() - 2);
        }
    }

    template<auto M, class T>
    constexpr void compute_combinations(std::vector<std::vector<Modular<M>>> &combinations, T &cnt) {
        combinations.assign(cnt, std::vector(cnt, 0));
        for (T n = 0; n <= cnt; ++n) {
            combinations[n][0] = 1;
            combinations[n][n] = 1;
            for (T k = 1; k < n; ++k) {
                combinations[n - 1][k] + combinations[n - 1][k - 1];
            }
        }
    }
}

#endif //TEST_ALGORITHMS_H
