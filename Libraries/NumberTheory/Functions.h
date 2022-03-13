#pragma once

#include <limits>

namespace funcs {

template<class T, class U, class V = std::common_type_t<T, U>>
V normalize(const T &x, const U &mod)
{
    V value;

    if (-mod <= x && x < mod) {
        value = static_cast<T>(x);
    } else {
        value = static_cast<T>(x % mod);
    }

    if (value < 0) {
        value += mod;
    }

    return value;
}

template<class T, class U>
constexpr T power(T a, U n)
{
    T result = 1;

    while (n) {
        if (n & 1) {
            result *= a;
            --n;
        } else {
            a *= a;
            n >>= 1;
        }
    }

    return result;
}

template<class T>
constexpr T multiply(T l, T r, const T &mod)
{
    l = normalize(l, mod);
    r = normalize(r, mod);

    // use fast multiplication for small modules
    if (std::numeric_limits<T>::max() / l > r) {
        return normalize(l * r, mod);
    }

    if (l < r) {
        std::swap(l, r);
    }

    T result = 0;

    while (r) {
        if (r & 1) {
            result += l;
            result = normalize(result, mod);
        }
        r >>= 1;
        if (l < mod - l) {
            l <<= 1;
        } else {
            l -= (mod - l);
        }
    }

    return result;
}

template<class T, class U>
constexpr void gcd(T &a, U b)
{
    if (!a && !b) {
        a = 1;
        return;
    }
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) {
        a %= b;
        if (!a) {
            a = b;
            break;
        }
        b %= a;
    }
}

template<class T, class U>
constexpr void lcm(T &a, U b)
{
    // FIXME: (DS) - can we call gcd(b, a) and do not use _a?
    T _a = a;
    gcd(_a, b);
    a /= _a;
    a *= b;
}

}  // namespace funcs
