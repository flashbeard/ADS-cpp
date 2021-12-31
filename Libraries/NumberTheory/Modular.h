/*
 * Created by flashbeard.
 * On 31.12.2021
 * number-theory
 */

#ifndef TEST_MODULAR_H
#define TEST_MODULAR_H

#include <type_traits>
#include <iostream>

namespace nt {

    template<class ModularType>
    class Modular {
    public:
        static_assert(std::is_integral_v<ModularType>, "Modular number must be an integer type");
        static_assert(!std::is_same_v<std::remove_cv_t<ModularType>, bool>, "Modular number must not be a boolean tupe");

        // FIX_ME: - % does not work fine for all test cases we want (e.g. negative values)
        // Can we make % operator faster?
        constexpr Modular(ModularType t_value, ModularType t_modulo) : value(t_value % t_modulo), modulo(t_modulo) {}

        template<class T, class U, class V = std::common_type<T, U>>
        V mod(T n, U m) {
            static_assert(std::is_integral_v<V>, "mod arguments must be integer types");
            static_assert(!std::is_same_v<std::remove_cv_t<V>, bool>, "mod arguments must not be boolean types");
            static_assert(m > 0, "modulo must be positive");

            if (n >= 0) {
                return n % m;
            } else {
                // -5 mod 4 = 3, so (-2 * 4) + 3 = -8 + 3 = -5
                return (m - (-n % m)) % m;
            }
        }

        void normalize() {
            this->value = mod(this->value, this->modulo);
        }

        template<class T>
        Modular &operator=(const T &other) {
            if constexpr(std::is_same_v<std::remove_cv_t<T>, Modular>) {
                if (this != &other) {
                    value = other.value;
                    modulo = other.modulo;
                }
            } else {
                value = other;
                normalize();
            }

            return *this;
        }

        template<class T>
        Modular operator+(const T &other) {
            Modular result = this;
            if constexpr(std::is_same_v<std::remove_cv_t<T>, Modular>) {
                static_assert(modulo == other->modulo, "modules must be equal while addition of two Modular");
                result.value += other.value;
                if (result.value >= result.modulo) {
                    result.value -= result.modulo;
                }
            } else {
                result.value += other;
                result.normalize();
            }

            return result;
        }

        template<class T>
        Modular operator-(const T &other) {
            Modular result = this;
            if constexpr(std::is_same_v<std::remove_cv_t<T>, Modular>) {
                static_assert(modulo == other->modulo, "modules must be equal while subtracting of two Modular");
                result.value -= other.value;
            } else {
                result.value -= other;
            }

            result.normalize();
            return result;
        }

        template<class T>
        Modular operator*(const T &other) {
            ModularType a = this->value;
            ModularType b;
            if constexpr(std::is_same_v<std::remove_cv_t<T>, Modular>) {
                static_assert(modulo == other->modulo, "modules must be equal while multiplication of two Modular");
                b = other.value;
            } else {
                b = other;
            }

            a = mod(a, this->modulo);
            b = mod(b, this->modulo);

            Modular result = this;
            result.value = 0;

            while (b) {
                if(b & 1) {
                    result.value += a;
                    result.value = mod(result.value, this->modulo);
                }
                b >>= 1;
                if(a < this->modulo - a) {
                    a <<= 1;
                } else {
                    a -= this->modulo - a;
                }
            }
            return result;
        }

        friend std::ostream &operator<<(std::ostream &out, const Modular m) {
            out << m.value;
            return out;
        }

    private:
        ModularType value, modulo;
    };

}


#endif //TEST_MODULAR_H
