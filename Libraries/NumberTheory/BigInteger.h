#pragma once

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

namespace nt {
class BigInteger;
}

// TODO: check is it works on other compilers
namespace std {
inline nt::BigInteger abs(const nt::BigInteger &x);

template<>
struct __is_integral_helper<nt::BigInteger> : public std::true_type
{
};
}  // namespace std

namespace nt {

class BigInteger
{
public:
    constexpr static short BASE_DIGITS = 9;
    constexpr static int BASE = 1e9;

    // TODO: refactor class methods
    // TODO: create sqrt overload

#pragma region Constructors

    BigInteger()
        : m_is_negative{false}
    {
    }

    BigInteger(int64_t x)
    {
        *this = x;
    }

    BigInteger(const std::string &s)
    {
        *this = s;
    }

    BigInteger(const BigInteger &other) noexcept
        : m_is_negative{other.m_is_negative}
        , m_value{other.m_value} {};

    BigInteger(BigInteger &&other) noexcept
        : m_is_negative{other.m_is_negative}
        , m_value{std::move(other.m_value)} {};

    BigInteger &operator=(const BigInteger &other) noexcept
    {
        m_is_negative = other.m_is_negative;
        m_value = other.m_value;
        return *this;
    }

    BigInteger &operator=(BigInteger &&other) noexcept
    {
        m_is_negative = other.m_is_negative;
        m_value = std::move(other.m_value);
        return *this;
    }

    BigInteger &operator=(const std::string &s)
    {
        __internal_read(s);
        return *this;
    }

    BigInteger &operator=(int64_t x) noexcept
    {
        m_is_negative = false;
        m_value.clear();

        if (x < 0) {
            m_is_negative = true;
            x = -x;
        }

        while (x) {
            m_value.push_back(static_cast<int>(x % BASE));
            x /= BASE;
        }

        if (m_value.empty()) {
            m_value.push_back(0);
        }

        return *this;
    }

#pragma endregion Constructors

#pragma region ComparisonOperators

    bool operator<(const BigInteger &other) const noexcept
    {
        if (m_is_negative != other.m_is_negative) {
            return m_is_negative > other.m_is_negative;
        }
        if (m_value.size() != other.m_value.size()) {
            int reverse_order = -1 * m_is_negative;
            return m_value.size() * reverse_order < other.m_value.size() * reverse_order;
        }
        for (int i = static_cast<int>(m_value.size()) - 1; i >= 0; --i) {
            if (m_value[i] != other.m_value[i]) {
                return m_value[i] < other.m_value[i];
            }
        }
        return false;
    }

    bool operator>(const BigInteger &other) const noexcept
    {
        return other < *this;
    }

    bool operator<=(const BigInteger &other) const noexcept
    {
        return !(other < *this);
    }

    bool operator>=(const BigInteger &other) const noexcept
    {
        return !(*this < other);
    }

    bool operator==(const BigInteger &other) const noexcept
    {
        return !(*this < other) && !(other < *this);
    }

    bool operator!=(const BigInteger &other) const noexcept
    {
        return *this < other || other < *this;
    }

#pragma endregion ComparisonOperators

#pragma region ArithmeticAssignOperators

    BigInteger &operator+=(const BigInteger &other) noexcept
    {
        if (m_is_negative == other.m_is_negative) {
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

    BigInteger &operator-=(const BigInteger &other) noexcept
    {
        if (m_is_negative == other.m_is_negative) {
            if (__compare_abs(*this, other) >= 0) {
                __internal_sub(other);
            } else {
                BigInteger temp = other;
                std::swap(*this, temp);
                __internal_sub(temp);
                m_is_negative = !m_is_negative;
            }
        } else {
            __internal_add(other);
        }
        return *this;
    }

    void operator*=(int64_t x)
    {
        if (llabs(x) >= BASE) {
            *this *= BigInteger(x);
            return;
        }

        if (x < 0) {
            m_is_negative = !m_is_negative;
            x = -x;
        }

        int carry = 0;
        for (size_t i = 0; i < m_value.size() || carry; ++i) {
            if (i == m_value.size()) {
                m_value.push_back(0);
            }
            long long cur = static_cast<long long>(m_value[i]) * x + carry;
            carry = static_cast<int>(cur / BASE);
            m_value[i] = static_cast<int>(cur % BASE);
        }
        trim();
    }

    void operator*=(const BigInteger &v)
    {
        *this = *this * v;
    }

    void operator/=(int64_t x)
    {
        if (x <= 0) {
            throw std::invalid_argument("Operator / not well-defined for x <= 0.");
        }

        if (llabs(x) >= BASE) {
            *this /= BigInteger(x);
            return;
        }

        if (x < 0) {
            m_is_negative = !m_is_negative;
            x = -x;
        }

        for (int i = static_cast<int>(m_value.size()) - 1, rem = 0; i >= 0; --i) {
            int64_t cur = m_value[i] + rem * BASE;
            m_value[i] = static_cast<int>(cur / x);
            rem = static_cast<int>(cur % x);
        }
        trim();
    }

    void operator/=(const BigInteger &x)
    {
        *this = *this / x;
    }

    void operator%=(const BigInteger &x)
    {
        *this = *this % x;
    }

    void operator%=(int64_t x)
    {
        // TODO: create definition
    }

#pragma endregion ArithmeticAssignOperators

#pragma region ArithmeticOperators

    template<typename L, typename R>
    typename std::enable_if_t<
        std::is_convertible_v<L, BigInteger> &&
            std::is_convertible_v<R, BigInteger> &&
            std::is_lvalue_reference_v<R &&>,
        BigInteger> friend
        operator+(L &&l, R &&r)
    {
        BigInteger result(std::forward<L>(l));
        result += r;
        return result;
    }

    template<typename L, typename R>
    typename std::enable_if_t<
        std::is_convertible_v<L, BigInteger> &&
            std::is_convertible_v<R, BigInteger> &&
            std::is_rvalue_reference_v<R &&>,
        BigInteger> friend
        operator+(L &&l, R &&r)
    {
        BigInteger result(std::move(r));
        result += l;
        return result;
    }

    template<typename L, typename R>
    typename std::enable_if_t<
        std::is_convertible_v<L, BigInteger> &&
            std::is_convertible_v<R, BigInteger>,
        BigInteger> friend
        operator-(L &&l, R &&r)
    {
        BigInteger result(std::forward<L>(l));
        result -= r;
        return result;
    }

    BigInteger operator/(int64_t x) const
    {
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

    BigInteger operator/(const BigInteger &x) const
    {
        return __divmod(*this, x).first;
    }

    int64_t operator%(int64_t x) const
    {
        if (x <= 0) {
            throw std::invalid_argument("Operator % not well-defined for x <= 0.");
        }

        if (x >= BASE) {
            throw std::invalid_argument("Operator % not well-defined for x >= BASE constant.");
        }

        int64_t result = 0;
        for (int i = static_cast<int>(m_value.size()) - 1; i >= 0; --i) {
            result = (m_value[i] + result * BASE) % x;
        }

        return result * m_is_negative;
    }

    BigInteger operator%(const BigInteger &x) const
    {
        return __divmod(*this, x).second;
    }

    BigInteger operator*(int64_t x) const
    {
        if (llabs(x) >= BASE) {
            return *this * BigInteger(x);
        }
        auto res = *this;
        res *= x;
        return res;
    }

    BigInteger operator*(const BigInteger &v) const
    {
        return __simple_multiply(*this, v);
        // TODO: 2 types of multiplication
        //            return {};
    }

#pragma endregion ArithmeticOperators

#pragma region UnaryOperators

    BigInteger &operator++()
    {
        *this += 1;
        return *this;
    }

    BigInteger &operator--()
    {
        *this -= 1;
        return *this;
    }

    BigInteger operator++(int)
    {
        BigInteger result(*this);
        *this += 1;
        return result;
    }

    BigInteger operator--(int)
    {
        BigInteger result(*this);
        *this -= 1;
        return result;
    }

    BigInteger operator-() const noexcept
    {
        auto t = *this;

        if (t.is_zero()) {
            return t;
        }

        t.m_is_negative = !m_is_negative;
        return t;
    }

#pragma endregion UnaryOperators

#pragma region IO

    friend std::istream &operator>>(std::istream &in, BigInteger &x)
    {
        std::string s;
        in >> s;
        x.__internal_read(s);
        return in;
    }

    friend std::ostream &operator<<(std::ostream &out, const BigInteger &x)
    {
        return x.__print(out);
        ;
    }

#pragma endregion IO

    friend BigInteger std::abs(const BigInteger &x);

private:
#pragma region MiscFunctions

    /*!
         * @retval  1 if abs(x) > abs(y)
         * @retval  0 if abs(x) == abs(y)
         * @retval -1 if abs(x) \< abs(y)
         */
    friend int __compare_abs(const BigInteger &x, const BigInteger &y)
    {
        if (x.m_value.size() != y.m_value.size()) {
            return x.m_value.size() < y.m_value.size() ? -1 : 1;
        }
        for (int i = static_cast<int>(x.size()) - 1; i >= 0; --i) {
            if (x.m_value[i] != y.m_value[i]) {
                return x.m_value[i] < y.m_value[i] ? -1 : 1;
            }
        }
        return 0;
    }

    // works only with absolute values
    void __internal_add(const BigInteger &x) noexcept
    {
        for (size_t i = 0, carry = 0; i < std::max((*this).size(), x.size()) || carry; ++i) {
            if (i == m_value.size()) {
                m_value.push_back(0);
            }
            m_value[i] += static_cast<int>(carry) + (i < x.size() ? x.m_value[i] : 0);
            carry = m_value[i] >= BASE;
            if (carry) {
                m_value[i] -= BASE;
            }
        }
    }

    // works only with absolute values
    void __internal_sub(const BigInteger &x) noexcept
    {
        for (size_t i = 0, carry = 0; i < x.size() || carry; ++i) {
            m_value[i] -= static_cast<int>(carry) + (i < x.m_value.size() ? x.m_value[i] : 0);
            carry = m_value[i] < 0;
            if (carry) {
                m_value[i] += BASE;
            }
        }
        trim();
    }

    void __internal_read(const std::string &s)
    {
        m_is_negative = false;
        m_value.clear();

        int beginIndex = 0, endIndex = static_cast<int>(s.size()) - 1;
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == '-') {
                m_is_negative = true;
            } else if (s[i] != '+' && s[i] != '0' || i + 1 == s.size()) {
                beginIndex = static_cast<int>(i);
                break;
            }
        }

        for (int i = endIndex; i >= beginIndex; i -= BASE_DIGITS) {
            int res = 0;
            for (size_t j = std::max(beginIndex, i - BASE_DIGITS + 1); j <= i; ++j) {
                if (s[j] < '0' || '9' < s[j]) {
                    std::stringstream ss;
                    ss << "BigInteger string representation contain incorrect character {" << s[j]
                       << "} at index {" << j << "}.";
                    throw std::runtime_error(ss.str());
                }
                res = res * 10 + s[j] - '0';
            }
            m_value.push_back(res);
        }

        trim();
    }

    std::ostream &__print(std::ostream &out) const noexcept
    {
        if (is_zero()) {
            out << 0;
        } else {
            if (m_is_negative) {
                out << '-';
            }
            out << m_value.back();
            out << std::setfill('0');
            for (int i = static_cast<int>(m_value.size()) - 2; i >= 0; --i) {
                out << std::setw(BASE_DIGITS) << m_value[i];
            }
        }
        return out;
    }

    friend BigInteger __simple_multiply(const BigInteger &a, const BigInteger &b)
    {
        BigInteger result;
        result.m_is_negative = a.m_is_negative | b.m_is_negative;
        result.resize(a.size() + b.size());
        for (size_t i = 0; i < a.size(); ++i) {
            if (a[i]) {
                int carry = 0;
                for (size_t j = 0; j < b.size() || carry; ++j) {
                    long long cur = result[i + j] +
                                    static_cast<long long>(a[i]) * (j < static_cast<int>(b.size()) ? b[j] : 0) +
                                    carry;
                    carry = static_cast<int>(cur / BASE);
                    result[i + j] = static_cast<int>(cur % BASE);
                }
            }
        }
        result.trim();
        return result;
    }

    friend std::pair<BigInteger, BigInteger> __divmod(const BigInteger &_a, const BigInteger &_b)
    {
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

        q.m_is_negative = _a.m_is_negative | _b.m_is_negative;
        q.trim();

        r.m_is_negative = _a.m_is_negative;
        r.trim();

        auto res = std::make_pair(q, r / norm);
        if (res.second < 0) {
            res.second += _b;
        }

        return res;
    }

    void trim() noexcept
    {
        while (m_value.size() > 1 && m_value.back() == 0) {
            m_value.pop_back();
        }
    }

    [[nodiscard]] BigInteger abs() const noexcept
    {
        auto result = *this;
        result.m_is_negative = false;
        return result;
    }

    [[nodiscard]] bool is_zero() const noexcept
    {
        return m_value.empty() || m_value.size() == 1 && m_value[0] == 0;
    }

    void resize(const size_t &n)
    {
        m_value.resize(n);
    }

    [[nodiscard]] size_t size() const noexcept
    {
        return m_value.size();
    }

    int &back() noexcept
    {
        return m_value.back();
    }

    [[nodiscard]] const int &back() const noexcept
    {
        return m_value.back();
    }

    int &operator[](size_t i)
    {
        return m_value[i];
    }

    const int &operator[](size_t i) const
    {
        return m_value[i];
    }

#pragma endregion MiscFunctions

    std::vector<int> m_value;
    bool m_is_negative;
};
}  // namespace nt

namespace std {
inline nt::BigInteger abs(const nt::BigInteger &x)
{
    return x.abs();
}
}  // namespace std
