/*
 * Created by flashbeard.
 * number-theory
 */

#ifndef ADS_CPP_FUNCTIONS_H
#define ADS_CPP_FUNCTIONS_H

#include <limits>

namespace funcs {

    template<class T, class U>
    constexpr T power(T a, U n) {
        T result = 1;

        //FIXME: [a = a * a] -> [a *= a]
        while (n) {
            if (n & 1) {
                result = result * a;
                --n;
            } else {
                a = a * a;
                n >>= 1;
            }
        }

        return result;
    }

    template<class T>
    constexpr T multiply(T l, T r, const T &mod) {
        // TODO: think when we need to use fast formula

        T result = 0;

        if (l < r) {
            std::swap(l, r);
        }

        while (r) {
            if (r & 1) {
                result += l;
                result %= mod;
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
