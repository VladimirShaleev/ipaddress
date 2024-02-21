/**
 * \file      uint128.hpp
 * \brief     Unsigned integer type with width of exactly 128 bits.
 * \author    Vladimir Shaleev
 * \copyright MIT License
 * 
 * The library adds its own type `uint128_t`, because it is missing in 
 * the standard. The library does not use compiler extensions such as 
 * `__int128` for greater portability.
 */

#ifndef IPADDRESS_UINT128_HPP
#define IPADDRESS_UINT128_HPP

#include "config.hpp"
#include "hash.hpp"
#include "optional.hpp"

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace IPADDRESS_NAMESPACE {

class uint128_t;

} // namespace IPADDRESS_NAMESPACE

namespace std {

template <>
struct is_integral<IPADDRESS_NAMESPACE::uint128_t> : true_type {
};

template <>
struct is_arithmetic<IPADDRESS_NAMESPACE::uint128_t> : true_type {
};

template <>
struct is_unsigned<IPADDRESS_NAMESPACE::uint128_t> : true_type {
};

template <>
struct is_fundamental<IPADDRESS_NAMESPACE::uint128_t> : true_type {
};

template <>
struct is_scalar<IPADDRESS_NAMESPACE::uint128_t> : true_type {
};

template <>
struct is_object<IPADDRESS_NAMESPACE::uint128_t> : true_type {
};

} // namespace std

#endif // IPADDRESS_NO_OVERLOAD_STD

namespace IPADDRESS_NAMESPACE {

/**
 * 128-bit unsigned integer.
 * 
 * Unsigned integer type with width of exactly 128 bits.
 */
class uint128_t final {
public:
    /**
     * String format
     * 
     * Possible representations of uint128_t in string form.
     */
    enum class format {
        decimal = 0, /**< Decimal format. *Default format.* */
        octal, /**< Octal format */
        hexadecimal, /**< Hexadecimal format */
    };

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t() IPADDRESS_NOEXCEPT = default; /**< Default constructor */

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(const uint128_t&) IPADDRESS_NOEXCEPT = default; /**< Copy constructor */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(uint128_t&&) IPADDRESS_NOEXCEPT = default; /**< Move constructor */

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator=(const uint128_t&) IPADDRESS_NOEXCEPT = default; /**< Assignment operator */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator=(uint128_t&&) IPADDRESS_NOEXCEPT = default; /**< Move assignment operator */

    /**
     * Creates an instance.
     * 
     * Creates an instance from the upper and lower integer.
     * 
     * @param[in] upper integer of value
     * @param[in] lower integer of value
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(uint64_t upper, uint64_t lower) IPADDRESS_NOEXCEPT

#if IPADDRESS_ENDIAN == IPADDRESS_BIG_ENDIAN

        : _upper(upper), _lower(lower)

#else // IPADDRESS_ENDIAN != IPADDRESS_BIG_ENDIAN

        : _lower(lower), _upper(upper)

#endif // IPADDRESS_ENDIAN != IPADDRESS_BIG_ENDIAN
    { }

    /**
     * Creates an instance.
     * 
     * Creates an instance from lower integer.
     * 
     * @tparam T is unsigned integer type
     * @param[in] lower integer of value
     */
    template <typename T, typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value, bool>::type = true>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(T lower) IPADDRESS_NOEXCEPT : _lower(uint64_t(lower)) {
    }

    /**
     * Creates an instance.
     * 
     * Creates an instance from lower integer.
     * 
     * @tparam T is signed integer type
     * @param[in] lower integer of value
     */
    template <typename T, typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value, bool>::type = true>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(T lower) IPADDRESS_NOEXCEPT

#if IPADDRESS_ENDIAN == IPADDRESS_BIG_ENDIAN

        : _upper(uint64_t(int64_t(lower) >> 63)), _lower(uint64_t(lower))

#else // IPADDRESS_ENDIAN != IPADDRESS_BIG_ENDIAN

        : _lower(uint64_t(lower)), _upper(uint64_t(int64_t(lower) >> 63))

#endif // IPADDRESS_ENDIAN != IPADDRESS_BIG_ENDIAN
    { }

    /**
     * Creates an instance.
     * 
     * Creates an instance from float value.
     * 
     * @tparam T is float type
     * @param[in] value of float type
     */
    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, bool>::type = true>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit uint128_t(T value) IPADDRESS_NOEXCEPT {
        const auto result = from_double(double(value));
        _upper = result._upper;
        _lower = result._lower;
    }

    /**
     * Get lower part of uint128_t.
     * 
     * @return Lower part `uint64_t` of uint128_t.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint64_t lower() const IPADDRESS_NOEXCEPT {
        return _lower;
    }

    /**
     * Get upper part of uint128_t.
     * 
     * @return Upper part `uint64_t` of uint128_t.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint64_t upper() const IPADDRESS_NOEXCEPT {
        return _upper;
    }

    /**
     * Calculate hash of uint128_t.
     * 
     * Returns a value of type std::size_t that represents the hash value.
     * 
     * @return Hash of uint128_t.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t hash() const IPADDRESS_NOEXCEPT {
        hash_combine<8> hasher{};
        const auto seed = hasher(_upper);
        const auto hash = hasher(seed + 0x9e3779b9 + _lower);
        return hash;
    }

    /**
     * Swap two uint128_t.
     * 
     * Exchanges the given values.
     * 
     * @param[in,out] other the value to be swapped
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE void swap(IPADDRESS_NAMESPACE::uint128_t& other) IPADDRESS_NOEXCEPT {
        const auto tmp = *this;
        *this = other;
        other = tmp;
    }

    /**
     * Converts value to std::string.
     * 
     * @param[in] fmt of format string
     * @return A string holding the converted value.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(format fmt = format::decimal) const {
        if (_upper == 0) {
            std::ostringstream ss;
            switch (fmt) {
                case format::octal:
                    ss << std::oct;
                    break;
                case format::hexadecimal:
                    ss << std::hex;
                    break;
                default:
                    ss << std::dec;
                    break;
            }
            ss << _lower;
            return ss.str();
        }

        switch (fmt) {
            case format::octal:
                return uint128_to_oct_str(*this);
            case format::hexadecimal:
                return uint128_to_hex_str(*this);
            default:
                return uint128_to_dec_str(*this);
        }
    }

    /**
     * Parse std::string to uint128_t.
     * 
     * @param[in] str of input string
     * @param[in] fmt of format string
     * @return optional containing uint128_t if parsed successfully.
     */
    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE optional<uint128_t> from_string(const std::string& str, format fmt = format::decimal) IPADDRESS_NOEXCEPT {
        switch (fmt) {
            case format::octal:
                return oct_str_to_uint128(str);
            case format::hexadecimal:
                return hex_str_to_uint128(str);
            default:
                return dec_str_to_uint128(str);
        }
    }

    /**
     * Checking that a number is non-zero.
     * 
     * @return `true` if non-zero.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator bool() const IPADDRESS_NOEXCEPT {
        return _upper || _lower;
    }

    /**
     * Cast value to integer type.
     * 
     * @tparam T is integer type
     * @return Integer value.
     */
    template <typename T, typename std::enable_if<std::is_integral<T>::value, bool>::type = true>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator T() const IPADDRESS_NOEXCEPT {
        return T(_lower);
    }

    /**
     * Cast value to float type.
     * 
     * @tparam T is float type
     * @return Float value.
     */
    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, bool>::type = true>
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE explicit operator T() const IPADDRESS_NOEXCEPT {
        return T(to_double(*this));
    }

    /**
     * Assigns lower unsigned integer.
     * 
     * @param[in] lower unsigned integer of value
     * @return This object.
     */
    template <typename T, typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value, bool>::type = true>
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator=(T lower) IPADDRESS_NOEXCEPT {
        _upper = 0;
        _lower = uint64_t(lower);
        return *this;
    }

    /**
     * Assigns lower signed integer.
     * 
     * @param[in] lower signed integer of value
     * @return This object.
     */
    template <typename T, typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value, bool>::type = true>
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator=(T lower) IPADDRESS_NOEXCEPT {
        _upper = uint64_t(int64_t(lower) >> 63);
        _lower = uint64_t(lower);
        return *this;
    }

    /**
     * Addition assignment.
     * 
     * @param[in] other of value
     * @return This object.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator+=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        *this = *this + other;
        return *this;
    }
    
    /**
     * Subtraction assignment.
     * 
     * @param[in] other of value
     * @return This object.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator-=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        *this = *this - other;
        return *this;
    }

    /**
     * Multiplication assignment.
     * 
     * @param[in] other of value
     * @return This object.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator*=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        *this = *this * other;
        return *this;
    }
    
    /**
     * Division assignment.
     * 
     * @param[in] other of value
     * @return This object.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator/=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        *this = *this / other;
        return *this;
    }
    
    /**
     * Remainder assignment.
     * 
     * @param[in] other of value
     * @return This object.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator%=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        *this = *this % other;
        return *this;
    }
    
    /**
     * Bitwise AND assignment.
     * 
     * @param[in] other of value
     * @return This object.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator&=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        _upper &= other._upper;
        _lower &= other._lower;
        return *this;
    }

    /**
     * Bitwise OR assignment.
     * 
     * @param[in] other of value
     * @return This object.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator|=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        _upper |= other._upper;
        _lower |= other._lower;
        return *this;
    }

    /**
     * Bitwise XOR assignment.
     * 
     * @param[in] other of value
     * @return This object.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator^=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        _upper ^= other._upper;
        _lower ^= other._lower;
        return *this;
    }

    /**
     * Bitwise left shift assignment.
     * 
     * @tparam T is integer type
     * @param[in] shift of value
     * @return This object.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator<<=(T shift) IPADDRESS_NOEXCEPT {
        *this = *this << shift;
        return *this;
    }
    
    /**
     * Bitwise right shift assignment.
     * 
     * @tparam T is integer type
     * @param[in] shift of value
     * @return This object.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator>>=(T shift) IPADDRESS_NOEXCEPT {
        *this = *this >> shift;
        return *this;
    }
    
    /**
     * Unary plus.
     * 
     * @return New value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+() const IPADDRESS_NOEXCEPT {
        return *this;
    }

    /**
     * Unary minus.
     * 
     * @return New value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-() const IPADDRESS_NOEXCEPT {
        return ++(~*this);
    }

    /**
     * Bitwise NOT.
     * 
     * @return New value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator~() const IPADDRESS_NOEXCEPT {
        return { ~_upper, ~_lower };
    }

    /**
     * Addition.
     * 
     * @tparam T is integer type
     * @param[in] lower of value
     * @return New value.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+(T lower) const IPADDRESS_NOEXCEPT {
        return *this + uint128_t(lower);
    }

    /**
     * Addition.
     * 
     * @param[in] other of value
     * @return New value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        const uint64_t lower = _lower + other._lower;
        const uint64_t carry = lower < _lower ? 1 : 0;
        return { _upper + other._upper + carry, lower };
    }
    
    /**
     * Subtraction.
     * 
     * @tparam T is integer type
     * @param[in] lower of value
     * @return New value.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-(T lower) const IPADDRESS_NOEXCEPT {
        return *this - uint128_t(lower);
    }

    /**
     * Subtraction.
     * 
     * @param[in] other of value
     * @return New value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        const uint64_t lower = _lower - other._lower;
        const uint64_t borrow = lower > _lower ? 1 : 0;
        return { _upper - other._upper - borrow, lower };
    }

    /**
     * Multiplication.
     * 
     * @tparam T is integer type
     * @param[in] lower of value
     * @return New value.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator*(T lower) const IPADDRESS_NOEXCEPT {
        return *this * uint128_t(lower);
    }

    /**
     * Multiplication.
     * 
     * @param[in] other of value
     * @return New value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator*(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        uint64_t lower = 0;
        uint64_t upper = big_mul(_lower, other._lower, lower);
        upper += (_upper * other._lower) + (_lower * other._upper);
        return { upper, lower };
    }
    
    /**
     * Division.
     * 
     * @tparam T is integer type
     * @param[in] lower of value
     * @return New value.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator/(T lower) const IPADDRESS_NOEXCEPT {
        return *this / uint128_t(lower);
    }

    /**
     * Division.
     * 
     * @param[in] other of value
     * @return New value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator/(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return divide(*this, other);
    }

    /**
     * Remainder.
     * 
     * @tparam T is integer type
     * @param[in] lower of value
     * @return New value.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator%(T lower) const IPADDRESS_NOEXCEPT {
        return *this % uint128_t(lower);
    }

    /**
     * Remainder.
     * 
     * @param[in] other of value
     * @return New value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator%(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        const auto quotient = divide(*this, other);
        return *this - quotient * other;
    }

    /**
     * Bitwise AND.
     * 
     * @tparam T is integer type
     * @param[in] lower of value
     * @return New value.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator&(T lower) const IPADDRESS_NOEXCEPT {
        return *this & uint128_t(lower);
    }

    /**
     * Bitwise AND.
     * 
     * @param[in] other of value
     * @return New value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator&(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return { _upper & other._upper, _lower & other._lower };
    }

    /**
     * Bitwise OR.
     * 
     * @tparam T is integer type
     * @param[in] lower of value
     * @return New value.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator|(T lower) const IPADDRESS_NOEXCEPT {
        return *this | uint128_t(lower);
    }

    /**
     * Bitwise OR.
     * 
     * @param[in] other of value
     * @return New value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator|(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return { _upper | other._upper, _lower | other._lower };
    }

    /**
     * Bitwise XOR.
     * 
     * @tparam T is integer type
     * @param[in] lower of value
     * @return New value.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator^(T lower) const IPADDRESS_NOEXCEPT {
        return *this ^ uint128_t(lower);
    }

    /**
     * Bitwise XOR.
     * 
     * @param[in] other of value
     * @return New value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator^(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return { _upper ^ other._upper, _lower ^ other._lower };
    }

    /**
     * Bitwise left shift.
     * 
     * @tparam T is integer type
     * @param[in] shift of value
     * @return New value.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator<<(T shift) const IPADDRESS_NOEXCEPT {
        if (!shift) {
            return *this;
        }
        if (shift >= 64 && shift <= 128) {
            return { _lower << (shift - 64), 0 };
        }
        if (shift < 64 && shift > 0) {
            return { (_upper << shift) + (_lower >> (64 - shift)), _lower << shift };
        }
        return { 0, 0 };
    }

    /**
     * Bitwise right shift.
     * 
     * @tparam T is integer type
     * @param[in] shift of value
     * @return New value.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator>>(T shift) const IPADDRESS_NOEXCEPT {
        if (!shift) {
            return *this;
        }
        if (shift >= 64 && shift <= 128) {
            return { 0, _upper >> (shift - 64) };
        }
        if (shift < 64 && shift > 0) {
            return { _upper >> shift, (_lower >> shift) + (_upper << (64 - shift)) };
        }
        return { 0, 0 };
    }

    /**
     * Pre-increment.
     * 
     * @return New value.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator++() IPADDRESS_NOEXCEPT {
        *this += 1;
        return *this;
    }
    
    /**
     * Pre-decrement.
     * 
     * @return New value.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t& operator--() IPADDRESS_NOEXCEPT {
        *this -= 1;
        return *this;
    }
    
    /**
     * Post-increment.
     * 
     * @return New value.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t operator++(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    /**
     * Post-decrement.
     * 
     * @return New value.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE uint128_t operator--(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    /**
     * Negation.
     * 
     * @return Logical not.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!() const IPADDRESS_NOEXCEPT {
        return !_upper && !_lower;
    }

    /**
     * AND.
     * 
     * @return Logical AND.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator&&(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return (_upper || _lower) && (other._upper || other._lower);
    }

    /**
     * inclusive OR.
     * 
     * @return Logical OR.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator||(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return (_upper || _lower) || (other._upper || other._lower);
    }

    /**
     * Equal to.
     * 
     * @tparam T is integer type
     * @param[in] lower of value
     * @return true` if the values of the uint128_t are equal, `false` otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(T lower) const IPADDRESS_NOEXCEPT {
        return *this == uint128_t(lower);
    }

    /**
     * Equal to.
     * 
     * @param[in] other of value
     * @return `true` if the values of the uint128_t are equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return _upper == other._upper && _lower == other._lower;
    }

    /**
     * Not equal to.
     * 
     * @tparam T is integer type
     * @param[in] lower of value
     * @return `true` if the values of the uint128_t are not equal, `false` otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(T lower) const IPADDRESS_NOEXCEPT {
        return *this != uint128_t(lower);
    }

    /**
     * Not equal to.
     * 
     * @param[in] other of value
     * @return `true` if the values of the uint128_t are not equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(T lower) const IPADDRESS_NOEXCEPT {
        return *this <=> uint128_t(lower);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        if (const auto result = _upper <=> other._upper; result == std::strong_ordering::equivalent) {
            return _lower <=> other._lower;
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(T lower) const IPADDRESS_NOEXCEPT {
        return *this < uint128_t(lower);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return _upper < other._upper || (_upper == other._upper && _lower < other._lower);
    }
    
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(T lower) const IPADDRESS_NOEXCEPT {
        return *this > uint128_t(lower);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }
    
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(T lower) const IPADDRESS_NOEXCEPT {
        return *this <= uint128_t(lower);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }
    
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(T lower) const IPADDRESS_NOEXCEPT {
        return *this >= uint128_t(lower);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return !(*this < other);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint64_t big_mul(uint64_t a, uint64_t b, uint64_t& lower) IPADDRESS_NOEXCEPT {
        uint32_t al = uint32_t(a);
        uint32_t ah = uint32_t(a >> 32);
        uint32_t bl = uint32_t(b);
        uint32_t bh = uint32_t(b >> 32);
 
        uint64_t mull = uint64_t(al) * bl;
        uint64_t t = uint64_t(ah) * bl + (mull >> 32);
        uint64_t tl = uint64_t(al) * bh + uint32_t(t);
 
        lower = tl << 32 | uint32_t(mull);
 
        return uint64_t(ah) * bh + (t >> 32) + (tl >> 32);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t divide(const uint128_t& lhs, const uint128_t& rhs) IPADDRESS_NOEXCEPT {
        if (rhs._upper == 0) {
            if (rhs._lower == 0) {
                return {};
            }

            if (lhs._upper == 0) {
                return lhs._lower / rhs._lower;
            }
        }

        if (rhs >= lhs) {
            return rhs == lhs ? 1 : 0;
        }

        return divide_slow(lhs, rhs);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t divide_slow(const uint128_t& quotient, const uint128_t& divisor) IPADDRESS_NOEXCEPT {
        uint32_t left[4] {
            uint32_t(quotient._lower),
            uint32_t(quotient._lower >> 32),
            uint32_t(quotient._upper),
            uint32_t(quotient._upper >> 32)
        };
        int32_t left_size = 4 - int32_t(leading_zero_count(quotient)) / 32;

        uint32_t right[4] {
            uint32_t(divisor._lower),
            uint32_t(divisor._lower >> 32),
            uint32_t(divisor._upper),
            uint32_t(divisor._upper >> 32)
        };
        int32_t right_size = 4 - int32_t(leading_zero_count(divisor)) / 32;

        uint32_t bits[4]{};
        int32_t bits_size = left_size - right_size + 1;

        assert(left_size >= 1);
        assert(right_size >= 1);
        assert(left_size >= right_size);

        uint32_t div_hi = right[right_size - 1];
        uint32_t div_lo = right_size > 1 ? right[right_size - 2] : 0;

        int32_t shift = leading_zero_count(div_hi);
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

    IPADDRESS_NODISCARD static uint128_t from_double(double value) IPADDRESS_NOEXCEPT {
        constexpr double two_pow_128 = 340282366920938463463374607431768211456.0;

        if (value < 0 || std::isnan(value)) {
            return uint128_t(0, 0);
        }

        if (value >= two_pow_128) {
            return uint128_t(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF);
        }
 
        if (value >= 1.0) {
            uint64_t bits = double_to_uint64_bits(value);
            uint128_t result((bits << 12) >> 1 | 0x8000000000000000, 0x0000000000000000);
            result >>= (1023 + 128 - 1 - int32_t(bits >> 52));
            return result;
        }
        
        return uint128_t(0, 0);
    }

    IPADDRESS_NODISCARD static double to_double(uint128_t value) IPADDRESS_NOEXCEPT {
        constexpr double two_pow_52 = 4503599627370496.0;
        constexpr double two_pow_76 = 75557863725914323419136.0;
        constexpr double two_pow_104 = 20282409603651670423947251286016.0;
        constexpr double two_pow_128 = 340282366920938463463374607431768211456.0;

        constexpr uint64_t two_pow_52_bits = 0x4330000000000000;
        constexpr uint64_t two_pow_76_bits = 0x44B0000000000000;
        constexpr uint64_t two_pow_104_bits = 0x4670000000000000;
        constexpr uint64_t two_pow_128_bits = 0x47F0000000000000; 
                 
        if (value._upper == 0) {
            return double(value._lower);
        }     

        if ((value._upper >> 24) == 0) {
            double lower = uint64_bits_to_double(two_pow_52_bits | ((value._lower << 12) >> 12)) - two_pow_52;
            double upper = uint64_bits_to_double(two_pow_104_bits | (uint64_t)(value >> 52)) - two_pow_104;
            return double(lower + upper);
        }

        double lower = uint64_bits_to_double(two_pow_76_bits | ((uint64_t)(value >> 12) >> 12) | (value._lower & 0xFFFFFF)) - two_pow_76;
        double upper = uint64_bits_to_double(two_pow_128_bits | (uint64_t)(value >> 76)) - two_pow_128;
        return double(lower + upper);
    }

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE double uint64_bits_to_double(uint64_t bits) IPADDRESS_NOEXCEPT {
        double result;
        std::memcpy(&result, &bits, sizeof(double));
        return result;
    }

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE uint64_t double_to_uint64_bits(double value) IPADDRESS_NOEXCEPT {
        uint64_t result;
        std::memcpy(&result, &value, sizeof(uint64_t));
        return result;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t leading_zero_count(const uint128_t& value) IPADDRESS_NOEXCEPT {
        if (value._upper != 0) {
            for (uint32_t i = 0; i < 64; ++i) {
                if (value._upper & (1ULL << (63 - i))) {
                    return i;
                }
            }
        }
        if (value._lower != 0) {
            for (uint32_t i = 0; i < 64; ++i) {
                if (value._lower & (1ULL << (63 - i))) {
                    return 64 + i;
                }
            }
        }
        return 128;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t leading_zero_count(uint32_t value) IPADDRESS_NOEXCEPT {
        int32_t shift = 32;
        if (value != 0) {
            for (shift = 0; shift < 32; ++shift) {
                if (value & (1ULL << (31 - shift))) {
                    break;
                }
            }
        }
        return shift;
    }

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE std::string uint128_to_dec_str(uint128_t value) {
        std::ostringstream ss;

        do {
            const auto q = value / 10;
            const auto r = value - q * 10;

            ss << r._lower;

            value = q;
        } while (value);

        auto result = ss.str();
        std::reverse(result.begin(), result.end());
        return result;
    }

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE std::string uint128_to_oct_str(uint128_t value) {
        std::ostringstream ss;

        do {
            const auto q = value / 8;
            const auto r = value - q * 8;

            ss << r._lower;

            value = q;
        } while (value);

        auto result = ss.str();
        std::reverse(result.begin(), result.end());
        return result;
    }

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE std::string uint128_to_hex_str(uint128_t value) {
        constexpr char digits[] = "0123456789abcdef";

        std::ostringstream ss;
        do {
            const auto q = value / 16;
            const auto r = value - q * 16;

            ss << digits[r._lower];

            value = q;
        } while (value);

        auto result = ss.str();
        std::reverse(result.begin(), result.end());
        return result;
    }

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE optional<uint128_t> dec_str_to_uint128(const std::string& str) IPADDRESS_NOEXCEPT {
        uint128_t result = 0;
        for (const auto c : str) {
            if (c < '0' || c > '9') {
                return nullptr;
            }
            result = result * 10 + (c - '0');
        }
        return result;
    }

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE optional<uint128_t> oct_str_to_uint128(const std::string& str) IPADDRESS_NOEXCEPT {
        uint128_t result = 0;
        for (const auto c : str) {
            if (c < '0' || c > '7') {
                return nullptr;
            }
            result = result * 8 + (c - '0');
        }
        return result;
    }

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE optional<uint128_t> hex_str_to_uint128(const std::string& str) IPADDRESS_NOEXCEPT {
        uint128_t result = 0;
        int digit = 0;
        for (const auto c : str) {
            if (c >= '0' && c <= '9') {
                digit = c - '0';
            } else if (c >= 'A' && c <= 'F') {
                digit = c - 55;
            } else if (c >= 'a' && c <= 'f') {
                digit = c - 87;
            } else {
                return nullptr;
            }
            result = result * 16 + digit;
        }
        return result;
    }

#if IPADDRESS_ENDIAN == IPADDRESS_BIG_ENDIAN

    uint64_t _upper{};
    uint64_t _lower{};

#else // IPADDRESS_ENDIAN != IPADDRESS_BIG_ENDIAN

    uint64_t _lower{};
    uint64_t _upper{};

#endif // IPADDRESS_ENDIAN != IPADDRESS_BIG_ENDIAN
};

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) + value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) - value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator*(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) * value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator/(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) / value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator%(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) % value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator&(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) & value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator|(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) | value;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator^(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) ^ value;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) == other;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) != other;
}

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) <=> other;
}

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) < other;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) > other;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) <= other;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) >= other;
}

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

} // namespace IPADDRESS_NAMESPACE

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace std {

#pragma warning(push, 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#ifdef __clang__
  _Pragma("clang diagnostic push")
  _Pragma("clang diagnostic ignored \"-Wdeprecated\"")
#endif

template <typename T>
struct __numeric_limits_uint128 {
    static IPADDRESS_CONSTEXPR bool is_bounded               = true;
    static IPADDRESS_CONSTEXPR bool is_exact                 = true;
    static IPADDRESS_CONSTEXPR bool is_integer               = true;
    static IPADDRESS_CONSTEXPR bool is_modulo                = true;
    static IPADDRESS_CONSTEXPR bool is_specialized           = true;
    static IPADDRESS_CONSTEXPR bool is_iec559                = false;
    static IPADDRESS_CONSTEXPR bool is_signed                = false;
    static IPADDRESS_CONSTEXPR bool has_denorm_loss          = false;
    static IPADDRESS_CONSTEXPR bool has_infinity             = false;
    static IPADDRESS_CONSTEXPR bool has_quiet_NaN            = false;
    static IPADDRESS_CONSTEXPR bool has_signaling_NaN        = false;
    static IPADDRESS_CONSTEXPR bool tinyness_before          = false;
    static IPADDRESS_CONSTEXPR bool traps                    = false;
    static IPADDRESS_CONSTEXPR int max_digits10              = 0;
    static IPADDRESS_CONSTEXPR int max_exponent              = 0;
    static IPADDRESS_CONSTEXPR int max_exponent10            = 0;
    static IPADDRESS_CONSTEXPR int min_exponent              = 0;
    static IPADDRESS_CONSTEXPR int min_exponent10            = 0;
    static IPADDRESS_CONSTEXPR int digits                    = 128;
    static IPADDRESS_CONSTEXPR int digits10                  = 38;
    static IPADDRESS_CONSTEXPR int radix                     = 2;
    static IPADDRESS_CONSTEXPR float_denorm_style has_denorm = denorm_absent;
    static IPADDRESS_CONSTEXPR float_round_style round_style = round_toward_zero;

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T (min)() IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T (max)() IPADDRESS_NOEXCEPT {
        return T(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T lowest() IPADDRESS_NOEXCEPT {
        return (min)();
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T epsilon() IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T round_error() IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T denorm_min() IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T infinity() IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T quiet_NaN() IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T signaling_NaN() IPADDRESS_NOEXCEPT {
        return 0;
    }
};

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::is_bounded;

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::is_exact;

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::is_integer;

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::is_modulo;

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::is_specialized;

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::is_iec559;

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::is_signed;

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::has_denorm_loss;

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::has_infinity;

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::has_quiet_NaN;

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::has_signaling_NaN;

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::tinyness_before;

template <typename T>
IPADDRESS_CONSTEXPR bool __numeric_limits_uint128<T>::traps;

template <typename T>
IPADDRESS_CONSTEXPR int __numeric_limits_uint128<T>::max_digits10;

template <typename T>
IPADDRESS_CONSTEXPR int __numeric_limits_uint128<T>::max_exponent;

template <typename T>
IPADDRESS_CONSTEXPR int __numeric_limits_uint128<T>::max_exponent10;

template <typename T>
IPADDRESS_CONSTEXPR int __numeric_limits_uint128<T>::min_exponent;

template <typename T>
IPADDRESS_CONSTEXPR int __numeric_limits_uint128<T>::min_exponent10;

template <typename T>
IPADDRESS_CONSTEXPR int __numeric_limits_uint128<T>::digits;

template <typename T>
IPADDRESS_CONSTEXPR int __numeric_limits_uint128<T>::digits10;

template <typename T>
IPADDRESS_CONSTEXPR int __numeric_limits_uint128<T>::radix;

template <typename T>
IPADDRESS_CONSTEXPR float_denorm_style __numeric_limits_uint128<T>::has_denorm;

template <typename T>
IPADDRESS_CONSTEXPR float_round_style __numeric_limits_uint128<T>::round_style;

#ifdef __clang__
  _Pragma("clang diagnostic pop")
#endif
#pragma GCC diagnostic pop
#pragma warning(pop)

template <>
struct numeric_limits<IPADDRESS_NAMESPACE::uint128_t> : __numeric_limits_uint128<IPADDRESS_NAMESPACE::uint128_t> {
};

template <>
struct hash<IPADDRESS_NAMESPACE::uint128_t> {
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t operator()(const IPADDRESS_NAMESPACE::uint128_t& value) const IPADDRESS_NOEXCEPT {
        return value.hash();
    }
};

IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(IPADDRESS_NAMESPACE::uint128_t& value1, IPADDRESS_NAMESPACE::uint128_t& value2) IPADDRESS_NOEXCEPT {
    const auto tmp = value1;
    value1 = value2;
    value2 = tmp;
}

IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE IPADDRESS_NAMESPACE::uint128_t abs(const IPADDRESS_NAMESPACE::uint128_t& value) IPADDRESS_NOEXCEPT {
    return value;
}

IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(const IPADDRESS_NAMESPACE::uint128_t& value) {
    return value.to_string();
}

IPADDRESS_FORCE_INLINE std::ostream& operator<<(std::ostream& stream, const IPADDRESS_NAMESPACE::uint128_t& value) {
    auto fmt = IPADDRESS_NAMESPACE::uint128_t::format::decimal;
    if (stream.flags() & ios_base::hex) {
        fmt = IPADDRESS_NAMESPACE::uint128_t::format::hexadecimal;
    } else if (stream.flags() & ios_base::oct) {
        fmt = IPADDRESS_NAMESPACE::uint128_t::format::octal;
    }
    auto str = value.to_string(fmt);
    if (stream.flags() & ios_base::uppercase) {
        std::transform(str.cbegin(), str.cend(), str.begin(), [](auto c){
            return std::toupper(c);
        });
    }
    return stream << str;
}

IPADDRESS_FORCE_INLINE std::istream& operator>>(std::istream& stream, IPADDRESS_NAMESPACE::uint128_t& value) {
    auto fmt = IPADDRESS_NAMESPACE::uint128_t::format::decimal;
    if (stream.flags() & ios_base::hex) {
        fmt = IPADDRESS_NAMESPACE::uint128_t::format::hexadecimal;
    } else if (stream.flags() & ios_base::oct) {
        fmt = IPADDRESS_NAMESPACE::uint128_t::format::octal;
    }
    std::string str;
    stream >> str;
    const auto result = IPADDRESS_NAMESPACE::uint128_t::from_string(str, fmt);
    if (result) {
        value = result.value();
    } else {
        stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

} // namespace std

#endif // IPADDRESS_NO_OVERLOAD_STD

#endif // IPADDRESS_UINT128_HPP
