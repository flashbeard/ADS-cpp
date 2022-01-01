/*
 * Created by flashbeard.
 * number-theory
 */

#ifndef ADS_CPP_NT_TYPE_TRAITS_H
#define ADS_CPP_NT_TYPE_TRAITS_H

namespace nt {

    template<typename>
    struct __is_integral_helper : public std::false_type {
    };

    template<>
    struct __is_integral_helper<char> : public std::true_type {
    };

    template<>
    struct __is_integral_helper<signed char> : public std::true_type {
    };

    template<>
    struct __is_integral_helper<unsigned char> : public std::true_type {
    };

#ifdef _GLIBCXX_USE_WCHAR_T
    template<>
    struct __is_integral_helper<wchar_t> : public std::true_type {
    };
#endif

    template<>
    struct __is_integral_helper<char16_t> : public std::true_type {
    };

    template<>
    struct __is_integral_helper<char32_t> : public std::true_type {
    };

    template<>
    struct __is_integral_helper<short> : public std::true_type {
    };

    template<>
    struct __is_integral_helper<unsigned short> : public std::true_type {
    };

    template<>
    struct __is_integral_helper<int> : public std::true_type {
    };

    template<>
    struct __is_integral_helper<unsigned int> : public std::true_type {
    };

    template<>
    struct __is_integral_helper<long> : public std::true_type {
    };

    template<>
    struct __is_integral_helper<unsigned long> : public std::true_type {
    };

    template<>
    struct __is_integral_helper<long long> : public std::true_type {
    };

    template<>
    struct __is_integral_helper<unsigned long long> : public std::true_type {
    };

    template<typename T>
    struct is_integral : public __is_integral_helper<typename std::remove_cv<T>::type>::type {
    };

    template<typename T>
    inline constexpr bool is_integral_v = is_integral<T>::value;

}

#endif //ADS_CPP_NT_TYPE_TRAITS_H
