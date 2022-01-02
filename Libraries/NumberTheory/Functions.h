/*
 * Created by flashbeard.
 * number-theory
 */

#ifndef ADS_CPP_FUNCTIONS_H
#define ADS_CPP_FUNCTIONS_H

#include <limits>

namespace funcs {

    template<class T>
    T normalize(const T &x, const T &mod) {
        T value;

        if (-mod() <= x && x < mod()) {
            value = static_cast<T>(x);
        } else {
            value = static_cast<T>(x % mod());
        }

        if (value < 0) {
            value += mod();
        }

        return value;
    }

    template<class T, class U>
    constexpr T power(T a, U n) {
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
    constexpr T multiply(T l, T r, const T &mod) {

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

}

#endif //ADS_CPP_FUNCTIONS_H
