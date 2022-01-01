/*
 * Created by flashbeard.
 * number-theory
 */

#ifndef TEST_MODULAR_H
#define TEST_MODULAR_H

#include <type_traits>
#include <iostream>

namespace nt {

    template<auto M>
    class Modular {
    public:
        using ic = std::integral_constant<std::decay_t<decltype(M)>, M>;
        using T = typename std::decay_t<decltype(ic::value)>;

        using Modular_m = Modular<M>;

        static_assert(std::is_integral_v<T>, "Modular number must be an integer type");
        static_assert(!std::is_same_v<std::remove_cv_t<T>, bool>, "Modular number must not be a boolean type");

#pragma region Constructors

        constexpr Modular() : m_value() {}

        template<class U>
        Modular &operator=(const U &other) {
            if constexpr(std::is_same_v<std::remove_cv_t<U>, Modular_m>) {
                if (this != &other) {
                    m_value = other.value;
                }
            } else {
                *this = Modular_m(other);
            }

            return *this;
        }

        template<class U>
        Modular(const U &x) {
            m_value = normalize(x);
        }

#pragma endregion Constructors

#pragma region Utilities

        constexpr static T mod() { return ic::value; }

        template<class U>
        static T normalize(const U &x) {
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

        const T &operator()() const {
            return m_value;
        }

        template<class U>
        explicit operator U() const {
            return static_cast<U>(m_value);
        }

#pragma endregion Utilities

#pragma region ArithmeticOperators

        Modular operator+(Modular_m &other) {
            T value = this->m_value + other.m_value;
            return normalize(value);
        }

        void operator+=(Modular_m &other) {
            this = this + other;
        }

        Modular operator-(Modular_m &other) {
            T value = this->m_value - other.m_value;
            return normalize(value);
        }

        void operator-=(Modular_m &other) {
            this = this - other;
        }

        Modular operator*(Modular_m &other) {
            T result = 0;
            T a = this->m_value;
            T b = other.m_value;
            while(b) {
                if(b & 1) {
                    result += a;
                    result %= mod();
                }
                b >>= 1;
                if(a < mod() - a) {
                    a <<= 1;
                } else {
                    a -= (mod() - a);
                }
            }
            return result;
        }

        void operator*=(Modular_m &other) {
            this = this * other;
        }

        Modular operator/(Modular_m &other) {
            // this * (other ^ (mod() - 2))
        }

        void operator/=(Modular_m &other) {
            this = this / other;
        }

#pragma endregion ArithmeticOperators

#pragma region ComparisonOperators



#pragma endregion ComparisonOperators

#pragma region IO

        friend std::ostream &operator<<(std::ostream &out, const Modular &m) {
            out << m.m_value;
            return out;
        }

        friend std::istream &operator>>(std::istream &in, Modular &m) {
            T x;
            in >> x;
            m = Modular_m(x);
            return in;
        }

#pragma endregion IO

    private:
        T m_value;
    };

}

// TODO: (DS) - add custom type traits
namespace std {
    template<auto M>
    struct __is_integral_helper<nt::Modular<M>> : public true_type {
    };
}

#endif //TEST_MODULAR_H
