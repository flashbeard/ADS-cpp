#ifndef TEST_MODULAR_H
#define TEST_MODULAR_H

#include <type_traits>
#include <iostream>

namespace nt {

    template<class T>
    class Modular {
    public:
        static_assert(std::is_integral_v<T>, "Modular mod is integer");
        static_assert(!std::is_same_v<std::remove_cv_t<T>, bool>, "Modular mod is not bool");

        constexpr Modular(T value, T mod) : m_value(value % mod), m_mod(mod) {}

        template<class U>
        Modular &operator=(const U &other) {
            if constexpr(std::is_same_v<std::remove_cv_t<U>, Modular>) {
                if (this != &other) {
                    m_value = other.m_value;
                    m_mod = other.m_mod;
                }
            } else {
                m_value = other;
                m_value %= m_mod;
            }

            return *this;
        }

        friend std::ostream &operator<<(std::ostream &out, const Modular m) {
            out << m.m_value;
            return out;
        }

    private:
        T m_value, m_mod;
    };

}


#endif //TEST_MODULAR_H
