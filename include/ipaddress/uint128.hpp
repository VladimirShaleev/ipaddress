#ifndef IPADDRESS_UINT128_HPP
#define IPADDRESS_UINT128_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

struct uint128_t final {
    uint64_t high{};
    uint64_t low{};

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t() IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(const uint128_t&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(uint128_t&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator=(const uint128_t&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator=(uint128_t&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(uint64_t h, uint64_t l) IPADDRESS_NOEXCEPT : high(h), low(l) {
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(T l) IPADDRESS_NOEXCEPT : low(uint64_t(l)) {
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE operator bool() const IPADDRESS_NOEXCEPT {
        return high || low;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, bool>::type = true>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator T() const IPADDRESS_NOEXCEPT {
        return T(low);
    }

    template <typename T, typename std::enable_if<!std::is_integral<T>::value, bool>::type = true>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator T() const IPADDRESS_NOEXCEPT {
        static_assert(std::is_integral<T>::value, "the operation is only supported with integers");
        return T{};
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator=(const T& l) IPADDRESS_NOEXCEPT {
        high = 0;
        low(l);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator+=(const uint128_t& value) IPADDRESS_NOEXCEPT {
        *this = *this + value;
        return *this;
    }
    
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator-=(const uint128_t& value) IPADDRESS_NOEXCEPT {
        *this = *this - value;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator*=(const uint128_t& value) IPADDRESS_NOEXCEPT {
        *this = *this * value;
        return *this;
    }
    
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator/=(const uint128_t& value) IPADDRESS_NOEXCEPT {
        *this = *this / value;
        return *this;
    }
    
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator%=(const uint128_t& value) IPADDRESS_NOEXCEPT {
        *this = *this % value;
        return *this;
    }
    
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator&=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        high &= other.high;
        low &= other.low;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator|=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        high |= other.high;
        low |= other.low;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator^=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        high ^= other.high;
        low ^= other.low;
        return *this;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator<<=(T value) IPADDRESS_NOEXCEPT {
        *this = *this << value;
        return *this;
    }
    
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator>>=(T value) IPADDRESS_NOEXCEPT {
        *this = *this >> value;
        return *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+() const IPADDRESS_NOEXCEPT {
        return *this;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-() const IPADDRESS_NOEXCEPT {
        return ++(~*this);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator~() const IPADDRESS_NOEXCEPT {
        return { ~high, ~low };
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+(T l) const IPADDRESS_NOEXCEPT {
        return *this + uint128_t(l);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+(const uint128_t& value) const IPADDRESS_NOEXCEPT {
        return { high + value.high + (value.low + low < low ? 1 : 0), value.low + low };
    }
    
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-(T l) const IPADDRESS_NOEXCEPT {
        return *this - uint128_t(l);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-(const uint128_t& value) const IPADDRESS_NOEXCEPT {
        return { high - value.high - (value.low > low ? 1 : 0), low - value.low };
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator*(T l) const IPADDRESS_NOEXCEPT {
        return *this * uint128_t(l);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator*(const uint128_t& value) const IPADDRESS_NOEXCEPT {
        auto high_first = low >> 32;
        auto high_second = low & 0xFFFFFFFF;
        auto low_first = value.low >> 32;
        auto low_second = value.low & 0xFFFFFFFF;

        auto part_1 = high_first * low_first;
        auto part_2 = high_first * low_second;
        auto part_3 = low_first * high_second;
        auto part_4 = low_second * high_second;

        auto tmp_1 = (part_2 & 0xFFFFFFFF) << 32;
        auto tmp_2 = (part_3 & 0xFFFFFFFF) << 32;

        auto carry_1 = tmp_1 + tmp_2 < tmp_1 ? 1 : 0;
        tmp_1 += tmp_2;
        carry_1 += tmp_1 + part_4 < tmp_1 ? 1 : 0;

        auto carry_2 = part_1 + (part_2 >> 32) + (part_3 >> 32);
        return { high * value.low + low * value.high + carry_2 + carry_1, tmp_1 + part_4 };
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator/(const uint128_t& value) const IPADDRESS_NOEXCEPT {
        return divide(value).first;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator%(const uint128_t& value) const IPADDRESS_NOEXCEPT {
        return divide(value).second;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator&(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return { high & other.high, low & other.low };
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator|(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return { high | other.high, low | other.low };
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator^(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return { high ^ other.high, low ^ other.low };
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator<<(T value) const IPADDRESS_NOEXCEPT {
        if (!value) {
            return *this;
        }
        if (value >= 64 && value <= 128) {
            return { low << (value - 64), 0 };
        }
        if (value < 64 && value > 0) {
            return { (high << value) + (low >> (64 - value)), low << value };
        }
        return { 0, 0 };
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator>>(T value) const IPADDRESS_NOEXCEPT {
        if (!value) {
            return *this;
        }
        if (value >= 64 && value <= 128) {
            return { 0, high >> (value - 64) };
        }
        if (value < 64 && value > 0) {
            return { high >> value, (low >> value) + (high << (64 - value)) };
        }
        return { 0, 0 };
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator++() IPADDRESS_NOEXCEPT {
        *this += 1;
        return *this;
    }
    
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator--() IPADDRESS_NOEXCEPT {
        *this -= 1;
        return *this;
    }
    
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t operator++(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t operator--(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!() const IPADDRESS_NOEXCEPT {
        return !high && !low;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator&&(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return (high || low) && (other.high || other.low);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator||(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return (high || low) || (other.high || other.low);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return high == other.high && low == other.low;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        if (const auto result = high <=> other.high; result != std::strong_ordering::equivalent) {
            return low <=> other.low;
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return high < other.high || (high == other.high && low < other.low);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return !(*this < other);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::pair<uint128_t, uint128_t> divide(const uint128_t& value) const IPADDRESS_NOEXCEPT {
        if (*this < value) {
            return { 0, *this };
        }
        if (*this == value) {
            return { 1, 0 };
        }
        auto left = value.lez() - lez();
        auto tmp_1 = value << left;
        auto tmp_2 = *this;

        uint128_t quotient = 0;
        uint128_t zero = 0;

        if (tmp_2 >= value) {
            auto shift = tmp_2.lez() - tmp_1.lez();
            if (shift) {
                tmp_1 >>= shift;
                quotient <<= shift;
                left -= shift;
            }
            if (tmp_2 < tmp_1) {
                tmp_1 >>= 1;
                quotient <<= 1;
                --left;
            }
            tmp_2 -= tmp_1;
            ++quotient;
        }

        if (tmp_2 >= value) {
            auto shift = tmp_2.lez() - tmp_1.lez();
            if (shift) {
                tmp_1 >>= shift;
                quotient <<= shift;
                left -= shift;
            }
            if (tmp_2 < tmp_1) {
                tmp_1 >>= 1;
                quotient <<= 1;
                --left;
            }
            tmp_2 -= tmp_1;
            ++quotient;
        }
        if (tmp_2 >= value) {
            auto shift = tmp_2.lez() - tmp_1.lez();
            if (shift) {
                tmp_1 >>= shift;
                quotient <<= shift;
                left -= shift;
            }
            if (tmp_2 < tmp_1) {
                tmp_1 >>= 1;
                quotient <<= 1;
                --left;
            }
            tmp_2 -= tmp_1;
            ++quotient;
        }
        if (tmp_2 >= value) {
            auto shift = tmp_2.lez() - tmp_1.lez();
            if (shift) {
                tmp_1 >>= shift;
                quotient <<= shift;
                left -= shift;
            }
            if (tmp_2 < tmp_1) {
                tmp_1 >>= 1;
                quotient <<= 1;
                --left;
            }
            tmp_2 -= tmp_1;
            ++quotient;
        }
        if (tmp_2 >= value) {
            auto shift = tmp_2.lez() - tmp_1.lez();
            if (shift) {
                tmp_1 >>= shift;
                quotient <<= shift;
                left -= shift;
            }
            if (tmp_2 < tmp_1) {
                tmp_1 >>= 1;
                quotient <<= 1;
                --left;
            }
            tmp_2 -= tmp_1;
            ++quotient;
        }
        if (tmp_2 >= value) {
            auto shift = tmp_2.lez() - tmp_1.lez();
            if (shift) {
                tmp_1 >>= shift;
                quotient <<= shift;
                left -= shift;
            }
            if (tmp_2 < tmp_1) {
                tmp_1 >>= 1;
                quotient <<= 1;
                --left;
            }
            tmp_2 -= tmp_1;
            ++quotient;
        }
        if (tmp_2 >= value) {
            auto shift = tmp_2.lez() - tmp_1.lez();
            if (shift) {
                tmp_1 >>= shift;
                quotient <<= shift;
                left -= shift;
            }
            if (tmp_2 < tmp_1) {
                tmp_1 >>= 1;
                quotient <<= 1;
                --left;
            }
            tmp_2 -= tmp_1;
            ++quotient;
        }
        if (tmp_2 >= value) {
            auto shift = tmp_2.lez() - tmp_1.lez();
            if (shift) {
                tmp_1 >>= shift;
                quotient <<= shift;
                left -= shift;
            }
            if (tmp_2 < tmp_1) {
                tmp_1 >>= 1;
                quotient <<= 1;
                --left;
            }
            tmp_2 -= tmp_1;
            ++quotient;
        }
        if (tmp_2 >= value) {
            auto shift = tmp_2.lez() - tmp_1.lez();
            if (shift) {
                tmp_1 >>= shift;
                quotient <<= shift;
                left -= shift;
            }
            if (tmp_2 < tmp_1) {
                tmp_1 >>= 1;
                quotient <<= 1;
                --left;
            }
            tmp_2 -= tmp_1;
            ++quotient;
        }
        if (tmp_2 >= value) {
            auto shift = tmp_2.lez() - tmp_1.lez();
            if (shift) {
                tmp_1 >>= shift;
                quotient <<= shift;
                left -= shift;
            }
            if (tmp_2 < tmp_1) {
                tmp_1 >>= 1;
                quotient <<= 1;
                --left;
            }
            tmp_2 -= tmp_1;
            ++quotient;
        }
        // while (tmp_2 >= value) {
        //     auto shift = tmp_2.lez() - tmp_1.lez();
        //     if (shift) {
        //         tmp_1 >>= shift;
        //         quotient <<= shift;
        //         left -= shift;
        //     }
        //     if (tmp_2 < tmp_1) {
        //         tmp_1 >>= 1;
        //         quotient <<= 1;
        //         --left;
        //     }
        //     tmp_2 -= tmp_1;
        //     ++quotient;
        // }
        return { quotient << left, tmp_2 };
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::ptrdiff_t lez() const IPADDRESS_NOEXCEPT {
        if (high != 0) {
            for (size_t i = 0; i < 64; ++i) {
                if (high & (1ULL << i)) {
                    return i;
                }
            }
        }
        if (low != 0) {
            for (size_t i = 0; i < 64; ++i) {
                if (low & (1ULL << i)) {
                    return 64 + i;
                }
            }
        }
        return 128;
    }
};

template <typename T, typename std::enable_if<std::is_integral<T>::value, bool>::type = true>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(l) + value;
}

template <typename T, typename std::enable_if<!std::is_integral<T>::value, bool>::type = true>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    static_assert(std::is_integral<T>::value, "the operation is only supported with integers");
    return {};
}

template <typename T, typename std::enable_if<std::is_integral<T>::value, bool>::type = true>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(l) - value;
}

template <typename T, typename std::enable_if<!std::is_integral<T>::value, bool>::type = true>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    static_assert(std::is_integral<T>::value, "the operation is only supported with integers");
    return {};
}

template <typename T, typename std::enable_if<std::is_integral<T>::value, bool>::type = true>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator*(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(l) * value;
}

template <typename T, typename std::enable_if<!std::is_integral<T>::value, bool>::type = true>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator*(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    static_assert(std::is_integral<T>::value, "the operation is only supported with integers");
    return {};
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator/(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(l) / value;
}

template <typename T, typename std::enable_if<!std::is_integral<T>::value, bool>::type = true>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator/(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    static_assert(std::is_integral<T>::value, "the operation is only supported with integers");
    return {};
}

} // IPADDRESS_NAMESPACE

#endif // IPADDRESS_UINT128_HPP
