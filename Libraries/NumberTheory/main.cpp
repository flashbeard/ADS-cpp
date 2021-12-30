#include <iostream>
#include "Everything.h"

int main() {
//    int a = 5, n = 3;
//    auto result = nt::binpow(a, n);
//    std::cout << typeid(result).name() << ' ' << result << std::endl;

//    float a = 5, n = 3;
//    auto result = nt::binpow(a, n);
//    std::cout << typeid(result).name() << ' ' << result << std::endl;

    long a = 5;
    short n = 3;
    auto result = nt::binpow(a, n);
    std::cout << typeid(result).name() << ' ' << result << std::endl;
}