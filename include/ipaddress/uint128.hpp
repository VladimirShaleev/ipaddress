/**
 * @file      uint128.hpp
 * @brief     Defines `uint128_t`, a portable 128-bit unsigned integer type
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This header introduces `uint128_t`, a class that emulates a 128-bit unsigned integer.
 * It is designed to fill the gap in the C++ standard, which does not natively support
 * 128-bit integers across all platforms. Unlike compiler-specific extensions like `__int128`,
 * `uint128_t` ensures compatibility and portability across different compilers and architectures.
 * The implementation is inspired by the algorithms used in the .NET framework's 
 * [UInt128](https://source.dot.net/#System.Private.CoreLib/src/libraries/System.Private.CoreLib/src/System/UInt128.cs),
 * providing a reliable foundation for arithmetic operations and other integer-related functionalities.
 * The class integrates seamlessly with the `ipaddress` library, offering standard hash support and
 * optional type compatibility.
 *
 * The `uint128_t` type is especially useful in applications requiring precise control over
 * large integer values, such as cryptography, high-precision arithmetic, and IP address manipulation.
 * Its design prioritizes accuracy, making it a robust tool for developers
 * working with high-volume data or complex numerical computations.
 */

#ifndef IPADDRESS_UINT128_HPP
#define IPADDRESS_UINT128_HPP

#include "config.hpp"
#include "hash.hpp"
#include "optional.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * Class for representing a 128-bit unsigned integer.
 * 
 * `uint128_t` offers a comprehensive suite of operations and utilities for handling
 * 128-bit unsigned integers. It supports basic arithmetic operations, bitwise logic,
 * comparison operators, and specialized functions for advanced integer manipulation.
 * The class is optimized for both space and time efficiency, ensuring minimal overhead
 * and maximum performance.
 *
 * The design of `uint128_t` is focused on ease of use and integration. It provides
 * constructors for seamless conversion from built-in integer types, as well as explicit
 * methods for converting to and from other numeric representations. The class also
 * includes support for standard library features like hashing and optional values,
 * enhancing its utility in a wide range of programming scenarios.
 * 
 * The implementation is based on algorithms from the .NET `UInt128`
 * structure, ensuring reliable and efficient operations.
 * 
 * @see [UInt128](https://source.dot.net/#System.Private.CoreLib/src/libraries/System.Private.CoreLib/src/System/UInt128.cs) .NET Implementation.
 */
class uint128_t final { // NOLINT(cppcoreguidelines-special-member-functions)
public:
    /**
     * Enumerates the string formats available for `uint128_t`.
     * 
     * This enumeration defines the possible string representations of `uint128_t` values.
     * It allows users to specify the desired format when converting `uint128_t` instances to strings.
     */
    enum class format {
        decimal = 0, /**< Represents the number in decimal format. *This is the default format.* */
        octal,       /**< Represents the number in octal format. */
        hexadecimal  /**< Represents the number in hexadecimal format. */
    };

    /**
     * Default constructor.
     *
     * Constructs a new `uint128_t` instance with default values.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t() IPADDRESS_NOEXCEPT = default;

    /**
     * Copy constructor.
     *
     * Constructs a new `uint128_t` instance by copying the value from another instance.
     * 
     * @param[in] other The `uint128_t` instance to copy.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(const uint128_t& other) IPADDRESS_NOEXCEPT = default;
    
    /**
     * Move constructor.
     *
     * Constructs a new `uint128_t` instance by moving the value from another instance.
     * 
     * @param[in,out] other The `uint128_t` instance to move.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(uint128_t&& other) IPADDRESS_NOEXCEPT = default;

    /**
     * Assignment operator.
     *
     * Assigns the value of one `uint128_t` instance to another.
     * 
     * @param[in] other The `uint128_t` instance to assign from.
     * @return A reference to the assigned `uint128_t` instance.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator=(const uint128_t& other) IPADDRESS_NOEXCEPT = default;
    
    /**
     * Move assignment operator.
     *
     * Moves the value of one `uint128_t` instance to another.
     * 
     * @param[in,out] other The `uint128_t` instance to move from.
     * @return A reference to the moved `uint128_t` instance.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator=(uint128_t&& other) IPADDRESS_NOEXCEPT = default;

    /**
     * Constructs a `uint128_t` instance from upper and lower parts.
     *
     * This constructor initializes a `uint128_t` instance using separate upper and lower 64-bit integers.
     * 
     * @param[in] upper The upper 64 bits of the `uint128_t` value.
     * @param[in] lower The lower 64 bits of the `uint128_t` value.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(uint64_t upper, uint64_t lower) IPADDRESS_NOEXCEPT

#if IPADDRESS_ENDIAN == IPADDRESS_BIG_ENDIAN

        : _upper(upper), _lower(lower)

#else // IPADDRESS_ENDIAN != IPADDRESS_BIG_ENDIAN

        : _lower(lower), _upper(upper)

#endif // IPADDRESS_ENDIAN != IPADDRESS_BIG_ENDIAN
    { }

    /**
     * Constructs a `uint128_t` instance from an unsigned integer.
     *
     * This constructor initializes a `uint128_t` instance using an unsigned integer,
     * setting it as the lower part of the `uint128_t` value.
     * 
     * @param[in] lower The unsigned integer to initialize the `uint128_t` instance with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(unsigned long long lower) IPADDRESS_NOEXCEPT : _lower(uint64_t(lower)) {
    }

    /**
     * Constructs a `uint128_t` instance from an unsigned integer.
     *
     * This constructor initializes a `uint128_t` instance using an unsigned integer,
     * setting it as the lower part of the `uint128_t` value.
     * 
     * @param[in] lower The unsigned integer to initialize the `uint128_t` instance with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(unsigned long lower) IPADDRESS_NOEXCEPT : uint128_t((unsigned long long) lower) {
    }

    /**
     * Constructs a `uint128_t` instance from an unsigned integer.
     *
     * This constructor initializes a `uint128_t` instance using an unsigned integer,
     * setting it as the lower part of the `uint128_t` value.
     * 
     * @param[in] lower The unsigned integer to initialize the `uint128_t` instance with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(unsigned int lower) IPADDRESS_NOEXCEPT : uint128_t((unsigned long long) lower) {
    }

    /**
     * Constructs a `uint128_t` instance from an unsigned integer.
     *
     * This constructor initializes a `uint128_t` instance using an unsigned integer,
     * setting it as the lower part of the `uint128_t` value.
     * 
     * @param[in] lower The unsigned integer to initialize the `uint128_t` instance with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(unsigned short lower) IPADDRESS_NOEXCEPT : uint128_t((unsigned long long) lower) {
    }

    /**
     * Constructs a `uint128_t` instance from a signed integer.
     *
     * This constructor initializes a `uint128_t` instance using a signed integer,
     * setting it as the lower part of the `uint128_t` value and extending the sign to the upper part.
     * 
     * @param[in] lower The signed integer to initialize the `uint128_t` instance with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(long long lower) IPADDRESS_NOEXCEPT

#if IPADDRESS_ENDIAN == IPADDRESS_BIG_ENDIAN

        : _upper(uint64_t(int64_t(lower) >> 63)), _lower(uint64_t(lower))

#else // IPADDRESS_ENDIAN != IPADDRESS_BIG_ENDIAN

        : _lower(uint64_t(lower)), _upper(uint64_t(int64_t(lower) >> 63))

#endif // IPADDRESS_ENDIAN != IPADDRESS_BIG_ENDIAN
    { }

    /**
     * Constructs a `uint128_t` instance from a signed integer.
     *
     * This constructor initializes a `uint128_t` instance using a signed integer,
     * setting it as the lower part of the `uint128_t` value and extending the sign to the upper part.
     * 
     * @param[in] lower The signed integer to initialize the `uint128_t` instance with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(long lower) IPADDRESS_NOEXCEPT : uint128_t((long long) lower) {
    }

    /**
     * Constructs a `uint128_t` instance from a signed integer.
     *
     * This constructor initializes a `uint128_t` instance using a signed integer,
     * setting it as the lower part of the `uint128_t` value and extending the sign to the upper part.
     * 
     * @param[in] lower The signed integer to initialize the `uint128_t` instance with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(int lower) IPADDRESS_NOEXCEPT : uint128_t((long long) lower) {
    }

    /**
     * Constructs a `uint128_t` instance from a signed integer.
     *
     * This constructor initializes a `uint128_t` instance using a signed integer,
     * setting it as the lower part of the `uint128_t` value and extending the sign to the upper part.
     * 
     * @param[in] lower The signed integer to initialize the `uint128_t` instance with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t(short lower) IPADDRESS_NOEXCEPT : uint128_t((long long) lower) {
    }

    /**
     * Constructs a `uint128_t` instance from a floating-point value.
     * 
     * This constructor initializes a `uint128_t` instance by converting a floating-point value to its
     * 128-bit unsigned integer representation. The conversion is explicit to prevent unintended implicit conversions.
     * 
     * @param[in] value The floating-point value to convert.
     */
    IPADDRESS_FORCE_INLINE explicit uint128_t(double value) IPADDRESS_NOEXCEPT {
        const auto result = from_double(value);
        _upper = result._upper;
        _lower = result._lower;
    }

    /**
     * Constructs a `uint128_t` instance from a floating-point value.
     * 
     * This constructor initializes a `uint128_t` instance by converting a floating-point value to its
     * 128-bit unsigned integer representation. The conversion is explicit to prevent unintended implicit conversions.
     * 
     * @param[in] value The floating-point value to convert.
     */
    IPADDRESS_FORCE_INLINE explicit uint128_t(long double value) IPADDRESS_NOEXCEPT : uint128_t((double) value) {
    }

    /**
     * Constructs a `uint128_t` instance from a floating-point value.
     * 
     * This constructor initializes a `uint128_t` instance by converting a floating-point value to its
     * 128-bit unsigned integer representation. The conversion is explicit to prevent unintended implicit conversions.
     * 
     * @param[in] value The floating-point value to convert.
     */
    IPADDRESS_FORCE_INLINE explicit uint128_t(float value) IPADDRESS_NOEXCEPT : uint128_t((double) value) {
    }

    /**
     * Retrieves the lower 64 bits of the `uint128_t` value.
     * 
     * This method returns the lower part of the `uint128_t` instance, allowing access to the least significant bits.
     * 
     * @return The lower 64 bits as a `uint64_t`.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint64_t lower() const IPADDRESS_NOEXCEPT {
        return _lower;
    }

    /**
     * Retrieves the upper 64 bits of the `uint128_t` value.
     * 
     * This method returns the upper part of the `uint128_t` instance, allowing access to the most significant bits.
     * 
     * @return The upper 64 bits as a `uint64_t`.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint64_t upper() const IPADDRESS_NOEXCEPT {
        return _upper;
    }

    /**
     * Computes the hash value of the `uint128_t` instance.
     * 
     * This method calculates a hash value for the `uint128_t` instance, which can be used in hash-based data structures.
     * 
     * @return A `size_t` representing the hash value of the `uint128_t` instance.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t hash() const IPADDRESS_NOEXCEPT {
        internal::hash_combine<8> hasher{};
        const auto seed = hasher(_upper);
        const auto hash = hasher(seed + 0x9e3779b9 + _lower);
        return hash;
    }

    /**
     * Swaps the values of two `uint128_t` instances.
     * 
     * This method exchanges the contents of the `uint128_t` instance with those of another instance.
     * 
     * @param[in,out] other The other `uint128_t` instance to swap with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(uint128_t& other) IPADDRESS_NOEXCEPT {
        const auto tmp = *this;
        *this = other;
        other = tmp;
    }

    /**
     * Converts the `uint128_t` value to a string representation.
     * 
     * This method converts the `uint128_t` instance to its string representation in the specified format.
     * It supports decimal, octal, and hexadecimal formats.
     * 
     * @param[in] fmt The format to use for the conversion, with `format::decimal` as the default.
     * @return A `std::string` holding the converted value.
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
     * Converts the `uint128_t` value to a string representation.
     * 
     * This method converts the `uint128_t` instance to its string representation in the specified format.
     * It supports decimal, octal, and hexadecimal formats.
     * 
     * @param[in] fmt The format to use for the conversion, with `format::decimal` as the default.
     * @return A `std::wstring` holding the converted value.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::wstring to_wstring(format fmt = format::decimal) const {
        return internal::string_converter<wchar_t>::convert(to_string(fmt));
    }

    /**
     * Converts the `uint128_t` value to a string representation.
     * 
     * This method converts the `uint128_t` instance to its string representation in the specified format.
     * It supports decimal, octal, and hexadecimal formats.
     * 
     * @param[in] fmt The format to use for the conversion, with `format::decimal` as the default.
     * @return A `std::u16string` holding the converted value.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::u16string to_u16string(format fmt = format::decimal) const {
        return internal::string_converter<char16_t>::convert(to_string(fmt));
    }

    /**
     * Converts the `uint128_t` value to a string representation.
     * 
     * This method converts the `uint128_t` instance to its string representation in the specified format.
     * It supports decimal, octal, and hexadecimal formats.
     * 
     * @param[in] fmt The format to use for the conversion, with `format::decimal` as the default.
     * @return A `std::u32string` holding the converted value.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::u32string to_u32string(format fmt = format::decimal) const {
        return internal::string_converter<char32_t>::convert(to_string(fmt));
    }

#if __cpp_char8_t >= 201811L

    /**
     * Converts the `uint128_t` value to a string representation.
     * 
     * This method converts the `uint128_t` instance to its string representation in the specified format.
     * It supports decimal, octal, and hexadecimal formats.
     * 
     * @param[in] fmt The format to use for the conversion, with `format::decimal` as the default.
     * @return A `std::u8string` holding the converted value.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::u8string to_u8string(format fmt = format::decimal) const {
        return internal::string_converter<char8_t>::convert(to_string(fmt));
    }

#endif // __cpp_char8_t

    /**
     * Parses a string to a `uint128_t` instance.
     *
     * This static method attempts to parse a given string as a `uint128_t` value in the specified format.
     * If the parsing is successful, it returns an optional containing the parsed `uint128_t` value.
     *
     * @param[in] str The input string to parse.
     * @param[in] fmt The string format to interpret the input string (*defaults to decimal*).
     * @return An optional containing the parsed `uint128_t` value if successful, otherwise an empty optional.
     */
    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE optional<uint128_t> from_string(const std::string& str, format fmt = format::decimal) IPADDRESS_NOEXCEPT {
        return str_to_uint128(str.data(), str.data() + str.length(), fmt);
    }

    /**
     * Parses a string to a `uint128_t` instance.
     *
     * This static method attempts to parse a given string as a `uint128_t` value in the specified format.
     * If the parsing is successful, it returns an optional containing the parsed `uint128_t` value.
     *
     * @param[in] str The input string to parse.
     * @param[in] fmt The string format to interpret the input string (*defaults to decimal*).
     * @return An optional containing the parsed `uint128_t` value if successful, otherwise an empty optional.
     */
    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE optional<uint128_t> from_string(const std::wstring& str, format fmt = format::decimal) IPADDRESS_NOEXCEPT {
        return str_to_uint128(str.data(), str.data() + str.length(), fmt);
    }

    /**
     * Parses a string to a `uint128_t` instance.
     *
     * This static method attempts to parse a given string as a `uint128_t` value in the specified format.
     * If the parsing is successful, it returns an optional containing the parsed `uint128_t` value.
     *
     * @param[in] str The input string to parse.
     * @param[in] fmt The string format to interpret the input string (*defaults to decimal*).
     * @return An optional containing the parsed `uint128_t` value if successful, otherwise an empty optional.
     */
    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE optional<uint128_t> from_string(const std::u16string& str, format fmt = format::decimal) IPADDRESS_NOEXCEPT {
        return str_to_uint128(str.data(), str.data() + str.length(), fmt);
    }

    /**
     * Parses a string to a `uint128_t` instance.
     *
     * This static method attempts to parse a given string as a `uint128_t` value in the specified format.
     * If the parsing is successful, it returns an optional containing the parsed `uint128_t` value.
     *
     * @param[in] str The input string to parse.
     * @param[in] fmt The string format to interpret the input string (*defaults to decimal*).
     * @return An optional containing the parsed `uint128_t` value if successful, otherwise an empty optional.
     */
    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE optional<uint128_t> from_string(const std::u32string& str, format fmt = format::decimal) IPADDRESS_NOEXCEPT {
        return str_to_uint128(str.data(), str.data() + str.length(), fmt);
    }

#if __cpp_char8_t >= 201811L

    /**
     * Parses a string to a `uint128_t` instance.
     *
     * This static method attempts to parse a given string as a `uint128_t` value in the specified format.
     * If the parsing is successful, it returns an optional containing the parsed `uint128_t` value.
     *
     * @param[in] str The input string to parse.
     * @param[in] fmt The string format to interpret the input string (*defaults to decimal*).
     * @return An optional containing the parsed `uint128_t` value if successful, otherwise an empty optional.
     */
    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE optional<uint128_t> from_string(const std::u8string& str, format fmt = format::decimal) IPADDRESS_NOEXCEPT {
        return str_to_uint128(str.data(), str.data() + str.length(), fmt);
    }

#endif // __cpp_char8_t

    /**
     * Parses a string to a `uint128_t` instance.
     *
     * This static method attempts to parse a given string as a `uint128_t` value in the specified format.
     * If the parsing is successful, it returns an optional containing the parsed `uint128_t` value.
     *
     * @tparam T The character type of the array.
     * @tparam N The size of the character array.
     * @param[in] str The input string to parse.
     * @param[in] fmt The string format to interpret the input string (*defaults to decimal*).
     * @return An optional containing the parsed `uint128_t` value if successful, otherwise an empty optional.
     */
    template <typename T, size_t N>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<uint128_t> from_string(const T(&str)[N], format fmt = format::decimal) IPADDRESS_NOEXCEPT {
        return str_to_uint128(str, str + N, fmt);
    }

    /**
     * Checks if the `uint128_t` value is non-zero.
     *
     * This method allows `uint128_t` instances to be contextually converted to a boolean to check for non-zero values.
     *
     * @return `true` if the `uint128_t` value is non-zero, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator bool() const IPADDRESS_NOEXCEPT {
        return _upper || _lower;
    }

    /**
     * Casts the `uint128_t` value to an integral type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified integral type.
     *
     * @return The `uint128_t` value cast to the specified integral type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator unsigned long long() const IPADDRESS_NOEXCEPT {
        return (unsigned long long) _lower;
    }

    /**
     * Casts the `uint128_t` value to an integral type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified integral type.
     *
     * @return The `uint128_t` value cast to the specified integral type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator long long() const IPADDRESS_NOEXCEPT {
        return (long long) _lower;
    }

    /**
     * Casts the `uint128_t` value to an integral type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified integral type.
     *
     * @return The `uint128_t` value cast to the specified integral type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator unsigned long() const IPADDRESS_NOEXCEPT {
        return (unsigned long) _lower;
    }

    /**
     * Casts the `uint128_t` value to an integral type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified integral type.
     *
     * @return The `uint128_t` value cast to the specified integral type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator long() const IPADDRESS_NOEXCEPT {
        return (long) _lower;
    }

    /**
     * Casts the `uint128_t` value to an integral type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified integral type.
     *
     * @return The `uint128_t` value cast to the specified integral type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator unsigned int() const IPADDRESS_NOEXCEPT {
        return (unsigned int) _lower;
    }

    /**
     * Casts the `uint128_t` value to an integral type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified integral type.
     *
     * @return The `uint128_t` value cast to the specified integral type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator int() const IPADDRESS_NOEXCEPT {
        return (int) _lower;
    }

    /**
     * Casts the `uint128_t` value to an integral type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified integral type.
     *
     * @return The `uint128_t` value cast to the specified integral type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator unsigned short() const IPADDRESS_NOEXCEPT {
        return (unsigned short) _lower;
    }

    /**
     * Casts the `uint128_t` value to an integral type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified integral type.
     *
     * @return The `uint128_t` value cast to the specified integral type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator short() const IPADDRESS_NOEXCEPT {
        return (short) _lower;
    }

    /**
     * Casts the `uint128_t` value to an integral type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified integral type.
     *
     * @return The `uint128_t` value cast to the specified integral type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator unsigned char() const IPADDRESS_NOEXCEPT {
        return (char) _lower;
    }

    /**
     * Casts the `uint128_t` value to an integral type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified integral type.
     *
     * @return The `uint128_t` value cast to the specified integral type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator char() const IPADDRESS_NOEXCEPT {
        return (char) _lower;
    }

    /**
     * Casts the `uint128_t` value to an integral type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified integral type.
     *
     * @return The `uint128_t` value cast to the specified integral type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator signed char() const IPADDRESS_NOEXCEPT {
        return (signed char) _lower;
    }

    /**
     * Casts the `uint128_t` value to a floating-point type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified floating-point type.
     *
     * @return The `uint128_t` value cast to the specified floating-point type.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE explicit operator long double() const IPADDRESS_NOEXCEPT {
        return (long double) to_double(*this);
    }

    /**
     * Casts the `uint128_t` value to a floating-point type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified floating-point type.
     *
     * @return The `uint128_t` value cast to the specified floating-point type.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE explicit operator double() const IPADDRESS_NOEXCEPT {
        return to_double(*this);
    }

    /**
     * Casts the `uint128_t` value to a floating-point type.
     *
     * This method allows for the explicit conversion of a `uint128_t` value to a specified floating-point type.
     *
     * @return The `uint128_t` value cast to the specified floating-point type.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE explicit operator float() const IPADDRESS_NOEXCEPT {
        return (float) to_double(*this);
    }

    /**
     * Addition assignment operator.
     *
     * Adds the value of another `uint128_t` instance to this instance and assigns the result to this instance.
     *
     * @param[in] other The `uint128_t` instance to add.
     * @return A reference to this instance after addition.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator+=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        *this = *this + other;
        return *this;
    }
    
    /**
     * Subtraction assignment operator.
     *
     * Subtracts the value of another `uint128_t` instance from this instance and assigns the result to this instance.
     *
     * @param[in] other The `uint128_t` instance to subtract.
     * @return A reference to this instance after subtraction.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator-=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        *this = *this - other;
        return *this;
    }

    /**
     * Multiplication assignment operator.
     *
     * Multiplies this instance by another `uint128_t` instance and assigns the result to this instance.
     *
     * @param[in] other The `uint128_t` instance to multiply by.
     * @return A reference to this instance after multiplication.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator*=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        *this = *this * other;
        return *this;
    }
    
    /**
     * Division assignment operator.
     *
     * Divides this instance by another `uint128_t` instance and assigns the result to this instance.
     *
     * @param[in] other The `uint128_t` instance to divide by.
     * @return A reference to this instance after division.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator/=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        *this = *this / other;
        return *this;
    }
    
    /**
     * Remainder assignment operator.
     *
     * Calculates the remainder of this instance divided by another `uint128_t` instance and assigns the result to this instance.
     *
     * @param[in] other The `uint128_t` instance to divide by.
     * @return A reference to this instance after calculating the remainder.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator%=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        *this = *this % other;
        return *this;
    }
    
    /**
     * Bitwise AND assignment operator.
     *
     * Performs a bitwise AND operation between this instance and another `uint128_t` instance and assigns the result to this instance.
     *
     * @param[in] other The `uint128_t` instance to perform the bitwise AND with.
     * @return A reference to this instance after the bitwise AND operation.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator&=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        _upper &= other._upper;
        _lower &= other._lower;
        return *this;
    }

    /**
     * Bitwise OR assignment operator.
     *
     * Performs a bitwise OR operation between this instance and another `uint128_t` instance and assigns the result to this instance.
     *
     * @param[in] other The `uint128_t` instance to perform the bitwise OR with.
     * @return A reference to this instance after the bitwise OR operation.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator|=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        _upper |= other._upper;
        _lower |= other._lower;
        return *this;
    }

    /**
     * Bitwise XOR assignment operator.
     *
     * Performs a bitwise XOR operation between this instance and another `uint128_t` instance and assigns the result to this instance.
     *
     * @param[in] other The `uint128_t` instance to perform the bitwise XOR with.
     * @return A reference to this instance after the bitwise XOR operation.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator^=(const uint128_t& other) IPADDRESS_NOEXCEPT {
        _upper ^= other._upper;
        _lower ^= other._lower;
        return *this;
    }

    /**
     * Bitwise left shift assignment operator.
     *
     * Shifts the bits of this instance to the left by the specified number of places and assigns the result to this instance.
     *
     * @tparam T An integral type representing the number of places to shift.
     * @param[in] shift The number of bits to shift to the left.
     * @return A reference to this instance after the shift operation.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator<<=(T shift) IPADDRESS_NOEXCEPT {
        *this = *this << shift;
        return *this;
    }
    
    /**
     * Bitwise right shift assignment operator.
     *
     * Shifts the bits of this instance to the right by the specified number of places and assigns the result to this instance.
     *
     * @tparam T An integral type representing the number of places to shift.
     * @param[in] shift The number of bits to shift to the right.
     * @return A reference to this instance after the shift operation.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator>>=(T shift) IPADDRESS_NOEXCEPT {
        *this = *this >> shift;
        return *this;
    }
    
    /**
     * Unary plus operator.
     *
     * Returns a new instance of `uint128_t` with the same value as this instance.
     *
     * @return A new `uint128_t` instance with the same value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+() const IPADDRESS_NOEXCEPT {
        return *this;
    }

    /**
     * Unary minus operator.
     *
     * Returns a new instance of `uint128_t` representing the two's complement of this instance.
     *
     * @return A new `uint128_t` instance representing the negated value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-() const IPADDRESS_NOEXCEPT {
        return ++(~*this);
    }

    /**
     * Bitwise NOT operator.
     *
     * Returns a new instance of `uint128_t` with all the bits of this instance inverted.
     *
     * @return A new `uint128_t` instance with inverted bits.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator~() const IPADDRESS_NOEXCEPT {
        return { ~_upper, ~_lower };
    }

    /**
     * Addition operator with another `uint128_t` instance.
     *
     * Adds the value of another `uint128_t` instance to this instance and returns a new `uint128_t` instance with the result.
     *
     * @param[in] other The `uint128_t` instance to add.
     * @return A new `uint128_t` instance representing the sum.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        const uint64_t lower = _lower + other._lower;
        const uint64_t carry = lower < _lower ? 1 : 0;
        return { _upper + other._upper + carry, lower };
    }
    
    /**
     * Subtraction operator with another `uint128_t` instance.
     *
     * Subtracts the value of another `uint128_t` instance from this instance and returns a new `uint128_t` instance with the result.
     *
     * @param[in] other The `uint128_t` instance to subtract.
     * @return A new `uint128_t` instance representing the difference.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        const uint64_t lower = _lower - other._lower;
        const uint64_t borrow = lower > _lower ? 1 : 0;
        return { _upper - other._upper - borrow, lower };
    }

    /**
     * Multiplication operator with another `uint128_t` instance.
     *
     * Multiplies this instance by another `uint128_t` instance and returns a new `uint128_t` instance with the result.
     *
     * @param[in] other The `uint128_t` instance to multiply by.
     * @return A new `uint128_t` instance representing the product.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator*(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        uint64_t lower = 0;
        uint64_t upper = big_mul(_lower, other._lower, lower);
        upper += (_upper * other._lower) + (_lower * other._upper);
        return { upper, lower };
    }
    
    /**
     * Division operator with another `uint128_t` instance.
     *
     * Divides this instance by another `uint128_t` instance and returns a new `uint128_t` instance with the result.
     *
     * @param[in] other The `uint128_t` instance to divide by.
     * @return A new `uint128_t` instance representing the quotient.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator/(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return divide(*this, other);
    }

    /**
     * Remainder operator with another `uint128_t` instance.
     *
     * Calculates the remainder of this instance divided by another `uint128_t` instance and returns a new `uint128_t` instance with the result.
     *
     * @param[in] other The `uint128_t` instance to divide by.
     * @return A new `uint128_t` instance representing the remainder.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator%(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        const auto quotient = divide(*this, other);
        return *this - quotient * other;
    }

    /**
     * Bitwise AND operator with another `uint128_t` instance.
     *
     * Performs a bitwise AND operation between this instance and another `uint128_t` instance and returns a new `uint128_t` instance with the result.
     *
     * @param[in] other The `uint128_t` instance to perform the bitwise AND with.
     * @return A new `uint128_t` instance representing the result of the bitwise AND operation.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator&(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return { _upper & other._upper, _lower & other._lower };
    }

    /**
     * Bitwise OR operator with another `uint128_t` instance.
     *
     * Performs a bitwise OR operation between this instance and another `uint128_t` instance and returns a new `uint128_t` instance with the result.
     *
     * @param[in] other The `uint128_t` instance to perform the bitwise OR with.
     * @return A new `uint128_t` instance representing the result of the bitwise OR operation.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator|(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return { _upper | other._upper, _lower | other._lower };
    }

    /**
     * Bitwise XOR operator with another `uint128_t` instance.
     *
     * Performs a bitwise XOR operation between this instance and another `uint128_t` instance and returns a new `uint128_t` instance with the result.
     *
     * @param[in] other The `uint128_t` instance to perform the bitwise XOR with.
     * @return A new `uint128_t` instance representing the result of the bitwise XOR operation.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator^(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return { _upper ^ other._upper, _lower ^ other._lower };
    }

    /**
     * Bitwise left shift operator with an integral type.
     *
     * Shifts the bits of this instance to the left by the specified number of places and returns a new `uint128_t` instance with the result.
     *
     * @tparam T An integral type representing the number of places to shift.
     * @param[in] shift The number of bits to shift to the left.
     * @return A new `uint128_t` instance representing the result of the left shift operation.
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
     * Bitwise right shift operator with an integral type.
     *
     * Shifts the bits of this instance to the right by the specified number of places and returns a new `uint128_t` instance with the result.
     *
     * @tparam T An integral type representing the number of places to shift.
     * @param[in] shift The number of bits to shift to the right.
     * @return A new `uint128_t` instance representing the result of the right shift operation.
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
     * Pre-increment operator.
     *
     * Increments this instance by one and returns a reference to this instance.
     *
     * @return A reference to this instance after the increment.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator++() IPADDRESS_NOEXCEPT {
        *this += 1;
        return *this;
    }
    
    /**
     * Pre-decrement operator.
     *
     * Decrements this instance by one and returns a reference to this instance.
     *
     * @return A reference to this instance after the decrement.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t& operator--() IPADDRESS_NOEXCEPT {
        *this -= 1;
        return *this;
    }
    
    /**
     * Post-increment operator.
     *
     * Increments this instance by one and returns the value of the instance before the increment.
     *
     * @return The value of this instance before the increment.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator++(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    /**
     * Post-decrement operator.
     *
     * Decrements this instance by one and returns the value of the instance before the decrement.
     *
     * @return The value of this instance before the decrement.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator--(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    /**
     * Logical NOT operator.
     *
     * Returns `true` if this instance is zero, otherwise returns `false`.
     *
     * @return `true` if this instance is zero, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!() const IPADDRESS_NOEXCEPT {
        return !_upper && !_lower;
    }

    /**
     * Logical AND operator.
     *
     * Returns `true` if both this instance and another `uint128_t` instance are non-zero, otherwise returns `false`.
     *
     * @param[in] other The `uint128_t` instance to compare with.
     * @return `true` if both instances are non-zero, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator&&(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return (_upper || _lower) && (other._upper || other._lower);
    }

    /**
     * Logical OR operator.
     *
     * Returns `true` if either this instance or another `uint128_t` instance is non-zero, otherwise returns `false`.
     *
     * @param[in] other The `uint128_t` instance to compare with.
     * @return `true` if either instance is non-zero, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator||(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return (_upper || _lower) || (other._upper || other._lower);
    }

    /**
     * Equality operator with an integral type.
     *
     * Compares this instance to an integral value for equality.
     *
     * @tparam T An integral type to compare with.
     * @param[in] lower The integral value to compare with.
     * @return `true` if this instance is equal to the integral value, `false` otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(T lower) const IPADDRESS_NOEXCEPT {
        return *this == uint128_t(lower);
    }

    /**
     * Equality operator with another `uint128_t` instance.
     *
     * Compares this instance to another `uint128_t` instance for equality.
     *
     * @param[in] other The `uint128_t` instance to compare with.
     * @return `true` if this instance is equal to the other instance, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return _upper == other._upper && _lower == other._lower;
    }

    /**
     * Inequality operator with an integral type.
     *
     * Compares this instance to an integral value for inequality.
     *
     * @tparam T An integral type to compare with.
     * @param[in] lower The integral value to compare with.
     * @return `true` if this instance is not equal to the integral value, `false` otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(T lower) const IPADDRESS_NOEXCEPT {
        return *this != uint128_t(lower);
    }

    /**
     * Inequality operator with another `uint128_t` instance.
     *
     * Compares this instance to another `uint128_t` instance for inequality.
     *
     * @param[in] other The `uint128_t` instance to compare with.
     * @return `true` if this instance is not equal to the other instance, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Three-way comparison operator with an integral type.
     *
     * Compares this instance to an integral value to determine the ordering relationship.
     *
     * @tparam T An integral type to compare with.
     * @param[in] lower The integral value to compare with.
     * @return An `std::strong_ordering` value indicating the ordering relationship.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(T lower) const IPADDRESS_NOEXCEPT {
        return *this <=> uint128_t(lower);
    }

    /**
     * Three-way comparison operator with another `uint128_t` instance.
     *
     * Compares this instance to another `uint128_t` instance to determine the ordering relationship.
     *
     * @param[in] other The `uint128_t` instance to compare with.
     * @return An `std::strong_ordering` value indicating the ordering relationship.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        if (const auto result = _upper <=> other._upper; result == std::strong_ordering::equivalent) {
            return _lower <=> other._lower;
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Less than operator with an integral type.
     *
     * Checks if the value of this instance is lexicographically less than the provided integral value.
     *
     * @tparam T An integral type to compare with.
     * @param[in] lower The integral value to compare with.
     * @return `true` if this instance is less than the integral value, `false` otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(T lower) const IPADDRESS_NOEXCEPT {
        return *this < uint128_t(lower);
    }

    /**
     * Less than operator with another `uint128_t` instance.
     *
     * Checks if the value of this instance is lexicographically less than the provided `uint128_t` instance.
     *
     * @param[in] other The `uint128_t` instance to compare with.
     * @return `true` if this instance is less than the other instance, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return _upper < other._upper || (_upper == other._upper && _lower < other._lower);
    }
    
    /**
     * Greater than operator with an integral type.
     *
     * Checks if the value of this instance is lexicographically greater than the provided integral value.
     *
     * @tparam T An integral type to compare with.
     * @param[in] lower The integral value to compare with.
     * @return `true` if this instance is greater than the integral value, `false` otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(T lower) const IPADDRESS_NOEXCEPT {
        return *this > uint128_t(lower);
    }

    /**
     * Greater than operator with another `uint128_t` instance.
     *
     * Checks if the value of this instance is lexicographically greater than the provided `uint128_t` instance.
     *
     * @param[in] other The `uint128_t` instance to compare with.
     * @return `true` if this instance is greater than the other instance, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }
    
    /**
     * Less than or equal to operator with an integral type.
     *
     * Checks if the value of this instance is lexicographically less than or equal to the provided integral value.
     *
     * @tparam T An integral type to compare with.
     * @param[in] lower The integral value to compare with.
     * @return `true` if this instance is less than or equal to the integral value, `false` otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(T lower) const IPADDRESS_NOEXCEPT {
        return *this <= uint128_t(lower);
    }

    /**
     * Less than or equal to operator with another `uint128_t` instance.
     *
     * Checks if the value of this instance is lexicographically less than or equal to the provided `uint128_t` instance.
     *
     * @param[in] other The `uint128_t` instance to compare with.
     * @return `true` if this instance is less than or equal to the other instance, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }
    
    /**
     * Greater than or equal to operator with an integral type.
     *
     * Checks if the value of this instance is lexicographically greater than or equal to the provided integral value.
     *
     * @tparam T An integral type to compare with.
     * @param[in] lower The integral value to compare with.
     * @return `true` if this instance is greater than or equal to the integral value, `false` otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(T lower) const IPADDRESS_NOEXCEPT {
        return *this >= uint128_t(lower);
    }

    /**
     * Greater than or equal to operator with another `uint128_t` instance.
     *
     * Checks if the value of this instance is lexicographically greater than or equal to the provided `uint128_t` instance.
     *
     * @param[in] other The `uint128_t` instance to compare with.
     * @return `true` if this instance is greater than or equal to the other instance, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const uint128_t& other) const IPADDRESS_NOEXCEPT {
        return !(*this < other);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint64_t big_mul(uint64_t a, uint64_t b, uint64_t& lower) IPADDRESS_NOEXCEPT {
        auto al = uint32_t(a);
        auto ah = uint32_t(a >> 32);
        auto bl = uint32_t(b);
        auto bh = uint32_t(b >> 32);
 
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

        uint32_t shift = leading_zero_count(div_hi);
        uint32_t back_shift = 32 - shift;

        if (shift > 0) {
            uint32_t div_nx = right_size > 2 ? right[right_size - 3] : 0;
            div_hi = (div_hi << shift) | (div_lo >> back_shift);
            div_lo = (div_lo << shift) | (div_nx >> back_shift);
        }

        for (int32_t i = left_size; i >= right_size; --i) {
            int32_t n = i - right_size;
            uint32_t t = uint32_t(i) < uint32_t(left_size) ? left[i] : 0;

            uint64_t val_hi = (uint64_t(t) << 32) | left[i - 1];
            uint32_t val_lo = (i > 1) ? left[i - 2] : 0; // NOLINT(clang-analyzer-core.uninitialized.Assign)

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

                    // NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores): carry is not used in the release build
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

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t subtract_divisor(uint32_t* left, int32_t ls /* NOLINT(misc-unused-parameters) */, const uint32_t (&right)[4], int32_t rs, uint64_t q) IPADDRESS_NOEXCEPT {
        assert(ls >= rs);
        assert(q <= 0xFFFFFFFF);

        uint64_t carry = 0;

        for (int32_t i = 0; i < rs; ++i) {
            carry += right[i] * q;

            auto digit = uint32_t(carry);
            carry >>= 32;

            if (left[i] < digit) {
                ++carry;
            }
            left[i] -= digit;
        }

        return uint32_t(carry);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t add_divisor(uint32_t* left, int32_t ls /* NOLINT(misc-unused-parameters) */, const uint32_t (&right)[4], int32_t rs) IPADDRESS_NOEXCEPT {
        assert(ls >= rs);

        uint64_t carry = 0;

        for (int32_t i = 0; i < rs; ++i) {
            uint64_t digit = left[i] + carry + right[i];
            left[i] = uint32_t(digit);
            carry = digit >> 32;
        }

        return uint32_t(carry);
    }

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE uint128_t from_double(double value) IPADDRESS_NOEXCEPT {
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

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE double to_double(uint128_t value) IPADDRESS_NOEXCEPT {
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
        double result; // NOLINT(cppcoreguidelines-init-variables)
        std::memcpy(&result, &bits, sizeof(double));
        return result;
    }

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE uint64_t double_to_uint64_bits(double value) IPADDRESS_NOEXCEPT {
        uint64_t result; // NOLINT(cppcoreguidelines-init-variables)
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

#pragma warning(push, 3)
#ifdef __clang__
  _Pragma("clang diagnostic push")
  _Pragma("clang diagnostic ignored \"-Wundefined-inline\"")
#endif
    template <typename T>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<uint128_t> str_to_uint128(const T* begin, const T* end, format fmt) IPADDRESS_NOEXCEPT {
        switch (fmt) {
            case format::octal:
                return oct_str_to_uint128(begin, end);
            case format::hexadecimal:
                return hex_str_to_uint128(begin, end);
            default:
                return dec_str_to_uint128(begin, end);
        }
    }
#ifdef __clang__
  _Pragma("clang diagnostic pop")
#endif
#pragma warning(pop)

    template <typename T>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<uint128_t> dec_str_to_uint128(const T* begin, const T* end) IPADDRESS_NOEXCEPT {
        error_code code = error_code::no_error;
        uint32_t error_symbol = 0;
        uint128_t result = 0;
        const T* it = begin;
        while (it < end) {
            const auto c = internal::next_char_or_error(it, end, code, error_symbol);
            if (code != error_code::no_error) { // NOLINT(bugprone-branch-clone)
                return nullptr;
            } else if (c == '\0') {
                break;
            } else if (c < '0' || c > '9') {
                return nullptr;
            }
            result = result * 10 + (c - '0');
        }
        return result;
    }

    template <typename T>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<uint128_t> oct_str_to_uint128(const T* begin, const T* end) IPADDRESS_NOEXCEPT {
        error_code code = error_code::no_error;
        uint32_t error_symbol = 0;
        uint128_t result = 0;
        const T* it = begin;
        while (it < end) {
            const auto c = internal::next_char_or_error(it, end, code, error_symbol);
            if (code != error_code::no_error) { // NOLINT(bugprone-branch-clone)
                return nullptr;
            } else if (c == '\0') {
                break;
            } else if (c < '0' || c > '7') {
                return nullptr;
            }
            result = result * 8 + (c - '0');
        }
        return result;
    }

    template <typename T>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<uint128_t> hex_str_to_uint128(const T* begin, const T* end) IPADDRESS_NOEXCEPT {
        error_code code = error_code::no_error;
        uint32_t error_symbol = 0;
        uint128_t result = 0;
        int digit = 0;
        const T* it = begin;
        while (it < end) {
            const auto c = internal::next_char_or_error(it, end, code, error_symbol);
            if (code != error_code::no_error) { // NOLINT(bugprone-branch-clone)
                return nullptr;
            } else if (c == '\0') {
                break;
            } else if (c >= '0' && c <= '9') {
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

/**
 * Performs addition between an integer and a uint128_t value.
 * 
 * @tparam T Integer type.
 * @param[in] lower The integer value to be added.
 * @param[in] value The uint128_t value to be added.
 * @return uint128_t the result of the addition.
 */
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator+(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) + value;
}

/**
 * Performs subtraction of a uint128_t value from an integer.
 * 
 * @tparam T Integer type.
 * @param[in] lower The integer value from which the uint128_t value is subtracted.
 * @param[in] value The uint128_t value to be subtracted.
 * @return uint128_t the result of the subtraction.
 */
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator-(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) - value;
}

/**
 * Performs multiplication between an integer and a uint128_t value.
 * 
 * @tparam T Integer type.
 * @param[in] lower The integer value to be multiplied.
 * @param[in] value The uint128_t value to be multiplied.
 * @return uint128_t the result of the multiplication.
 */
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator*(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) * value;
}

/**
 * Performs division of an integer by a uint128_t value.
 * 
 * @tparam T Integer type.
 * @param[in] lower The integer value to be divided.
 * @param[in] value The uint128_t value that divides the integer.
 * @return uint128_t the result of the division.
 */
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator/(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) / value;
}

/**
 * Calculates the remainder of division of an integer by a uint128_t value.
 * 
 * @tparam T Integer type.
 * @param[in] lower The integer value to be divided.
 * @param[in] value The uint128_t value that divides the integer.
 * @return uint128_t the remainder of the division.
 */
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator%(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) % value;
}

/**
 * Performs bitwise AND operation between an integer and a uint128_t value.
 * 
 * @tparam T Integer type.
 * @param[in] lower The integer value.
 * @param[in] value The uint128_t value.
 * @return uint128_t the result of the bitwise AND operation.
 */
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator&(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) & value;
}

/**
 * Performs bitwise OR operation between an integer and a uint128_t value.
 * 
 * @tparam T Integer type.
 * @param[in] lower The integer value.
 * @param[in] value The uint128_t value.
 * @return uint128_t the result of the bitwise OR operation.
 */
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator|(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) | value;
}

/**
 * Performs bitwise XOR operation between an integer and a uint128_t value.
 * 
 * @tparam T Integer type.
 * @param[in] lower The integer value.
 * @param[in] value The uint128_t value.
 * @return uint128_t the result of the bitwise XOR operation.
 */
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t operator^(T lower, const uint128_t& value) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) ^ value;
}

/**
 * Checks if an integer value is equal to a uint128_t value.
 * 
 * @tparam T Integral type.
 * @param[in] lower The integer value to compare.
 * @param[in] other The uint128_t value to compare.
 * @return `true` if the integer value is equal to the uint128_t value, `false` otherwise.
 */
template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) == other;
}

/**
 * Checks if an integer value is not equal to a uint128_t value.
 * 
 * @tparam T Integral type.
 * @param[in] lower The integer value to compare.
 * @param[in] other The uint128_t value to compare.
 * @return `true` if the integer value is not equal to the uint128_t value, `false` otherwise.
 */
template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) != other;
}

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

/**
 * Performs a three-way comparison between an integer and a uint128_t value.
 *
 * This function template overloads the spaceship operator (<=>) to provide a three-way comparison
 * between a value of an integral type T and a uint128_t value. It returns the ordering category
 * according to the C++20 standard library definition of std::strong_ordering.
 *
 * @tparam T An integral type parameter that is checked at compile-time to ensure it is an integer type.
 * @param[in] lower The value of type T to be compared.
 * @param[in] other The uint128_t value to be compared with.
 * @return The result of the comparison: less, equal, or greater.
 */
template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) <=> other;
}

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

/**
 * Compares if an integer value is less than a uint128_t value.
 *
 * This function template overloads the less than operator (<) to compare an integer of type T with a uint128_t value.
 * It checks if the integer value is lexicographically less than the uint128_t value.
 *
 * @tparam T An integral type parameter that is checked at compile-time to ensure it is an integer type.
 * @param[in] lower The integer value of type T to be compared.
 * @param[in] other The uint128_t value to compare against.
 * @return `true` if \a lower is less than \a other, `false` otherwise.
 */
template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) < other;
}

/**
 * Compares if an integer value is greater than a uint128_t value.
 *
 * This function template overloads the greater than operator (>) to compare an integer of type T with a uint128_t value.
 * It checks if the integer value is lexicographically greater than the uint128_t value.
 *
 * @tparam T An integral type parameter that is checked at compile-time to ensure it is an integer type.
 * @param[in] lower The integer value of type T to be compared.
 * @param[in] other The uint128_t value to compare against.
 * @return `true` if \a lower is greater than \a other, `false` otherwise.
 */
template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) > other;
}

/**
 * Compares if an integer value is less than or equal to a uint128_t value.
 *
 * This function template overloads the less than or equal to operator (<=) to compare an integer of type T with a uint128_t value.
 * It checks if the integer value is lexicographically less than or equal to the uint128_t value.
 *
 * @tparam T An integral type parameter that is checked at compile-time to ensure it is an integer type.
 * @param[in] lower The integer value of type T to be compared.
 * @param[in] other The uint128_t value to compare against.
 * @return `true` if \a lower is less than or equal to \a other, `false` otherwise.
 */
template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) <= other;
}

/**
 * Compares if an integer value is greater than or equal to a uint128_t value.
 *
 * This function template overloads the greater than or equal to operator (>=) to compare an integer of type T with a uint128_t value.
 * It checks if the integer value is lexicographically greater than or equal to the uint128_t value.
 *
 * @tparam T An integral type parameter that is checked at compile-time to ensure it is an integer type.
 * @param[in] lower The integer value of type T to be compared.
 * @param[in] other The uint128_t value to compare against.
 * @return `true` if \a lower is greater than or equal to \a other, `false` otherwise.
 */
template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(T lower, const uint128_t& other) IPADDRESS_NOEXCEPT {
    return uint128_t(lower) >= other;
}

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

} // namespace IPADDRESS_NAMESPACE

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace std {

#pragma warning(push, 3)
#if defined(_MSC_VER)
#  pragma warning(disable:4996)
#elif defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__clang__)
  _Pragma("clang diagnostic push")
  _Pragma("clang diagnostic ignored \"-Wdeprecated\"")
#endif

template <typename T>
struct _numeric_limits_uint128 {
    static constexpr bool is_bounded                    = true;
    static constexpr bool is_exact                      = true;
    static constexpr bool is_integer                    = true;
    static constexpr bool is_modulo                     = true;
    static constexpr bool is_specialized                = true;
    static constexpr bool is_iec559                     = false;
    static constexpr bool is_signed                     = false;
    static constexpr bool has_denorm_loss               = false;
    static constexpr bool has_infinity                  = false;
    static constexpr bool has_quiet_NaN                 = false;
    static constexpr bool has_signaling_NaN             = false;
    static constexpr bool tinyness_before               = false;
    static constexpr bool traps                         = false;
    static constexpr int max_digits10                   = 0;
    static constexpr int max_exponent                   = 0;
    static constexpr int max_exponent10                 = 0;
    static constexpr int min_exponent                   = 0;
    static constexpr int min_exponent10                 = 0;
    static constexpr int digits                         = 128;
    static constexpr int digits10                       = 38;
    static constexpr int radix                          = 2;
    static constexpr std::float_denorm_style has_denorm = std::denorm_absent;
    static constexpr std::float_round_style round_style = std::round_toward_zero;

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
constexpr bool _numeric_limits_uint128<T>::is_bounded;

template <typename T>
constexpr bool _numeric_limits_uint128<T>::is_exact;

template <typename T>
constexpr bool _numeric_limits_uint128<T>::is_integer;

template <typename T>
constexpr bool _numeric_limits_uint128<T>::is_modulo;

template <typename T>
constexpr bool _numeric_limits_uint128<T>::is_specialized;

template <typename T>
constexpr bool _numeric_limits_uint128<T>::is_iec559;

template <typename T>
constexpr bool _numeric_limits_uint128<T>::is_signed;

template <typename T>
constexpr bool _numeric_limits_uint128<T>::has_denorm_loss;

template <typename T>
constexpr bool _numeric_limits_uint128<T>::has_infinity;

template <typename T>
constexpr bool _numeric_limits_uint128<T>::has_quiet_NaN;

template <typename T>
constexpr bool _numeric_limits_uint128<T>::has_signaling_NaN;

template <typename T>
constexpr bool _numeric_limits_uint128<T>::tinyness_before;

template <typename T>
constexpr bool _numeric_limits_uint128<T>::traps;

template <typename T>
constexpr int _numeric_limits_uint128<T>::max_digits10;

template <typename T>
constexpr int _numeric_limits_uint128<T>::max_exponent;

template <typename T>
constexpr int _numeric_limits_uint128<T>::max_exponent10;

template <typename T>
constexpr int _numeric_limits_uint128<T>::min_exponent;

template <typename T>
constexpr int _numeric_limits_uint128<T>::min_exponent10;

template <typename T>
constexpr int _numeric_limits_uint128<T>::digits;

template <typename T>
constexpr int _numeric_limits_uint128<T>::digits10;

template <typename T>
constexpr int _numeric_limits_uint128<T>::radix;

template <typename T>
constexpr std::float_denorm_style _numeric_limits_uint128<T>::has_denorm;

template <typename T>
constexpr std::float_round_style _numeric_limits_uint128<T>::round_style;

#if defined(_MSC_VER)
#  pragma warning(default:4996)
#elif defined(__GNUC__)
#  pragma GCC diagnostic pop
#elif defined(__clang__)
  _Pragma("clang diagnostic pop")
#endif
#pragma warning(pop)

template <>
struct numeric_limits<IPADDRESS_NAMESPACE::uint128_t> : _numeric_limits_uint128<IPADDRESS_NAMESPACE::uint128_t> {
};

template <>
struct hash<IPADDRESS_NAMESPACE::uint128_t> {
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t operator()(const IPADDRESS_NAMESPACE::uint128_t& value) const IPADDRESS_NOEXCEPT {
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

IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::wstring to_wstring(const IPADDRESS_NAMESPACE::uint128_t& value) {
    return value.to_wstring();
}

template <typename T>
IPADDRESS_FORCE_INLINE std::basic_ostream<T, std::char_traits<T>>& operator<<(std::basic_ostream<T, std::char_traits<T>>& stream, const IPADDRESS_NAMESPACE::uint128_t& value) {
    auto fmt = IPADDRESS_NAMESPACE::uint128_t::format::decimal;
    if (stream.flags() & ios_base::hex) {
        fmt = IPADDRESS_NAMESPACE::uint128_t::format::hexadecimal;
    } else if (stream.flags() & ios_base::oct) {
        fmt = IPADDRESS_NAMESPACE::uint128_t::format::octal;
    }
    auto str = value.to_string(fmt);
    if (stream.flags() & ios_base::uppercase) {
        std::transform(str.cbegin(), str.cend(), str.begin(), [](char c){
            return std::toupper(c);
        });
    }
    return stream << IPADDRESS_NAMESPACE::internal::string_converter<T>::convert(str);
}

template <typename T>
IPADDRESS_FORCE_INLINE std::basic_istream<T, std::char_traits<T>>& operator>>(std::basic_istream<T, std::char_traits<T>>& stream, IPADDRESS_NAMESPACE::uint128_t& value) {
    auto fmt = IPADDRESS_NAMESPACE::uint128_t::format::decimal;
    if (stream.flags() & ios_base::hex) {
        fmt = IPADDRESS_NAMESPACE::uint128_t::format::hexadecimal;
    } else if (stream.flags() & ios_base::oct) {
        fmt = IPADDRESS_NAMESPACE::uint128_t::format::octal;
    }
    std::basic_string<T, std::char_traits<T>, std::allocator<T>> str;
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
