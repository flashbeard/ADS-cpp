/*
 * Created by flashbeard.
 * number-theory
 */

#ifndef ADS_CPP_BIGINTEGER_H
#define ADS_CPP_BIGINTEGER_H

#include <type_traits>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <utility>
#include <cassert>
#include <stdexcept>
#include <iomanip>

#include "nt_type_traits.h"

namespace nt {
    class BigInteger;
}

// TODO: check is it works on other compilers
namespace std {
    inline nt::BigInteger abs(const nt::BigInteger &x);
}

namespace nt {

    class BigInteger {
    public:
        constexpr static short BASE_DIGITS = 9;
        constexpr static int BASE = 1e9;

        // TODO: refactor class methods
        // TODO: create sqrt overload

#pragma region Constructors

        BigInteger() : is_negative(false) {}

        BigInteger(int64_t x) {
            *this = x;
        }

        BigInteger(const std::string &x) {
            std::stringstream ss(x);
            ss >> *this;
        }

        BigInteger(const BigInteger &other) noexcept: val(other.val), is_negative(other.is_negative) {}

        BigInteger(BigInteger &&other) noexcept: val(std::move(other.val)), is_negative(other.is_negative) {}

        BigInteger &operator=(const BigInteger &other) noexcept {
            val = other.val;
            is_negative = other.is_negative;
            return *this;
        }

        BigInteger &operator=(BigInteger &&other) noexcept {
            val = std::move(other.val);
            is_negative = other.is_negative;
            return *this;
        }

        BigInteger &operator=(int64_t x) noexcept {
            is_negative = false;
            val.clear();

            if (x < 0) {
                is_negative = true;
                x = -x;
            }

            while (x) {
                val.push_back(x % BASE);
                x /= BASE;
            }

            if (val.empty()) {
                val.push_back(0);
            }

            return *this;
        }

#pragma endregion Constructors

#pragma region ComparisonOperators

        bool operator<(const BigInteger &other) const noexcept {
            if (is_negative != other.is_negative) {
                return is_negative > other.is_negative;
            }
            if (val.size() != other.val.size()) {
                int reverse_order = -1 * is_negative;
                return val.size() * reverse_order < other.val.size() * reverse_order;
            }
            for (int i = static_cast<int>(val.size()) - 1; i >= 0; --i) {
                if (val[i] != other.val[i]) {
                    return val[i] < other.val[i];
                }
            }
            return false;
        }

        bool operator>(const BigInteger &other) const noexcept {
            return other < *this;
        }

        bool operator<=(const BigInteger &other) const noexcept {
            return !(other < *this);
        }

        bool operator>=(const BigInteger &other) const noexcept {
            return !(*this < other);
        }

        bool operator==(const BigInteger &other) const noexcept {
            return !(*this < other) && !(other < *this);
        }

        bool operator!=(const BigInteger &other) const noexcept {
            return *this < other || other < *this;
        }

#pragma endregion ComparisonOperators

#pragma region IO

        friend std::istream &operator>>(std::istream &in, BigInteger &x) {
            std::string str;
            in >> str;

            x.is_negative = false;
            x.val.clear();

            int beginIndex = 0, endIndex = static_cast<int>(str.size()) - 1;
            for (size_t i = 0; i < str.size(); ++i) {
                if (str[i] == '-') {
                    x.is_negative = !x.is_negative;
                } else if (str[i] != '+' && str[i] != '0' || i + 1 == str.size()) {
                    beginIndex = static_cast<int>(i);
                    break;
                }
            }

            for (int i = endIndex; i >= beginIndex; i -= BASE_DIGITS) {
                int res = 0;
                for (size_t j = std::max(beginIndex, i - BASE_DIGITS + 1); j <= i; ++j) {
                    if (str[j] < '0' || '9' < str[j]) {
                        std::stringstream ss;
                        ss << "BigInteger string representation contain incorrect character {" << str[j]
                           << "} at index {" << j << "}.";
                        throw std::runtime_error(ss.str());
                    }
                    res = res * 10 + str[j] - '0';
                }
                x.val.push_back(res);
            }

            x.trim();
            return in;
        }

        friend std::ostream &operator<<(std::ostream &out, const BigInteger &x) {
            if (x.is_zero()) {
                out << 0;
            } else {
                if (x.is_negative && !x.is_zero()) {
                    out << '-';
                }
                out << x.back();
                out << std::setfill('0');
                for (int i = static_cast<int>(x.size()) - 2; i >= 0; --i) {
                    out << std::setw(BASE_DIGITS) << x[i];
                }
            }
            return out;
        }

#pragma endregion IO

        BigInteger operator-() const noexcept {
            auto t = *this;

            if (t.is_zero()) {
                return t;
            }

            t.is_negative = !is_negative;
            return t;
        }

        BigInteger operator+=(const BigInteger &other) noexcept {
            if (is_negative == other.is_negative) {
                __internal_add(other);
            } else {
                if (__compare_abs(*this, other) >= 0) {
                    __internal_sub(other);
                } else {
                    auto temp = other;
                    std::swap(*this, temp);
                    __internal_sub(temp);
                }
            }
            return *this;
        }

        BigInteger operator-=(const BigInteger &other) noexcept {
            if (is_negative == other.is_negative) {
                if (__compare_abs(*this, other) >= 0) {
                    __internal_sub(other);
                } else {
                    BigInteger temp = other;
                    std::swap(*this, temp);
                    __internal_sub(temp);
                    is_negative = !is_negative;
                }
            } else {
                __internal_add(other);
            }
            return *this;
        }

        template<typename L, typename R>
        typename std::enable_if_t<
                std::is_convertible_v<L, BigInteger> &&
                std::is_convertible_v<R, BigInteger> &&
                std::is_lvalue_reference_v<R &&>,
                BigInteger> friend operator+(L &&l, R &&r) {
            BigInteger result(std::forward<L>(l));
            result += r;
            return result;
        }

        template<typename L, typename R>
        typename std::enable_if_t<
                std::is_convertible_v<L, BigInteger> &&
                std::is_convertible_v<R, BigInteger> &&
                std::is_rvalue_reference_v<R &&>,
                BigInteger> friend operator+(L &&l, R &&r) {
            BigInteger result(std::move(r));
            result += l;
            return result;
        }

        template<typename L, typename R>
        typename std::enable_if_t<
                std::is_convertible_v<L, BigInteger> &&
                std::is_convertible_v<R, BigInteger>,
                BigInteger> friend operator-(L &&l, R &&r) {
            BigInteger result(std::forward<L>(l));
            result -= r;
            return result;
        }

        void operator/=(int64_t x) {
            if (x <= 0) {
                throw std::invalid_argument("Operator / not well-defined for x <= 0.");
            }

            if (llabs(x) >= BASE) {
                *this /= BigInteger(x);
                return;
            }

            if (x < 0) {
                is_negative = !is_negative;
                x = -x;
            }

            for (int i = static_cast<int>(val.size()) - 1, rem = 0; i >= 0; --i) {
                int64_t cur = val[i] + rem * BASE;
                val[i] = static_cast<int64_t>(cur / x);
                rem = static_cast<int>(cur % x);
            }
            trim();
        }

        BigInteger operator/(int64_t x) const {
            if (x <= 0) {
                throw std::invalid_argument("Operator / not well-defined for x <= 0.");
            }

            if (llabs(x) >= BASE) {
                return *this / BigInteger(x);
            }
            auto result = *this;
            result /= x;
            return result;
        }

        void operator/=(const BigInteger &x) {
            *this = *this / x;
        }

        int64_t operator%(int64_t x) const {
            if (x <= 0) {
                throw std::invalid_argument("Operator % not well-defined for x <= 0.");
            }

            if (x >= BASE) {
                throw std::invalid_argument("Operator % not well-defined for x >= BASE constant.");
            }

            int64_t result = 0;
            for (int i = static_cast<int>(val.size()) - 1; i >= 0; --i) {
                result = (val[i] + result * BASE) % x;
            }

            return result * is_negative;
        }

        void operator*=(int64_t x) {
            if (llabs(x) >= BASE) {
                *this *= BigInteger(x);
                return;
            }

            if (x < 0) {
                is_negative = !is_negative;
                x = -x;
            }

            int carry = 0;
            for (size_t i = 0; i < val.size() || carry; ++i) {
                if (i == val.size()) {
                    val.push_back(0);
                }
                long long cur = static_cast<long long>(val[i]) * x + carry;
                carry = static_cast<int>(cur / BASE);
                val[i] = static_cast<int>(cur % BASE);
            }
            trim();
        }

        BigInteger operator*(int64_t x) const {
            if (llabs(x) >= BASE) {
                return *this * BigInteger(x);
            }
            auto res = *this;
            res *= x;
            return res;
        }

        BigInteger operator/(const BigInteger &x) const {
            return __divmod(*this, x).first;
        }

        BigInteger operator%(const BigInteger &x) const {
            return __divmod(*this, x).second;
        }

        void operator*=(const BigInteger &v) {
            *this = *this * v;
        }

        BigInteger operator*(const BigInteger &v) const {
            return __simple_multiply(*this, v);
            // TODO: 2 types of multiplication
//            return {};
        }

        [[nodiscard]] BigInteger abs() const noexcept {
            auto result = *this;
            result.is_negative = false;
            return result;
        }

    private:

#pragma region MiscFunctions

        /*!
         * @retval  1 if abs(x) > abs(y)
         * @retval  0 if abs(x) == abs(y)
         * @retval -1 if abs(x) \< abs(y)
         */
        friend int __compare_abs(const BigInteger &x, const BigInteger &y) {
            if (x.val.size() != y.val.size()) {
                return x.val.size() < y.val.size() ? -1 : 1;
            }
            for (int i = static_cast<int>(x.size()) - 1; i >= 0; --i) {
                if (x.val[i] != y.val[i]) {
                    return x.val[i] < y.val[i] ? -1 : 1;
                }
            }
            return 0;
        }

        // works only with absolute values
        void __internal_add(const BigInteger &x) noexcept {
            for (size_t i = 0, carry = 0; i < std::max((*this).size(), x.size()) || carry; ++i) {
                if (i == val.size()) {
                    val.push_back(0);
                }
                val[i] += static_cast<int>(carry) + (i < x.size() ? x.val[i] : 0);
                carry = val[i] >= BASE;
                if (carry) {
                    val[i] -= BASE;
                }
            }
        }

        // works only with absolute values
        void __internal_sub(const BigInteger &x) noexcept {
            for (size_t i = 0, carry = 0; i < x.size() || carry; ++i) {
                val[i] -= static_cast<int>(carry) + (i < x.val.size() ? x.val[i] : 0);
                carry = val[i] < 0;
                if (carry) {
                    val[i] += BASE;
                }
            }
            trim();
        }

        friend BigInteger __simple_multiply(const BigInteger &a, const BigInteger &b) {
            BigInteger result;
            result.is_negative = a.is_negative | b.is_negative;
            result.resize(a.size() + b.size());
            for (size_t i = 0; i < a.size(); ++i) {
                if (a[i]) {
                    int carry = 0;
                    for (size_t j = 0; j < b.size() || carry; ++j) {
                        long long cur = result[i + j] + static_cast<long long>(a[i]) * (j < static_cast<int>(b.size()) ? b[j] : 0) + carry;
                        carry = static_cast<int>(cur / BASE);
                        result[i + j] = static_cast<int>(cur % BASE);
                    }
                }
            }
            result.trim();
            return result;
        }

        friend std::pair<BigInteger, BigInteger> __divmod(const BigInteger &_a, const BigInteger &_b) {
            if (_b <= 0) {
                throw std::invalid_argument("Operator % is defined only for positive numbers.");
            }

            int64_t norm = BASE / (_b.back() + 1);
            BigInteger a = std::abs(_a) * norm;
            BigInteger b = std::abs(_b) * norm;
            BigInteger q = 0, r = 0;
            q.resize(a.size());

            for (int i = static_cast<int>(b.size()) - 1; i >= 0; --i) {
                r *= BASE;
                r += a[i];
                long long s1 = r.size() <= b.size() ? 0 : r[static_cast<int>(b.size())];
                long long s2 = r.size() <= b.size() - 1 ? 0 : r[static_cast<int>(b.size()) - 1];
                long long d = (static_cast<long long>(BASE) * s1 + s2) / b.back();
                r -= b * d;
                while (r < 0) {
                    r += b, --d;
                }
                q[i] = d;
            }

            q.is_negative = _a.is_negative | _b.is_negative;
            q.trim();

            r.is_negative = _a.is_negative;
            r.trim();

            auto res = std::make_pair(q, r / norm);
            if (res.second < 0) {
                res.second += _b;
            }

            return res;
        }

        void trim() noexcept {
            while (val.size() > 1 && val.back() == 0) {
                val.pop_back();
            }
        }

        [[nodiscard]] bool is_zero() const noexcept {
            return val.empty() || val.size() == 1 && val[0] == 0;
        }

        void resize(const size_t &n) {
            val.resize(n);
        }

        [[nodiscard]] size_t size() const noexcept {
            return val.size();
        }

        int &back() noexcept {
            return val.back();
        }

        [[nodiscard]] const int &back() const noexcept {
            return val.back();
        }

        int &operator[](size_t i) {
            return val[i];
        }

        const int &operator[](size_t i) const {
            return val[i];
        }

#pragma endregion MiscFunctions

        std::vector<int> val;
        bool is_negative;
    };

    template<>
    struct __is_integral_helper<BigInteger> : public std::true_type {
    };

}

namespace std {
    inline nt::BigInteger abs(const nt::BigInteger &x) {
        return x.abs();
    }
}

#endif //ADS_CPP_BIGINTEGER_H
