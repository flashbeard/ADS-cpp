/*
 * Created by flashbeard.
 * number-theory
 */

#ifndef TEST_MODULAR_H
#define TEST_MODULAR_H

#include <type_traits>
#include <iostream>

#include "Functions.h"
#include "nt_type_traits.h"

namespace nt {

    template<auto M>
    class Modular {
    public:
        using ic = std::integral_constant<std::decay_t<decltype(M)>, M>;
        using T = typename std::decay_t<decltype(ic::value)>;

        using Modular_m = Modular<M>;

        static_assert(is_integral_v<T>, "Modular number must be integer types");

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

        const T &operator*() const {
            return m_value;
        }

        template<class U>
        explicit operator U() const {
            return static_cast<U>(m_value);
        }

#pragma endregion Utilities

#pragma region ArithmeticOperators

        // plus
        friend Modular operator+(const Modular_m &lhs, const Modular_m &rhs) {
            return *lhs - mod() + *rhs;
        }

        template<class U>
        friend Modular operator+(const U &lhs, const Modular_m &rhs) {
            return *Modular_m(lhs) - mod() + *rhs;
        }

        template<class U>
        friend Modular operator+(const Modular_m &lhs, const U &rhs) {
            return *lhs - mod() + *Modular_m(rhs);
        }

        // minus
        friend Modular operator-(const Modular_m &lhs, const Modular_m &rhs) {
            return mod() - *rhs + *lhs;
        }

        template<class U>
        friend Modular operator-(const U &lhs, const Modular_m &rhs) {
            return mod() - *rhs + *Modular_m(lhs);
        }

        template<class U>
        friend Modular operator-(const Modular_m &lhs, const U &rhs) {
            return mod() - *Modular_m(rhs) + *lhs;
        }

        // multiply
        friend Modular operator*(const Modular_m &lhs, const Modular_m &rhs) {
            return funcs::multiply(*lhs, *rhs, mod());
        }

        template<class U>
        friend Modular operator*(const U &lhs, const Modular_m &rhs) {
            return funcs::multiply(*Modular_m(lhs), *rhs, mod());
        }

        template<class U>
        friend Modular operator*(const Modular_m &lhs, const U &rhs) {
            return funcs::multiply(*lhs, *Modular_m(rhs), mod());
        }

        // divide
        friend Modular operator/(const Modular_m &lhs, const Modular_m &rhs) {
            return *lhs * funcs::power(*rhs, mod() - 2);
        }

        template<class U>
        friend Modular operator/(const U &lhs, const Modular_m &rhs) {
            return *Modular_m(lhs) * funcs::power(*rhs, mod() - 2);
        }

        template<class U>
        friend Modular operator/(const Modular_m &lhs, const U &rhs) {
            return *lhs * funcs::power(*Modular_m(rhs), mod() - 2);
        }

#pragma endregion ArithmeticOperators

#pragma region ArithmeticAssignOperators

        // plus
        template<class U>
        friend void operator+=(Modular_m &lhs, const U &rhs) {
            lhs = lhs + rhs;
        }

        // minus
        template<class U>
        friend void operator-=(Modular_m &lhs, const U &rhs) {
            lhs = lhs - rhs;
        }

        // multiply
        template<class U>
        friend void operator*=(Modular_m &lhs, const U &rhs) {
            lhs = lhs * rhs;
        }

        // divide
        template<class U>
        friend void operator/=(Modular_m &lhs, const U &rhs) {
            lhs = lhs / rhs;
        }

#pragma endregion

#pragma region ComparisonOperators

#define OP(SIGN) \
friend bool operator SIGN(const Modular_m &lhs, const Modular_m &rhs) { return *lhs SIGN *rhs; }\
template<class U>friend bool operator SIGN(const U &lhs, const Modular_m &rhs) { return *Modular_m(lhs) SIGN *rhs; }\
template<class U>friend bool operator SIGN(const Modular_m &lhs, const U &rhs) { return *lhs SIGN *Modular_m(rhs); }

    OP(==) OP(!=) OP(<) OP(>) OP(<=) OP(>=)

#undef OP

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

    template<auto M>
    struct __is_integral_helper<Modular<M>> : public std::true_type {
    };

}

#endif //TEST_MODULAR_H
