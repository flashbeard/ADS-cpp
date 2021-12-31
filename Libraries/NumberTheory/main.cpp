/*
 * Created by flashbeard.
 * On 31.12.2021
 * number-theory
 */

#include <iostream>
#include "Everything.h"

int main() {
//    int a = 5, n = 3;
//    auto result = nt::binpow(a, n);
//    std::cout << typeid(result).name() << ' ' << result << std::endl;

//    float a = 5, n = 3;
//    auto result = nt::binpow(a, n);
//    std::cout << typeid(result).name() << ' ' << result << std::endl;

//    long a = 5;
//    short n = 3;
//    auto result = nt::binpow(a, n);
//    std::cout << typeid(result).name() << ' ' << result << std::endl;

    int mod = 6;
    nt::Modular a(10, mod), b(12, mod);
    std::cout << a << std::endl;
    a = 5;
    std::cout << a << std::endl;
    a = b;
    std::cout << a << std::endl;
}