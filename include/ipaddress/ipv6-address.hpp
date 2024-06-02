/**
 * @file      ipv6-address.hpp
 * @brief     Provides a set of functions and classes for handling IPv6 addresses
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * Includes functionalities to convert IPv6 addresses to and from various formats,
 * perform comparisons, and query specific properties of the addresses.
 * It serves as a foundational component for network applications that require manipulation
 * and analysis of IPv6 address data.
 */

#ifndef IPADDRESS_IPV6_ADDRESS_HPP
#define IPADDRESS_IPV6_ADDRESS_HPP

#include "ipv4-address.hpp"
#include "base-v6.hpp"

namespace IPADDRESS_NAMESPACE {

namespace internal {

template <typename T>
struct ipv6_set_scope {
    template <typename Str>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void change(fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH>& result, const Str& scope) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        error_code code = error_code::no_error;
        uint32_t index = 0;
        change(result, scope, code, index);
    #ifndef IPADDRESS_NO_EXCEPTIONS
        if (code != error_code::no_error) {
            raise_error(code, index, scope.data(), scope.size());
        }
    #endif // !IPADDRESS_NO_EXCEPTIONS
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }
    
    template <typename Str>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void change(fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH>& result, const Str& scope, error_code& code) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        uint32_t index = 0;
        change(result, scope, code, index);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }
    
    template <typename Str>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void change(fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH>& result, const Str& scope, error_code& code, uint32_t& index) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        if (scope.size() > IPADDRESS_IPV6_SCOPE_MAX_LENGTH) {
            code = error_code::scope_id_is_too_long;
            return;
        }
        char str[IPADDRESS_IPV6_SCOPE_MAX_LENGTH + 1] = {};
        auto it = scope.data();
        auto end = it + scope.size();
        uint32_t error_symbol = 0;
        code = error_code::no_error;
        index = 0;
        for (int i = 0; it < end; ++i) {
            auto c = internal::next_char_or_error(it, end, code, error_symbol);
            if (code != error_code::no_error) {
                index = error_symbol;
                return;
            }
            if (internal::is_invalid_scope_id_symbol(c)) {
                code = error_code::invalid_scope_id;
                return;
            }
            str[i] = c;
        }
        const auto fixed_scope = make_fixed_string(str, code);
        if (code == error_code::no_error) {
            result = fixed_scope;
        }
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }
};

} // namespace IPADDRESS_NAMESPACE::internal

/**
 * Represents the scope identifier for an IPv6 address.
 * 
 * The scope identifier is used to distinguish between different zones for the same address,
 * such as link-local or site-local scopes. This class provides methods to set, retrieve,
 * and compare scope identifiers in both string and numeric formats.
 */
IPADDRESS_EXPORT class scope final {
public:
    /**
     * Constructs a scope object with a given scope identifier.
     * 
     * @param[in] scope_id A fixed_string representing the scope identifier.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE scope(const fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH>& scope_id) IPADDRESS_NOEXCEPT
        : 
        _scope_id(scope_id) {
        parse_value();
    }

    /**
     * Retrieves the scope identifier as a string.
     * 
     * @return A `std::string` representing the scope identifier.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string get_string() const {
        return std::string(_scope_id.begin(), _scope_id.end());
    }

    /**
     * Retrieves the scope identifier as a numeric value.
     * 
     * @return A uint32_t representing the numeric value of the scope identifier.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t get_uint32() const IPADDRESS_NOEXCEPT {
        return _scope_id_value;
    }

    /**
     * Checks if the scope identifier has a string representation.
     * 
     * @return `true` if a string representation exists, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool has_string() const IPADDRESS_NOEXCEPT {
        return !_scope_id.empty();
    }

    /**
     * Checks if the scope identifier has a numeric representation.
     * 
     * @return `true` if a numeric representation exists, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool has_uint32() const IPADDRESS_NOEXCEPT {
        return _has_value;
    }

    /**
     * Converts the scope object to a boolean value based on the presence of a scope representation.
     * 
     * @return `true` if a scope exists, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator bool() const IPADDRESS_NOEXCEPT {
        return has_string();
    }

    /**
     * Converts the scope object to a string representation.
     * 
     * @return A std::string representing the scope identifier.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE operator std::string() const {
        return get_string();
    }

    /**
     * Converts the scope object to a numeric representation.
     * 
     * @return A uint32_t representing the numeric value of the scope identifier.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE operator uint32_t() const IPADDRESS_NOEXCEPT {
        return get_uint32();
    }

    /**
     * Compares two scope objects for equality.
     * 
     * @param[in] rhs The right-hand side scope object for comparison.
     * @return `true` if both scope objects are equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return _scope_id == rhs._scope_id;
    }

    /**
     * Compares two scope objects for inequality.
     * 
     * @param[in] rhs The right-hand side scope object for comparison.
     * @return `true` if both scope objects are not equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Compares two scope objects using the three-way comparison operator (spaceship operator).
     * 
     * @param[in] rhs The right-hand side scope object for comparison.
     * @return A value of type `std::strong_ordering` that represents the result of the comparison.
     */
     IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const scope& rhs) const IPADDRESS_NOEXCEPT {
         return _scope_id <=> rhs._scope_id;
     }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Checks if one scope object is less than another.
     * 
     * @param[in] rhs The right-hand side scope object for comparison.
     * @return `true` if the left-hand side scope object is less than the right-hand side, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return _scope_id < rhs._scope_id;
    }
    
    /**
     * Checks if one scope object is greater than another.
     * 
     * @param[in] rhs The right-hand side scope object for comparison.
     * @return `true` if the left-hand side scope object is greater than the right-hand side, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    /**
     * Checks if one scope object is less than or equal to another.
     * 
     * @param[in] rhs The right-hand side scope object for comparison.
     * @return `true` if the left-hand side scope object is less than or equal to the right-hand side, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    /**
     * Checks if one scope object is greater than or equal to another.
     * 
     * @param[in] rhs The right-hand side scope object for comparison.
     * @return `true` if the left-hand side scope object is greater than or equal to the right-hand side, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void parse_value() IPADDRESS_NOEXCEPT {
        if (!_scope_id.empty()) {
            _has_value = true;
            uint32_t value = 0;
            const auto scope_id_size = _scope_id.size();
            for (size_t i = 0; i < scope_id_size; ++i) {
                const auto c = _scope_id[i];
                if (c >= '0' && c <= '9') {
                    value = value * 10 + (c - '0');
                } else {
                    _has_value = false;
                    break;
                }
            }
            _scope_id_value = _has_value ? value : 0;
        }
    }

    fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH> _scope_id;
    uint32_t _scope_id_value = 0;
    bool _has_value = false;
}; // scope

/**
 * Represents the base class for IPv6 address manipulation.
 * 
 * This class provides the basic functionalities required for handling IPv6 addresses,
 * including conversion to and from numeric representations, access to the underlying bytes,
 * and utility functions that are common across different representations of IPv6 addresses.
 */
IPADDRESS_EXPORT class ipv6_address_base : public base_v6<ipv6_address_base> {
public:
    using base_type = typename base_v6<ipv6_address_base>::base_type; /**< The base type for the IPv6 address. */
    using uint_type = typename base_v6<ipv6_address_base>::uint_type; /**< The unsigned integer type for the IPv6 address. */

    /**
     * Retrieves the scope identifier of the IPv6 address.
     * 
     * The scope identifier is used to determine the context in which the address is valid.
     * It is particularly relevant for link-local and site-local addresses.
     * 
     * @return A `scope` object representing the scope identifier of the IPv6 address.
     * @remark If the scope is disabled in the settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`), then an empty scope will be returned.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE scope get_scope_id() const IPADDRESS_NOEXCEPT {
        return scope(
        #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
            _data.scope_id
        #else // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
            make_fixed_string("")
        #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
        );
    }

    /**
     * Sets the scope identifier of the IPv6 address.
     * 
     * This function sets the scope identifier using a character array. The length of the array
     * should not exceed `IPADDRESS_IPV6_SCOPE_MAX_LENGTH + 1`.
     * 
     * @tparam T The character type of the scope identifier.
     * @tparam N The size of the scope identifier array.
     * @param[in] scope_id The character array representing the scope identifier.
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     */
    template <typename T, size_t N>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(const T(&scope_id)[N]) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        static_assert(N <= IPADDRESS_IPV6_SCOPE_MAX_LENGTH + 1, "scope id is too long");
    #ifdef IPADDRESS_NO_EXCEPTIONS
        auto code = error_code::no_error;
        const auto result = make_fixed_string(scope_id, code);
        if (code == error_code::no_error) {
            internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, result);
        }
    #else // !IPADDRESS_NO_EXCEPTIONS
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, make_fixed_string(scope_id));
    #endif // !IPADDRESS_NO_EXCEPTIONS
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address and reports any errors encountered.
     * 
     * This function sets the scope identifier using a character array. The length of the array
     * should not exceed `IPADDRESS_IPV6_SCOPE_MAX_LENGTH + 1`.
     * 
     * @tparam T The character type of the scope identifier.
     * @tparam N The size of the scope identifier array.
     * @param[in] scope_id The character array representing the scope identifier.
     * @param[out] code An error_code object that will be set to the error that occurred, if any.
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     */
    template <typename T, size_t N>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(const T(&scope_id)[N], error_code& code) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        static_assert(N <= IPADDRESS_IPV6_SCOPE_MAX_LENGTH + 1, "scope id is too long");
        const auto fixed_scope = make_fixed_string(scope_id, code);
        if (code == error_code::no_error) {
            internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, fixed_scope, code);
        }
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

#if IPADDRESS_CPP_VERSION >= 17

    /**
     * Sets the scope identifier of the IPv6 address using a string view.
     * 
     * This function sets the scope identifier using a string view, which allows for a more efficient
     * way to handle strings without copying them.
     * 
     * @param[in] scope_id The string view representing the scope identifier.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::string` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address using a wide string view.
     * 
     * This function sets the scope identifier using a wide string view, which allows for a more efficient
     * way to handle strings without copying them.
     * 
     * @param[in] scope_id The wide string view representing the scope identifier.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::wstring` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::wstring_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

#if __cpp_char8_t >= 201811L

    /**
     * Sets the scope identifier of the IPv6 address using UTF-8 string view.
     * 
     * This function sets the scope identifier using UTF-8 string view, which allows for a more efficient
     * way to handle strings without copying them.
     * 
     * @param[in] scope_id The UTF-8 string view representing the scope identifier.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++20 and later versions where `char8_t` is supported.
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u8string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

#endif // __cpp_char8_t

    /**
     * Sets the scope identifier of the IPv6 address using UTF-16 string view.
     * 
     * This function sets the scope identifier using UTF-16 string view, which allows for a more efficient
     * way to handle strings without copying them.
     * 
     * @param[in] scope_id The UTF-16 string view representing the scope identifier.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::u16string` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u16string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address using UTF-32 string view.
     * 
     * This function sets the scope identifier using UTF-32 string view, which allows for a more efficient
     * way to handle strings without copying them.
     * 
     * @param[in] scope_id The UTF-32 string view representing the scope identifier.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::u32string` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u32string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address using a string view and reports any errors encountered.
     * 
     * This function sets the scope identifier using a string view and provides an error code parameter to report any issues that occur during the operation.
     * 
     * @param[in] scope_id The string view representing the scope identifier.
     * @param[out] code An error_code object that will be set to the error that occurred, if any.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::string` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id, code);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address using a wide string view and reports any errors encountered.
     * 
     * This function sets the scope identifier using a wide string view and provides an error code parameter to report any issues that occur during the operation.
     * 
     * @param[in] scope_id The wide string view representing the scope identifier.
     * @param[out] code An error_code object that will be set to the error that occurred, if any.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::wstring` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::wstring_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id, code);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

#if __cpp_char8_t >= 201811L

    /**
     * Sets the scope identifier of the IPv6 address using a UTF-8 string view and reports any errors encountered.
     * 
     * This function sets the scope identifier using a UTF-8 string view and provides an error code parameter to report any issues that occur during the operation.
     * 
     * @param[in] scope_id The UTF-8 string view representing the scope identifier.
     * @param[out] code An error_code object that will be set to the error that occurred, if any.
     * @note This method is available for C++20 and later versions.
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u8string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id, code);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

#endif // __cpp_char8_t

    /**
     * Sets the scope identifier of the IPv6 address using a UTF-16 string view and reports any errors encountered.
     * 
     * This function sets the scope identifier using a UTF-16 string view and provides an error code parameter to report any issues that occur during the operation.
     * 
     * @param[in] scope_id The UTF-16 string view representing the scope identifier.
     * @param[out] code An error_code object that will be set to the error that occurred, if any.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::u16string` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u16string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id, code);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address using a UTF-32 string view and reports any errors encountered.
     * 
     * This function sets the scope identifier using a UTF-32 string view and provides an error code parameter to report any issues that occur during the operation.
     * 
     * @param[in] scope_id The UTF-32 string view representing the scope identifier.
     * @param[out] code An error_code object that will be set to the error that occurred, if any.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::u32string` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u32string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id, code);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

#else // IPADDRESS_CPP_VERSION < 17

    /**
     * Sets the scope identifier of the IPv6 address using a `std::string`.
     * 
     * @param[in] scope_id The string representing the scope identifier.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::string& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::wstring`.
     * 
     * @param[in] scope_id The wide string representing the scope identifier.
     * @throw parse_error Exception caused by invalid input wide string.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::wstring& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::u16string`.
     * 
     * @param[in] scope_id The UTF-16 string representing the scope identifier.
     * @throw parse_error Exception caused by invalid input UTF-16 string.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u16string& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::u32string`.
     * 
     * @param[in] scope_id The UTF-32 string representing the scope identifier.
     * @throw parse_error Exception caused by invalid input UTF-32 string.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u32string& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::string` and reports any errors.
     * 
     * @param[in] scope_id The string representing the scope identifier.
     * @param[out] code An error_code object that will store the result of the operation.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::string& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id, code);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::wstring` and reports any errors.
     * 
     * @param[in] scope_id The wide string representing the scope identifier.
     * @param[out] code An error_code object that will store the result of the operation.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::wstring& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id, code);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::u16string` and reports any errors.
     * 
     * @param[in] scope_id The UTF-16 string representing the scope identifier.
     * @param[out] code An error_code object that will store the result of the operation.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u16string& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id, code);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::u32string` and reports any errors.
     * 
     * @param[in] scope_id The UTF-32 string representing the scope identifier.
     * @param[out] code An error_code object that will store the result of the operation.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u32string& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        internal::ipv6_set_scope<ipv6_address_base>::change(_data.scope_id, scope_id, code);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

#endif // IPADDRESS_CPP_VERSION < 17

    /**
     * Creates an IPv6 address from an unsigned integer using a template parameter.
     * 
     * @param[in] ip The unsigned integer representing the IPv6 address.
     * @return An instance of ip address representing the IPv6 address.
     */
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base<ipv6_address_base> from_uint(uint_type ip) IPADDRESS_NOEXCEPT {
        ip_address_base<ipv6_address_base> result;
        auto& bytes = result._data.bytes;
        int shift = 0;
        int inc = 8;
        if (is_little_endian()) {
            shift = 56;
            inc = -8;
        }
        for (int i = 0, s = shift; i < 8; ++i, s += inc) {
            bytes[i] = uint8_t(ip.upper() >> s);
            bytes[i + 8] = uint8_t(ip.lower() >> s);
        }
        return result;
    }

    /**
     * Converts the IPv6 address to an unsigned integer.
     * 
     * @return The unsigned integer representation of the IPv6 address.
     * @remark Bytes in integer are presented in **host byte order**.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint_type to_uint() const IPADDRESS_NOEXCEPT {
        const auto& bytes = _data.bytes;
        uint64_t upper = 0;
        uint64_t lower = 0;
        int shift = 0;
        int inc = 8;
        if (is_little_endian()) {
            shift = 56;
            inc = -8;
        }
        for (int i = 0, s = shift; i < 8; ++i, s += inc) {
            upper |= uint64_t(bytes[i]) << s;
            lower |= uint64_t(bytes[i + 8]) << s;
        }
        return uint_type(upper, lower);
    }

    /**
     * Provides access to the underlying bytes of the IPv6 address.
     * 
     * @return A reference to the base type containing the bytes of the IPv6 address.
     * @remark Retrieves the data representing the IP address in **network byte order** (big-endian).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const base_type& bytes() const IPADDRESS_NOEXCEPT {
        return _data.bytes;
    }

    /**
     * Determines if the IPv6 address is an IPv4-mapped address.
     * 
     * @return An `optional` containing the mapped IPv4 address if the IPv6 address is IPv4-mapped, or an empty `optional` otherwise.
     * @remark An IPv4-mapped IPv6 address has its first 80 bits set to zero and the next 16 bits set to one (starting with `::FFFF/96`).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_address> ipv4_mapped() const IPADDRESS_NOEXCEPT {
        const auto& b = bytes();
        if (b[10] != 0xFF || b[11] != 0xFF) {
            return nullptr;
        } 
        ipv4_address::base_type ipv4_bytes = { b[12], b[13], b[14], b[15] };
        return ipv4_address(ipv4_bytes);
    }

    /**
     * Determines if the IPv6 address is a 6to4 address.
     * 
     * @return An optional containing the encapsulated IPv4 address if the IPv6 address is a 6to4 address, or an empty optional otherwise.
     * @remark A 6to4 address uses a `2002::/16` prefix and embeds an IPv4 address in the next 32 bits.
     * @see [RFC 3056](https://datatracker.ietf.org/doc/html/rfc3056.html).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_address> sixtofour() const IPADDRESS_NOEXCEPT {
        const auto& b = bytes();
        if (b[0] != 0x20 || b[1] != 0x02) {
            return nullptr;
        }
        ipv4_address::base_type ipv4_bytes = { b[2], b[3], b[4], b[5] };
        return ipv4_address(ipv4_bytes);
    }

    /**
     * Determines if the IPv6 address is a Teredo address.
     * 
     * @return An optional containing a pair of IPv4 addresses representing the Teredo server and client if the IPv6 address is a Teredo address, or an empty optional otherwise.
     * @retval std::pair::first The Teredo server IPv4 address
     * @retval std::pair::second The Teredo client IPv4 address
     * @remark A Teredo address begins with the `2001::/32` prefix and is used for NAT traversal for IPv6.
     * @see [RFC 4380](https://datatracker.ietf.org/doc/html/rfc4380.html).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<std::pair<ipv4_address, ipv4_address>> teredo() const IPADDRESS_NOEXCEPT {
        const auto& b = bytes();
        if (b[0] != 0x20 || b[1] != 0x01 || b[2] != 0x00 || b[3] != 0x00) {
            return nullptr;
        }
        ipv4_address::base_type server_bytes = { b[4], b[5], b[6], b[7] };
        ipv4_address::base_type client_bytes = { uint8_t(~b[12]), uint8_t(~b[13]), uint8_t(~b[14]), uint8_t(~b[15]) };
        return std::make_pair(ipv4_address(server_bytes), ipv4_address(client_bytes));
    }

    /**
     * Checks if the IPv6 address is a site-local address.
     * 
     * @return `true` if the address is site-local, `false` otherwise.
     * @note Site-local addresses are equivalent to private addresses in IPv4 and are not routable on the global internet.
     * @remark These attribute is true for the network as a whole if it is true for both the network address and the broadcast address.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_site_local() const IPADDRESS_NOEXCEPT;

protected:
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_address_base() IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ipv6_address_base(const base_type& bytes) IPADDRESS_NOEXCEPT : _data(bytes) {
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(ip_address_base<ipv6_address_base>& lhs, ip_address_base<ipv6_address_base>& rhs) IPADDRESS_NOEXCEPT {
        lhs._data.bytes.swap(rhs._data.bytes);
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        lhs._data.scope_id.swap(rhs._data.scope_id);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t hash(const base_type& bytes) const IPADDRESS_NOEXCEPT {
        return internal::calc_hash(
        #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
            _data.scope_id.hash(),
        #else // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
            0,
        #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
            size_t(bytes[0]), size_t(bytes[1]), size_t(bytes[2]), size_t(bytes[3]), 
            size_t(bytes[4]), size_t(bytes[5]), size_t(bytes[6]), size_t(bytes[7]), 
            size_t(bytes[8]), size_t(bytes[9]), size_t(bytes[10]), size_t(bytes[11]), 
            size_t(bytes[12]), size_t(bytes[13]), size_t(bytes[14]), size_t(bytes[15]));
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool equals(const ip_address_base<ipv6_address_base>& lhs, const ip_address_base<ipv6_address_base>& rhs) IPADDRESS_NOEXCEPT {
        return lhs._data.bytes == rhs._data.bytes
        
        #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
            && lhs._data.scope_id.compare(rhs._data.scope_id) == 0
        #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
            ;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool less(const ip_address_base<ipv6_address_base>& lhs, const ip_address_base<ipv6_address_base>& rhs) IPADDRESS_NOEXCEPT {
        return lhs._data.bytes < rhs._data.bytes 
        
        #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
            ? true : lhs._data.scope_id.compare(rhs._data.scope_id) < 0;
        #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
            ;
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering compare(const ip_address_base<ipv6_address_base>& lhs, const ip_address_base<ipv6_address_base>& rhs) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        if (auto result = lhs._data.bytes <=> rhs._data.bytes; result == std::strong_ordering::equivalent) {
            if (const auto scope = lhs._data.scope_id.compare(rhs._data.scope_id); scope < 0) {
                return std::strong_ordering::less;
            } else if (scope == 0) {
                return std::strong_ordering::equivalent;
            } else {
                return std::strong_ordering::greater;
            }
        } else {
            return result;
        }
    #else // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
        return lhs._data.bytes <=> rhs._data.bytes;
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
    }

#endif // IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t ip_to_chars(const base_type& bytes, format fmt, char (&result)[base_max_string_len + 1]) const IPADDRESS_NOEXCEPT {
        return base_v6<ipv6_address_base>::ip_to_chars(bytes, 

        #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
            _data.scope_id,
        #else // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
            make_fixed_string(""),
        #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
            fmt,
            result);
    }

private:
    friend base_v6<ipv6_address_base>;

    template <typename>
    friend class ip_network_base;

    struct ipv6_data {
        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_data() IPADDRESS_NOEXCEPT = default;
        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_data(const base_type& b) IPADDRESS_NOEXCEPT : bytes(b) {
        }

        base_type bytes{};
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH> scope_id;
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    } _data;
}; // ipv6_address_base

/**
 * Alias for the base class specialized for IPv6 address manipulation.
 * 
 * This alias provides a convenient shorthand for the specialized `ip_address_base` class
 * tailored for IPv6 address handling. It inherits all functionalities from the `ipv6_address_base`
 * class, allowing for operations such as conversion, comparison, and property querying
 * specific to IPv6 addresses.
 */
IPADDRESS_EXPORT using ipv6_address = ip_address_base<ipv6_address_base>;

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * User-defined literal for creating an ipv6_address from a fixed string at compile time.
     * 
     * @tparam FixedString A compile-time fixed string representing the IPv6 address.
     * @return An ipv6_address object parsed from the fixed string.
     */
    IPADDRESS_EXPORT template <fixed_string FixedString>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEVAL IPADDRESS_FORCE_INLINE ipv6_address operator""_ipv6() IPADDRESS_NOEXCEPT {
        return ipv6_address::parse<FixedString>();
    }

#else // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * User-defined literal for creating an ipv6_address from a string literal.
     * 
     * @param[in] address A pointer to a character array representing the IPv6 address.
     * @param[in] size The size of the character array.
     * @return An ipv6_address object parsed from the string literal.
     */
    IPADDRESS_EXPORT IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_address operator""_ipv6(const char* address, size_t size) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::parse_ip_from_literal<ipv6_address_base, char, ipv6_address::base_max_string_len>(address, size);
    }

    /**
     * User-defined literal for creating an ipv6_address from a wide string literal.
     * 
     * @param[in] address A pointer to a character array representing the IPv6 address.
     * @param[in] size The size of the character array.
     * @return An ipv6_address object parsed from the string literal.
     */
    IPADDRESS_EXPORT IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_address operator""_ipv6(const wchar_t* address, size_t size) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::parse_ip_from_literal<ipv6_address_base, wchar_t, ipv6_address::base_max_string_len>(address, size);
    }

    /**
     * User-defined literal for creating an ipv6_address from a UTF-16 string literal.
     * 
     * @param[in] address A pointer to a character array representing the IPv6 address.
     * @param[in] size The size of the character array.
     * @return An ipv6_address object parsed from the string literal.
     */
    IPADDRESS_EXPORT IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_address operator""_ipv6(const char16_t* address, size_t size) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::parse_ip_from_literal<ipv6_address_base, char16_t, ipv6_address::base_max_string_len>(address, size);
    }

    /**
     * User-defined literal for creating an ipv6_address from a UTF-32 string literal.
     * 
     * @param[in] address A pointer to a character array representing the IPv6 address.
     * @param[in] size The size of the character array.
     * @return An ipv6_address object parsed from the string literal.
     */
    IPADDRESS_EXPORT IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_address operator""_ipv6(const char32_t* address, size_t size) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::parse_ip_from_literal<ipv6_address_base, char32_t, ipv6_address::base_max_string_len>(address, size);
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IPV6_ADDRESS_HPP
