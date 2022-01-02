/*
 * Created by flashbeard.
 * number-theory
 */

#include <iostream>
#include "Everything.h"
#include <cassert>

int main() {
//    int a = 5, n = 3;
//    auto result = nt::power(a, n);
//    std::cout << typeid(result).name() << ' ' << result << std::endl;

//    float a = 5, n = 3;
//    auto result = nt::power(a, n);
//    std::cout << typeid(result).name() << ' ' << result << std::endl;
//
//    long a = 5;
//    short n = 3;
//    auto result = nt::power(a, n);
//    std::cout << typeid(result).name() << ' ' << result << std::endl;
//
    constexpr int md = 13;
    using Mint = nt::Modular<md>;

//    Mint a = 5;
//    int n = 3;
//
//    auto res = nt::power(a, n);
//
//    std::cout << typeid(res).name() << std::endl; // N2nt7ModularILi13EEE

//    Mint a = 6, b = 9, c = a + b;
//    std::cout << typeid(c).name() << ' ' << (*(a + b) == *(a + 9)) << std::endl;

//    std::cout << nt::power(static_cast<Mint>(5), 2) << std::endl; // 12 - true
//    std::cout << static_cast<Mint>(5) / static_cast<Mint>(2) << std::endl; // 9 - true

    std::cout << nt::gcd(16, 8, 24, 20) << ' ' << nt::lcm(16, 8, 24, 20) << std::endl; // 4 240 - true
}