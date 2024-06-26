/**
 * @file      fixed-string.hpp
 * @brief     Provides a template for a fixed-length string suitable for use in non-type template parameters
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This header defines fixed_string, a template class designed to store a string of fixed size.
 * It is particularly useful in contexts where a constant string is required at compile-time,
 * such as in template metaprogramming or when specifying non-type template parameters.
 */

#ifndef IPADDRESS_FIXED_STRING_HPP
#define IPADDRESS_FIXED_STRING_HPP

#include "config.hpp"
#include "unicode.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * Fixed size string class.
 * 
 * fixed_string is a template class that encapsulates a string of a fixed number of characters.
 * It is designed to be used where strings are needed as non-type template parameters.
 * 
 * @tparam N The maximum number of characters the fixed_string can hold.
 */
IPADDRESS_EXPORT template <size_t N>
struct fixed_string {
    using value_type             = char; /**< Type of character in a string. */
    using const_pointer          = const char*; /**< Type of constant pointer to the string data. */
    using const_reference        = const char&; /**< Type of constant reference to a character in the string. */
    using const_iterator         = const_pointer; /**< Type of constant iterator for traversing the string. */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>; /**< Type of constant reverse iterator for traversing the string in reverse. */

    static constexpr size_t max_length = N;

    size_t length = 0;

    char _data[N] = {};

    /**
     * Default constructor.
     * 
     * Constructs a fixed_string with default values, initializing the string with null characters.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string() IPADDRESS_NOEXCEPT = default;

    /**
     * Constructs a fixed_string from a character array.
     * 
     * This constructor template initializes a fixed_string with the contents of a given character array.
     * 
     * @tparam T The character type of the input array.
     * @param[in] data The character array to initialize the fixed_string with.
     * @throw parse_error Thrown if contains unexpected characters for addresses
     */
    template <typename T>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const T (&data)[N + 1]) IPADDRESS_NOEXCEPT(noexcept(internal::char_reader<T>::has_throw())) {
        const auto begin = &data[0];
        const auto end = &data[N];
        auto it = begin;
        for (size_t i = 0; i < N; ++i) {
            _data[i] = internal::next_char(it, begin, end);
            if (_data[i] == '\0') {
                break;
            }
            ++length;
        }
    }

    /**
     * Constructs a fixed_string from a character array.
     * 
     * This constructor template initializes a fixed_string with the contents of a given character array.
     * 
     * @tparam T The character type of the input array.
     * @param[in] data The character array to initialize the fixed_string with.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs during parsing.
     */
    template <typename T>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const T (&data)[N + 1], error_code& code) IPADDRESS_NOEXCEPT {
        const auto begin = &data[0];
        const auto end = &data[N];
        auto it = begin;
        uint32_t error_symbol = 0;
        for (size_t i = 0; i < N; ++i) {
            _data[i] = internal::next_char_or_error(it, end, code, error_symbol);
            if (_data[i] == '\0' || code != error_code::no_error) {
                break;
            }
            ++length;
        }
    }

    /**
     * Retrieves the begin iterator of the fixed_string.
     * 
     * Returns an iterator pointing to the first character of the fixed_string.
     * If the fixed_string is empty, the returned iterator will be equal to the one returned by end().
     * 
     * @return A constant iterator to the beginning of the fixed_string.
     * @sa     end()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data);
    }
    
    /**
     * Retrieves the end iterator of the fixed_string.
     * 
     * Returns an iterator pointing to the past-the-end character of the fixed_string.
     * This iterator acts as a placeholder and should not be dereferenced.
     * 
     * @return A constant iterator to the element following the last character.
     * @sa     begin()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data) + length;
    }
    
    /**
     * Retrieves the reverse begin iterator of the fixed_string.
     * 
     * Returns a reverse iterator pointing to the last character of the fixed_string.
     * If the fixed_string is empty, the returned iterator will be equal to rend().
     * 
     * @return A constant reverse iterator to the beginning of the reversed fixed_string.
     * @sa    rend()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    /**
     * Retrieves the reverse end iterator of the fixed_string.
     * 
     * Returns a reverse iterator pointing to the position preceding the first character of the fixed_string.
     * This iterator acts as a placeholder and should not be dereferenced.
     * 
     * @return A constant reverse iterator to the end of the reversed fixed_string.
     * @sa     rbegin()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }

    /**
     * Retrieves the constant begin iterator of the fixed_string.
     * 
     * Returns a constant iterator pointing to the first character of the fixed_string.
     * If the fixed_string is empty, the returned iterator will be equal to cend().
     * 
     * @return A constant iterator to the beginning of the fixed_string.
     * @sa     cend()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    /**
     * Retrieves the constant end iterator of the fixed_string.
     * 
     * Returns a constant iterator pointing to the past-the-end character of the fixed_string.
     * This iterator acts as a placeholder and should not be dereferenced.
     * 
     * @return A constant iterator to the end of the fixed_string.
     * @sa     cbegin()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    /**
     * Retrieves the constant reverse begin iterator of the fixed_string.
     * 
     * Returns a constant reverse iterator pointing to the last character of the fixed_string.
     * If the fixed_string is empty, the returned iterator will be equal to crend().
     * 
     * @return A constant reverse iterator to the beginning of the reversed fixed_string.
     * @sa     crend()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    /**
     * Retrieves the constant reverse end iterator of the fixed_string.
     * 
     * Returns a reverse iterator pointing to the position preceding the first character of the fixed_string when reversed.
     * This iterator acts as a placeholder and should not be dereferenced.
     * 
     * @return A constant reverse iterator to the end of the reversed fixed_string.
     * @sa     crbegin()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    /**
     * Checks if the fixed_string is empty.
     * 
     * Evaluates whether the fixed_string contains no characters.
     * 
     * @return `true` if the fixed_string is empty, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool empty() const IPADDRESS_NOEXCEPT {
        return size() == 0;
    }
    
    /**
     * Retrieves the size of the fixed_string.
     * 
     * Returns the number of characters currently stored in the fixed_string.
     * 
     * @return The number of characters in the fixed_string.
     * @sa     capacity()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return length;
    }

    /**
     * Retrieves the capacity of the fixed_string.
     * 
     * Returns the total number of characters that the fixed_string can hold.
     * 
     * @return The capacity of the fixed_string.
     * @sa     size()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t capacity() const IPADDRESS_NOEXCEPT {
        return max_length;
    }

    /**
     * Accesses the character at the specified location with bounds checking.
     * 
     * Returns a reference to the character at the specified location \a n. If \a n is out of bounds,
     * an exception of type `std::out_of_range` will be thrown.
     * 
     * @param[in] n The position of the character to return.
     * @return A reference to the character at the specified location.
     * @throw std::out_of_range When going beyond the bounds of the character array.
     * @sa at(size_t) const
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference operator[](size_t n) const IPADDRESS_NOEXCEPT {
        return at(n);
    }

    /**
     * Accesses the character at the specified location with bounds checking. 
     * 
     * Returns a reference to the character at the specified location \a n. If \a n is out of bounds,
     * an exception of type `std::out_of_range` will be thrown.
     * 
     * @param[in] n The position of the character to return.
     * @return A reference to the character at the specified location.
     * @throw std::out_of_range When going beyond the bounds of the character array.
     * @sa operator[](size_t) const
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference at(size_t n) const IPADDRESS_NOEXCEPT {
        return _data[n];
    }

    /**
     * Accesses the first element.
     * 
     * Provides a reference to the first element in the string. 
     * Undefined behavior occurs if this function is called on an empty string.
     * 
     * @return Reference to the first element.
     * @sa     back()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference front() const IPADDRESS_NOEXCEPT {
        return _data[0];
    }

    /**
     * Accesses the last element.
     * 
     * Provides a reference to the last element in the string.
     * Undefined behavior occurs if this function is called on an empty string.
     * 
     * @return Reference to the last element.
     * @sa     front()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference back() const IPADDRESS_NOEXCEPT {
        return _data[N - 1];
    }

    /**
     * Provides a pointer to the underlying data.
     * 
     * Returns a pointer to the underlying array serving as the string's storage.
     * The range [data(), data() + size()) is valid even if the string is empty,
     * but the data is not dereferenceable in that case.
     * 
     * @return Pointer to the underlying data storage.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_pointer data() const IPADDRESS_NOEXCEPT {
        return _data;
    }
    
    /**
     * Compares the string with another fixed_string.
     * 
     * Compares the string with another fixed_string lexicographically.
     * 
     * @tparam N2 The size of the other fixed_string.
     * @param[in] rhs The other fixed_string to compare with.
     * @return Negative value if less, zero if equal, positive value if greater.
     */
    template <size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE int compare(const fixed_string<N2>& rhs) const IPADDRESS_NOEXCEPT {
        size_t i = 0;
        for (; i < size() && i < rhs.size(); ++i) {
            const auto c1 = at(i);
            const auto c2 = rhs.at(i);
            if (c1 != c2) {
                return int(c1) - int(c2);
            }
        }

        if (i == size() && i == rhs.size()) {
            return 0;
        } else if (i == size() && i < rhs.size()) {
            return -1;
        } else {
            return 1;
        }
    }

    /**
     * Calculates the hash of the string.
     * 
     * Computes a hash value for the string using a FNV-1a hash function.
     * 
     * @return The hash value of the string.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t hash() const IPADDRESS_NOEXCEPT {
        size_t value{};
        size_t prime{};

        if (sizeof(size_t) == 8) {
            value = size_t(14695981039346656037ULL);
            prime = size_t(1099511628211ULL);
        } else {
            value = size_t(2166136261U);
            prime = size_t(16777619U);
        }

        for (size_t i = 0; i < size(); ++i) {
            value ^= static_cast<size_t>(_data[i]);
            value *= prime;
        }
        return value;
    }

    /**
     * Swaps the contents with another fixed_string.
     * 
     * Exchanges the contents of the string with those of another fixed_string.
     * 
     * @param[in,out] other The other fixed_string to swap with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(fixed_string& other) IPADDRESS_NOEXCEPT {
        auto count = size() < other.size() ? other.size() : size();
        for (size_t i = 0; i < count; ++i) {
            auto tmp = _data[i];
            _data[i] = other._data[i];
            other._data[i] = tmp;
        }
        auto tmp = length;
        length = other.length;
        other.length = tmp;
    }
}; // fixed_string<N>

IPADDRESS_EXPORT template <>
struct fixed_string<0> {
    using value_type             = char;
    using const_pointer          = const char*;
    using const_reference        = const char&;
    using const_iterator         = const_pointer;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_t max_length = 0;

    static constexpr size_t length = 0;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string() IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const char*) IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const wchar_t*) IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const char16_t*) IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const char32_t*) IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const char*, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::no_error;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const wchar_t*, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::no_error;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const char16_t*, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::no_error;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const char32_t*, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::no_error;
    }

#if __cpp_char8_t >= 201811L
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const char8_t*) IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const char8_t*, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::no_error;
    }
#endif // __cpp_char8_t

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(nullptr);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(nullptr) + length;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool empty() const IPADDRESS_NOEXCEPT {
        return true;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t capacity() const IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference operator[](size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return at(n);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference at(size_t /*n*/) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return front();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference front() const IPADDRESS_NOEXCEPT {
        return *cbegin();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference back() const IPADDRESS_NOEXCEPT {
        return *cbegin();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_pointer data() const IPADDRESS_NOEXCEPT {
        return nullptr;
    }

    template <size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE int compare(const fixed_string<N2>& rhs) const IPADDRESS_NOEXCEPT {
        return rhs.empty() ? 0 : -1;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t hash() const IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(fixed_string& other) IPADDRESS_NOEXCEPT {
    }
}; // fixed_string<0>

/**
 * Compares the contents of two fixed strings for equality.
 * 
 * Checks if the contents of \a lhs and \a rhs are equal, meaning they have the same 
 * number of elements and each element in \a lhs compares equal with the element in \a rhs at the same position.
 * 
 * @tparam N1 The maximum number of characters of \a lhs.
 * @tparam N2 The maximum number of characters of \a rhs.
 * @param[in] lhs The fixed string whose contents to compare.
 * @param[in] rhs The fixed string whose contents to compare.
 * @return `true` if the contents of the strings are equal, `false` otherwise.
 */
IPADDRESS_EXPORT template <size_t N1, size_t N2>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
    return lhs.compare(rhs) == 0;
}

/**
 * Compares the contents of two fixed strings for inequality.
 * 
 * Checks if the contents of \a lhs and \a rhs are not equal, meaning they do not have the same 
 * number of elements or there is at least one position at which the elements in \a lhs and \a rhs differ.
 * 
 * @tparam N1 The maximum number of characters of \a lhs.
 * @tparam N2 The maximum number of characters of \a rhs.
 * @param[in] lhs The fixed string whose contents to compare.
 * @param[in] rhs The fixed string whose contents to compare.
 * @return `true` if the contents of the strings are not equal, `false` otherwise.
 */
IPADDRESS_EXPORT template <size_t N1, size_t N2>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
    return !(lhs == rhs);
}

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Compares the contents of two fixed strings lexicographically.
     * 
     * Uses the three-way comparison operator (spaceship operator) to compare the contents of \a lhs and \a rhs.
     * 
     * @tparam N1 The maximum number of characters of \a lhs.
     * @tparam N2 The maximum number of characters of \a rhs.
     * @param[in] lhs The fixed string whose contents to compare.
     * @param[in] rhs The fixed string whose contents to compare.
     * @return The relative order of the first pair of non-equivalent elements in \a lhs and \a rhs if there are such elements.
     */
    IPADDRESS_EXPORT template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        if (const auto result = lhs.compare(rhs); result == 0) {
            return std::strong_ordering::equivalent;
        } else if (result < 0) {
            return std::strong_ordering::less;
        } else {
            return std::strong_ordering::greater;
        }
    }
        
#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Compares the contents of two fixed strings lexicographically.
     * 
     * Checks if the contents of \a lhs are lexicographically less than the contents of \a rhs.
     * 
     * @tparam N1 The maximum number of characters of \a lhs.
     * @tparam N2 The maximum number of characters of \a rhs.
     * @param[in] lhs The fixed string whose contents to compare.
     * @param[in] rhs The fixed string whose contents to compare.
     * @return `true` if the contents of \a lhs are lexicographically less than the contents of \a rhs, `false` otherwise.
     */
    IPADDRESS_EXPORT template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        return lhs.compare(rhs) < 0;
    }

    /**
     * Compares the contents of two fixed strings for greater than relation.
     * 
     * Determines if the contents of \a lhs are lexicographically greater than the contents of \a rhs.
     * 
     * @tparam N1 The maximum number of characters of \a lhs.
     * @tparam N2 The maximum number of characters of \a rhs.
     * @param[in] lhs The fixed string whose contents to compare.
     * @param[in] rhs The fixed string whose contents to compare.
     * @return `true` if \a lhs is lexicographically greater than \a rhs, `false` otherwise.
     */
    IPADDRESS_EXPORT template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        return rhs < lhs;
    }

    /**
     * Compares the contents of two fixed strings for less than or equal relation.
     * 
     * Determines if the contents of \a lhs are lexicographically less than or equal to the contents of \a rhs.
     * 
     * @tparam N1 The maximum number of characters of \a lhs.
     * @tparam N2 The maximum number of characters of \a rhs.
     * @param[in] lhs The fixed string whose contents to compare.
     * @param[in] rhs The fixed string whose contents to compare.
     * @return `true` if \a lhs is lexicographically less than or equal to \a rhs, `false` otherwise.
     */
    IPADDRESS_EXPORT template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        return !(rhs < lhs);
    }

    /**
     * Compares the contents of two fixed strings for greater than or equal relation.
     * 
     * Determines if the contents of \a lhs are lexicographically greater than or equal to the contents of \a rhs.
     * 
     * @tparam N1 The maximum number of characters of \a lhs.
     * @tparam N2 The maximum number of characters of \a rhs.
     * @param[in] lhs The fixed string whose contents to compare.
     * @param[in] rhs The fixed string whose contents to compare.
     * @return `true` if \a lhs is lexicographically greater than or equal to \a rhs, `false` otherwise.
     */
    IPADDRESS_EXPORT template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        return !(lhs < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

/**
 * Creates a fixed-length string from a character array.
 * 
 * Constructs a fixed_string object from a character array, deducing the size automatically.
 * 
 * @tparam T The character type of the input array.
 * @tparam N The size of the character array plus one for the null terminator.
 * @param[in] data The character array to initialize the fixed_string with.
 * @return A fixed_string object of size N-1.
 */
IPADDRESS_EXPORT template <typename T, size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string<N - 1> make_fixed_string(const T(&data)[N]) IPADDRESS_NOEXCEPT(noexcept(fixed_string<N - 1>(data))) {
    return fixed_string<N - 1>(data);
}

/**
 * Creates a fixed-length string from a character array.
 * 
 * Constructs a fixed_string object from a character array, deducing the size automatically.
 * 
 * @tparam T The character type of the input array.
 * @tparam N The size of the character array plus one for the null terminator.
 * @param[in] data The character array to initialize the fixed_string with.
 * @param[out] code A reference to an `error_code` object that will be set if an error occurs during parsing.
 * @return A fixed_string object of size N-1.
 */
IPADDRESS_EXPORT template <typename T, size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string<N - 1> make_fixed_string(const T(&data)[N], error_code& code) IPADDRESS_NOEXCEPT {
    return fixed_string<N - 1>(data, code);
}

#if IPADDRESS_CPP_VERSION >= 17

#if __cpp_char8_t >= 201811L
    IPADDRESS_EXPORT template <size_t N> fixed_string(const char8_t(&)[N]) -> fixed_string<N - 1>;
#endif // __cpp_char8_t
    IPADDRESS_EXPORT template <size_t N> fixed_string(fixed_string<N>) -> fixed_string<N>;
    IPADDRESS_EXPORT template <size_t N> fixed_string(const char(&)[N]) -> fixed_string<N - 1>;
    IPADDRESS_EXPORT template <size_t N> fixed_string(const wchar_t(&)[N]) -> fixed_string<N - 1>;
    IPADDRESS_EXPORT template <size_t N> fixed_string(const char16_t(&)[N]) -> fixed_string<N - 1>;
    IPADDRESS_EXPORT template <size_t N> fixed_string(const char32_t(&)[N]) -> fixed_string<N - 1>;
    IPADDRESS_EXPORT fixed_string() -> fixed_string<0>;

#endif

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_FIXED_STRING_HPP
