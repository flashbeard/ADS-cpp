/*
 * Created by flashbeard.
 * number-theory
 */

#include <iostream>
#include "Everything.h"
#include <vector>

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

    Mint a = 5;
    int n = 3;

    auto res = nt::power(a, n);

    std::cout << typeid(res).name() << std::endl; // N2nt7ModularILi13EEE
}