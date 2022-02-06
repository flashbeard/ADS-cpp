/*
 * Created by flashbeard.
 * number-theory
 */

#include <iostream>
#include <cassert>
#include <chrono>

#include "Everything.h"

typedef nt::BigInteger Bint;

int main() {
    Bint a = 5;
    std::cout << a << std::endl;
    std::cout << nt::power(a, 1000) << std::endl;
}