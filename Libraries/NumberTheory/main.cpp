/*
 * Created by flashbeard.
 * number-theory
 */

#include <iostream>
#include <cassert>
#include <chrono>

#include "Everything.h"

struct wrapper_ctx {
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double, std::milli>> m_begin;

    wrapper_ctx() {
        m_begin = std::chrono::steady_clock::now();
    }

    ~wrapper_ctx() {
        auto end = std::chrono::steady_clock::now();
        auto elapses_mcs = std::chrono::duration_cast<std::chrono::microseconds>(end - m_begin);
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_begin);

        std::cout << "Elapsed time: " << elapses_mcs.count() << "mcs | " << elapsed_ms.count() << "ms\n";
    }
};

template<typename F, typename... Args>
auto executor(F &&f, Args &&... args) -> typename std::result_of<F(Args...)>::type {
    wrapper_ctx ctx;
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

template<typename F>
class wrapper_helper;


template<typename Ret, typename... Args>
class wrapper_helper<std::function<Ret(Args...)>> {
public:
    wrapper_helper(std::function<Ret(Args...)> f) : m_f(f) {}

    Ret operator()(Args... args) const {
        return executor(m_f, args...);
    }

private:
    std::function<Ret(Args...)> m_f;
};

template<typename T>
wrapper_helper<T> wrapper(T f) {
    return wrapper_helper<T>(f);
}

using Bint = nt::BigInteger;

constexpr int mod = 1e9 + 7;
using Mint = nt::Modular<mod>;

Bint testcase_power_biginteger(const Bint &a, int p) {
    return nt::power(a, p);
}

Mint testcase_power_modular(const Mint &a, int p) {
    return nt::power(a, p);
}

long long testcase_power_longlong(const long long &a, int p) {
    return nt::power(a, p);
}

int main() {
    std::function<Bint(Bint, int)> t1 = testcase_power_biginteger;
    auto w1 = wrapper(t1);

    std::function<Mint(Mint, int)> t2 = testcase_power_modular;
    auto w2 = wrapper(t2);

    std::function<long long(long long, int)> t3 = testcase_power_longlong;
    auto w3 = wrapper(t3);

#define TEST(NUM, ARGS...) \
auto r ## NUM = w ## NUM(ARGS);\
std::cout << r ## NUM << std::endl << std::endl;

    TEST(1, 5, 40)
    TEST(2, 5, 40)
    TEST(3, 5, 40)

    // TODO: fix multiply operation for Biginteger

#undef TEST
}