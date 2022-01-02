//
// Created by Daniel Pustotin on 02.01.2022.
//

#ifndef ADS_CPP_BIG_H
#define ADS_CPP_BIG_H

#include <type_traits>
#include <iostream>

#include "nt_type_traits.h"

namespace nt {

    // FIXME: (DS) - check the structure
    template<auto B>
    class Big {
    public:
        using ic = std::integral_constant<std::decay_t<decltype(B)>, B>;
        using T = typename std::decay_t<decltype(ic::value)>;
        using ll = long long;

        using Big_b = Big<B>;

        static_assert(is_integral_v<T>, "Big number must be integer type");

#pragma region Constructors

        constexpr Big() : b_value() {}

        Big &operator=(const Big_b &other) {
            if (this != &other) {
                b_value = other.value;
            }
            return *this;
        }

        Big &operator=(const std::string &other) {
            int idx = 0;
            b_value.assign(1, 0);
            for (char c : other) {

            }

            return *this;
        }

        template<class U>
        Big(const U &x) {
            // TODO: (DS) - Need to be done
        }

#pragma endregion Constructors

#pragma region Utilities

        constexpr static T maxDigit() { return std::numeric_limits<T>::max(); }

        const T &operator*() const {
            return b_value;
        }

        template<class U>
        explicit operator U() const {
            return static_cast<U>(b_value);
        }

#pragma endregion Utilities

#pragma region IO

        friend std::ostream &operator<<(std::ostream &out, const Big &b) {
            for (T t : b) {
                out << b.b_value;
            }
            return out;
        }

        friend std::istream &operator>>(std::istream &in, Big &b) {
            T x;
            in >> x;
            b = Big_b(x);
            return in;
        }

#pragma endregion IO

    private:
        std::vector<T> b_value;
    };

    template<auto B>
    struct __is_integral_helper<Big<B>> : public std::true_type {
    };
}

#endif //ADS_CPP_BIG_H
