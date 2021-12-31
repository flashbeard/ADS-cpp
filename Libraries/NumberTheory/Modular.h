#ifndef TEST_MODULAR_H
#define TEST_MODULAR_H

#include <type_traits>
#include <iostream>

namespace nt {

    template<class T>
    class Modular {
    public:
        static_assert(std::is_integral_v<T>, "Modular number must be an integer");
        static_assert(!std::is_same_v<std::remove_cv_t<T>, bool>, "Modular number must not be a bool");

        // FIX_ME: - % does not work fine for all test cases we want (e.g. negative values)
        // Can we make % operator faster?
        constexpr Modular(T t_value, T t_modulo) : value(t_value % t_modulo), modulo(t_modulo) {}

        template<class U>
        Modular &operator=(const U &other) {
            if constexpr(std::is_same_v<std::remove_cv_t<U>, Modular>) {
                if (this != &other) {
                    value = other.value;
                    modulo = other.modulo;
                }
            } else {
                value = other;
                value %= modulo;
            }

            return *this;
        }

        friend std::ostream &operator<<(std::ostream &out, const Modular m) {
            out << m.value;
            return out;
        }

    private:
        T value, modulo;
    };

}


#endif //TEST_MODULAR_H
