#ifndef TEST_MODULAR_H
#define TEST_MODULAR_H

#include <type_traits>

namespace nt {

    template<class T>
    class Modular {
    public:
        using Type = typename std::decay<decltype(T::value)>::type;

        static_assert(std::is_integral_v<Type>, "Modular mod is integer");
        static_assert(!std::is_same_v<std::remove_cv_t<Type>, bool>, "Modular mod is not bool");

        constexpr explicit Modular(Type mod) : m_value(), m_mod(mod) {}

    private:
        Type m_value, m_mod;
    };

}


#endif //TEST_MODULAR_H
