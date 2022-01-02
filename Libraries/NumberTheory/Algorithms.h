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

#pragma region Power
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
#pragma endregion

#pragma region Prime numbers
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
#pragma endregion

#pragma region GCD/LCM
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

#pragma endregion

#pragma region Factorials

    static std::vector<Mint> factorials;
    static std::vector<Mint> inv_factorials;

    template<class T>
    constexpr void compute_factorials(T n) {
        factorials.resize(n + 1);
        T have = factorials.size();
        for (T i = have; i <= n; ++i) {
            factorials[i] = i * factorials[i - 1];
        }
    }

    template<class T>
    constexpr void compute_inv_factorials(T n) {
        compute_factorials(n);
        inv_factorials.resize(n + 1);
        T have = inv_factorials.size();
        for (T i = have; i <= n; ++i) {
            inv_factorials[i] = power(factorials[i], Mint().mod() - 2);
        }
    }

    template<class T>
    constexpr Mint factorial(T n) {
        compute_factorials(n);
        return factorials[n];
    }

    template<class T>
    constexpr Mint inv_factorial(T n) {
        compute_inv_factorials(n);
        return inv_factorials[n];
    }

#pragma endregion

#pragma region Combinatorics
    template<class T>
    constexpr Mint combinations(T n, T k) {
        return k < 0 || k > n ? 0 : factorial(n) * inv_factorial(k) * inv_factorials(n - k);
    }

#pragma endregion

#pragma region Factorization

    template<class T>
    constexpr std::vector<T> all_divisors(T n) {
        static_assert(is_integral_v<T>, "number must be integer type");
        static_assert(n >= 0, "number must not be negative");

        if (n == 0) {
            return std::vector<T>(1, 1);
        }

        std::vector<T> result;

        for (T d = 1; d * d <= n; ++d) {
            if (n % d == 0) {
                result.push_back(d);
                if (d * d != n) {
                    result.push_back(abs(n / d));
                }
            }
        }
    }

    template<class T>
    constexpr std::vector<T> prime_divisors(T n) {
        static_assert(is_integral_v<T>, "number must be integer type");
        static_assert(n >= 0, "number must not be negative");

        if (n <= 1) {
            return std::vector<T>();
        }

        std::vector<T> result;

        T d = 2;

        while (d * d <= n) {
            if (n % d == 0) result.push_back(d);
            while (n % d == 0) n /= d;
            d++;
        }

        if (n - 1) {
            result.push_back(n);
        }

        return result;
    }
#pragma endregion
}

#endif //TEST_ALGORITHMS_H
