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

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator T() const IPADDRESS_NOEXCEPT {
        return T(low);
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
    
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator/(T l) const IPADDRESS_NOEXCEPT {
        return *this / uint128_t(l);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator/(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return divide(other);
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator%(T l) const IPADDRESS_NOEXCEPT {
        return *this % uint128_t(l);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator%(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        const auto result = divide(other);
        return *this - result * other;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator&(T l) const IPADDRESS_NOEXCEPT {
        return *this & uint128_t(l);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator&(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return { high & other.high, low & other.low };
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator|(T l) const IPADDRESS_NOEXCEPT {
        return *this | uint128_t(l);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator|(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return { high | other.high, low | other.low };
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator^(T l) const IPADDRESS_NOEXCEPT {
        return *this ^ uint128_t(l);
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
        if (const auto result = high <=> other.high; result == std::strong_ordering::equivalent) {
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

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t divide(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        if (other.high == 0) {
            if (other.low == 0) {
                return {};
            }

            if (high == 0) {
                return low / other.low;
            }
        }

        if (other >= *this) {
            return other == *this ? 1 : 0;
        }

        auto quotient = *this;
        auto divisor = other;

        uint32_t left[4] {
            uint32_t(quotient.low),
            uint32_t(quotient.low >> 32),
            uint32_t(quotient.high),
            uint32_t(quotient.high >> 32)
        };
        auto a = quotient.lez();
        int32_t left_size = 4 - int32_t(quotient.lez()) / 32;

        uint32_t right[4] {
            uint32_t(divisor.low),
            uint32_t(divisor.low >> 32),
            uint32_t(divisor.high),
            uint32_t(divisor.high >> 32)
        };
        auto b = divisor.lez();
        int32_t right_size = 4 - int32_t(divisor.lez()) / 32;

        uint32_t bits[4]{};
        int32_t bits_size = left_size - right_size + 1;

        assert(left_size >= 1);
        assert(right_size >= 1);
        assert(left_size >= right_size);

        uint32_t div_hi = right[right_size - 1];
        uint32_t div_lo = right_size > 1 ? right[right_size - 2] : 0;

        int32_t shift = 32;
        if (div_hi != 0) {
            for (shift = 0; shift < 32; ++shift) {
                if (div_hi & (1ULL << (31 - shift))) {
                    break;
                }
            }
        }
        int32_t back_shift = 32 - shift;

        if (shift > 0) {
            uint32_t div_nx = right_size > 2 ? right[right_size - 3] : 0;
            div_hi = (div_hi << shift) | (div_lo >> back_shift);
            div_lo = (div_lo << shift) | (div_nx >> back_shift);
        }

        for (int32_t i = left_size; i >= right_size; --i) {
            int32_t n = i - right_size;
            uint32_t t = uint32_t(i) < uint32_t(left_size) ? left[i] : 0;

            uint64_t val_hi = (uint64_t(t) << 32) | left[i - 1];
            uint32_t val_lo = (i > 1) ? left[i - 2] : 0;

            if (shift > 0) {
                uint32_t val_nx = i > 2 ? left[i - 3] : 0;
                val_hi = (val_hi << shift) | (val_lo >> back_shift);
                val_lo = (val_lo << shift) | (val_nx >> back_shift);
            }

            uint64_t digit = val_hi / div_hi;

            if (digit > 0xFFFFFFFF) {
                digit = 0xFFFFFFFF;
            }

            while (divide_guess_too_big(digit, val_hi, val_lo, div_hi, div_lo)) {
                --digit;
            }
            
            if (digit > 0) {
                uint32_t carry = subtract_divisor(left + n, left_size - n, right, right_size, digit);

                if (carry != t) {
                    assert(carry == t + 1);

                    carry = add_divisor(left + n, left_size - n, right, right_size);

                    --digit;
                    assert(carry == 1);
                }
            }

            if (uint32_t(n) < uint32_t(bits_size)) {
                bits[n] = uint32_t(digit);
            }

            if (uint32_t(i) < uint32_t(left_size)) {
                left[i] = 0;
            }
        }

        return { uint64_t(bits[3]) << 32 | bits[2], uint64_t(bits[1]) << 32 | bits[0] };
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t lez() const IPADDRESS_NOEXCEPT {
        if (high != 0) {
            for (uint32_t i = 0; i < 64; ++i) {
                if (high & (1ULL << (63 - i))) {
                    return i;
                }
            }
        }
        if (low != 0) {
            for (uint32_t i = 0; i < 64; ++i) {
                if (low & (1ULL << (63 - i))) {
                    return 64 + i;
                }
            }
        }
        return 128;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool divide_guess_too_big(uint64_t q, uint64_t val_hi, uint32_t val_lo, uint32_t div_hi, uint32_t div_lo) IPADDRESS_NOEXCEPT {
        assert(q <= 0xFFFFFFFF);

        uint64_t chk_hi = div_hi * q;
        uint64_t chk_lo = div_lo * q;

        chk_hi += chk_lo >> 32;
        chk_lo = uint32_t(chk_lo);

        return (chk_hi > val_hi) || (chk_hi == val_hi && chk_lo > val_lo);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t subtract_divisor(uint32_t* left, size_t ls, uint32_t (&right)[4], size_t rs, uint64_t q) IPADDRESS_NOEXCEPT {
        assert(ls >= rs);
        assert(q <= 0xFFFFFFFF);

        uint64_t carry = 0;

        for (int32_t i = 0; i < rs; ++i) {
            carry += right[i] * q;

            uint32_t digit = uint32_t(carry);
            carry >>= 32;

            uint32_t& left_item = left[i];

            if (left_item < digit) {
                ++carry;
            }
            left_item -= digit;
        }

        return uint32_t(carry);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t add_divisor(uint32_t* left, size_t ls, uint32_t (&right)[4], size_t rs) IPADDRESS_NOEXCEPT {
        assert(ls >= rs);

        uint64_t carry = 0;

        for (int32_t i = 0; i < rs; ++i) {
            uint32_t& left_element = left[i];
            uint64_t digit = left_element + carry + right[i];

            left_element = uint32_t(digit);
            carry = digit >> 32;
        }

        return uint32_t(carry);
    }
};

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(l) + value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(l) - value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator*(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(l) * value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator/(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(l) / value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator%(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(l) % value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator&(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(l) & value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator|(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(l) | value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator^(T l, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(l) ^ value;
}

} // IPADDRESS_NAMESPACE

#endif // IPADDRESS_UINT128_HPP
