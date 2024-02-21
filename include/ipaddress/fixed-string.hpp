/**
 * \file      fixed-string.hpp
 * \brief     Fixed string for non-type template parameters syntax
 * \author    Vladimir Shaleev
 * \copyright MIT License
 * 
 * Template for fixed length string.
 */

#ifndef IPADDRESS_FIXED_STRING_HPP
#define IPADDRESS_FIXED_STRING_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

namespace internal {

template <typename T>
IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void is_char_type() IPADDRESS_NOEXCEPT {
    static_assert(std::is_same<T, char>::value 
        || std::is_same<T, signed char>::value 
        || std::is_same<T, unsigned char>::value 
        || std::is_same<T, wchar_t>::value 
        || std::is_same<T, char16_t>::value 
        || std::is_same<T, char32_t>::value
    #if __cpp_char8_t >= 201811L
        || std::is_same<T, char8_t>::value
    #endif // __cpp_char8_t
        , "Only character type supported");
}

} // namespace internal

/**
 * Fixed size string.
 * 
 * Used in non-type template parameters syntax.
 * 
 * @tparam N is maximum number of characters.
 */
template <size_t N>
struct fixed_string {
    using const_pointer          = const char*; /**< Type of constant pointer */
    using const_reference        = const char&; /**< Type of constant reference */
    using const_iterator         = const_pointer; /**< Type of constant iterator */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>; /**< Type of reverse constant iterator */

    static IPADDRESS_CONSTEXPR size_t max_length = N;

    size_t length = 0;

    char _data[N] = {};

    /**
     * Default constructor.
     * 
     * Creates an instance with default values, i.e. a string filled with null characters.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string() IPADDRESS_NOEXCEPT = default;

    /**
     * Creates an instance from a character array.
     * 
     * All characters will be converted to ASCII, with possible truncation if characters 
     * from other encodings are used.
     * 
     * @tparam T is character type
     * @param[in] data of character array
     */
    template <typename T>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const T (&data)[N + 1]) IPADDRESS_NOEXCEPT {
        internal::is_char_type<T>();
        auto ended = false;
        for (size_t i = 0; i < N; ++i) {
            if (IPADDRESS_IS_CONST_EVALUATED(data) && data[i] > 127) {
                size_t i = data[i] / (data[i] - data[i]); // invalid symbol for ip address
            }
            _data[i] = char(data[i]);
            if (data[i] == '\0') {
                ended = true;
            }
            if (!ended) {
                ++length;
            }
        }
    }

    /**
     * Get begin iterator.
     * 
     * Returns an iterator to the first element of the string. 
     * If the string is empty, the returned iterator will be equal to end().
     * 
     * @return Iterator to the first element.
     * 
     * @sa end()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data);
    }
    
    /**
     * Get end iterator.
     * 
     * Returns an iterator to the element following the last element of the string. 
     * This element acts as a placeholder; attempting to access it results in undefined behavior.
     * 
     * @return Iterator to the element following the last element.
     * 
     * @sa begin()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data) + length;
    }
    
    /**
     * Get reverse begin iterator.
     * 
     * Returns a reverse iterator to the first element of the reversed string. 
     * It corresponds to the last element of the non-reversed string. If the string 
     * is empty, the returned iterator is equal to rend().
     * 
     * @return Reverse iterator to the first element.
     * 
     * @sa rend()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    /**
     * Get reverse end iterator.
     * 
     * Returns a reverse iterator to the element following the last element of the 
     * reversed string. It corresponds to the element preceding the first element of 
     * the non-reversed string. This element acts as a placeholder, attempting to 
     * access it results in undefined behavior.
     * 
     * @return Reverse iterator to the element following the last element.
     * 
     * @sa rbegin()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }

    /**
     * Get begin iterator.
     * 
     * Returns an iterator to the first element of the string. 
     * If the string is empty, the returned iterator will be equal to cend().
     * 
     * @return Iterator to the first element.
     * 
     * @sa cend()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    /**
     * Get end iterator.
     * 
     * Returns an iterator to the element following the last element of the string. 
     * This element acts as a placeholder; attempting to access it results in undefined behavior.
     * 
     * @return Iterator to the element following the last element.
     * 
     * @sa cbegin()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    /**
     * Get reverse begin iterator.
     * 
     * Returns a reverse iterator to the first element of the reversed string. 
     * It corresponds to the last element of the non-reversed string. If the string 
     * is empty, the returned iterator is equal to crend().
     * 
     * @return Reverse iterator to the first element.
     * 
     * @sa crend()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    /**
     * Get reverse end iterator.
     * 
     * Returns a reverse iterator to the element following the last element of the 
     * reversed string. It corresponds to the element preceding the first element of 
     * the non-reversed string. This element acts as a placeholder, attempting to 
     * access it results in undefined behavior.
     * 
     * @return Reverse iterator to the element following the last element.
     * 
     * @sa crbegin()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    /**
     * Checks if the string is empty.
     * 
     * @return `true` if the string is empty, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool empty() const IPADDRESS_NOEXCEPT {
        return size() == 0;
    }
    
    /**
     * Returns the number of characters in the string.
     * 
     * @return The number of characters in the string.
     * 
     * @sa capacity()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return length;
    }

    /**
     * Returns the number of characters that the string has currently allocated space for.
     * 
     * @return Capacity of the currently allocated storage.
     * 
     * @sa size()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t capacity() const IPADDRESS_NOEXCEPT {
        return max_length;
    }

    /**
     * Returns a reference to the element at specified location \a n.
     * 
     * If n is not within the range of the string, an exception of type std::out_of_range is thrown.
     * 
     * @param[in] n position of the element to return
     * @return Reference to the requested element.
     * 
     * @sa at(size_t) const
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference operator[](size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return at(n);
    }

    /**
     * Returns a reference to the element at specified location \a n, with bounds checking. 
     * 
     * If n is not within the range of the string, an exception of type std::out_of_range is thrown.
     * 
     * @param[in] n position of the element to return
     * @return Reference to the requested element.
     * 
     * @sa operator[](size_t) const
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference at(size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (n >= 0 && n < N) {
            return _data[n];
        }
    #ifdef IPADDRESS_NO_EXCEPTIONS
        return *_data;
    #else
        throw std::out_of_range("index out of array");
    #endif
    }

    /**
     * Returns a reference to the first element in the string. 
     * 
     * Calling front on an empty string causes undefined behavior.
     * 
     * @return Reference to the first element.
     * 
     * @sa back()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference front() const IPADDRESS_NOEXCEPT {
        return _data[0];
    }

    /**
     * Returns a reference to the last element in the container.
     * 
     * Calling back on an empty container causes undefined behavior.
     * 
     * @return Reference to the last element.
     * 
     * @sa front()
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference back() const IPADDRESS_NOEXCEPT {
        return _data[N - 1];
    }

    /**
     * Returns pointer to the underlying array serving as element storage. 
     * 
     * The pointer is such that range [data(), data() + size()) is always a valid range, 
     * even if the string is empty (data() is not dereferenceable in that case).
     * 
     * @return Pointer to the underlying element storage. For non-empty containers, 
     * the returned pointer compares equal to the address of the first element.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_pointer data() const IPADDRESS_NOEXCEPT {
        return _data;
    }
    
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
     * Calculate hash of string.
     * 
     * Returns a value of type std::size_t that represents the hash value.
     * 
     * @return Hash of string.
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
     * Swap two string.
     * 
     * Exchanges the given values.
     * 
     * @param[in,out] other the value to be swapped
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

template <>
struct fixed_string<0> {
    using const_pointer          = const char*;
    using const_reference        = const char&;
    using const_iterator         = const_pointer;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static IPADDRESS_CONSTEXPR size_t max_length = 0;

    static IPADDRESS_CONSTEXPR size_t length = 0;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string() IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const char*) IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const wchar_t*) IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const char16_t*) IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const char32_t*) IPADDRESS_NOEXCEPT {
    }

#if __cpp_char8_t >= 201811L
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string(const char8_t*) IPADDRESS_NOEXCEPT {
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

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference at(size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
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
 * Compares the contents of two fixed strings.
 * 
 * Checks if the contents of  \a lhs and rhs are equal, that is, they have the same 
 * number of elements and each element in \a lhs compares equal with the element in rhs at the same position.
 * 
 * @tparam N1 is maximum number of characters of \a lhs.
 * @tparam N2 is maximum number of characters of \a rhs.
 * @param[in] lhs string whose contents to compare
 * @param[in] rhs string whose contents to compare
 * @return `true` if the contents of the strings are equal, `false` otherwise.
 */
template <size_t N1, size_t N2>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
    return lhs.compare(rhs) == 0;
}

/**
 * Compares the contents of two fixed strings.
 * 
 * Checks if the contents of  \a lhs and rhs are equal, that is, they have the same 
 * number of elements and each element in \a lhs compares equal with the element in rhs at the same position.
 * 
 * @tparam N1 is maximum number of characters of \a lhs.
 * @tparam N2 is maximum number of characters of \a rhs.
 * @param[in] lhs string whose contents to compare
 * @param[in] rhs string whose contents to compare
 * @return `true` if the contents of the strings are not equal, `false` otherwise.
 */
template <size_t N1, size_t N2>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
    return !(lhs == rhs);
}

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Compares the contents of two fixed strings.
     * 
     * Compares the contents of \a lhs and \a rhs lexicographically.
     * 
     * @tparam N1 is maximum number of characters of \a lhs.
     * @tparam N2 is maximum number of characters of \a rhs.
     * @param[in] lhs string whose contents to compare
     * @param[in] rhs string whose contents to compare
     * @return The relative order of the first pair of non-equivalent elements in \a lhs and \a rhs if there are such elements.
     */
    template <size_t N1, size_t N2>
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
     * Compares the contents of two fixed strings.
     * 
     * Compares the contents of \a lhs and \a rhs lexicographically.
     * 
     * @tparam N1 is maximum number of characters of \a lhs.
     * @tparam N2 is maximum number of characters of \a rhs.
     * @param[in] lhs string whose contents to compare
     * @param[in] rhs string whose contents to compare
     * @return `true` if the contents of the \a lhs are lexicographically less than the contents of \a rhs, `false` otherwise.
     */
    template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        return lhs.compare(rhs) < 0;
    }

    /**
     * Compares the contents of two fixed strings.
     * 
     * Compares the contents of \a lhs and \a rhs lexicographically.
     * 
     * @tparam N1 is maximum number of characters of \a lhs.
     * @tparam N2 is maximum number of characters of \a rhs.
     * @param[in] lhs string whose contents to compare
     * @param[in] rhs string whose contents to compare
     * @return `true` if the contents of the \a lhs are lexicographically greater than the contents of \a rhs, `false` otherwise.
     */
    template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        return rhs < lhs;
    }

    /**
     * Compares the contents of two fixed strings.
     * 
     * Compares the contents of \a lhs and \a rhs lexicographically.
     * 
     * @tparam N1 is maximum number of characters of \a lhs.
     * @tparam N2 is maximum number of characters of \a rhs.
     * @param[in] lhs string whose contents to compare
     * @param[in] rhs string whose contents to compare
     * @return `true` if the contents of the \a lhs are lexicographically less than or equal to the contents of \a rhs, `false` otherwise.
     */
    template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        return !(rhs < lhs);
    }

    /**
     * Compares the contents of two fixed strings.
     * 
     * Compares the contents of \a lhs and \a rhs lexicographically.
     * 
     * @tparam N1 is maximum number of characters of \a lhs.
     * @tparam N2 is maximum number of characters of \a rhs.
     * @param[in] lhs string whose contents to compare
     * @param[in] rhs string whose contents to compare
     * @return `true` if the contents of the \a lhs are lexicographically greater than or equal to the contents of \a rhs, `false` otherwise.
     */
    template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        return !(lhs < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

/**
 * Make fixed string.
 * 
 * Creates a fixed-length string from a multicharacter array.
 * 
 * @tparam N is maximum number of characters
 * @param[in] data character array of string
 * @return Fixed string.
 */
template <size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string<N - 1> make_fixed_string(const char(&data)[N]) IPADDRESS_NOEXCEPT {
    return fixed_string<N - 1>(data);
}

/**
 * Make fixed string.
 * 
 * Creates a fixed-length string from a wide character array.
 * 
 * @tparam N is maximum number of characters
 * @param[in] data character array of string
 * @return Fixed string.
 */
template <size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string<N - 1> make_fixed_string(const wchar_t(&data)[N]) IPADDRESS_NOEXCEPT {
    return fixed_string<N - 1>(data);
}

/**
 * Make fixed string.
 * 
 * Creates a fixed-length string from UTF-16 character array.
 * 
 * @tparam N is maximum number of characters
 * @param[in] data character array of string
 * @return Fixed string.
 */
template <size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string<N - 1> make_fixed_string(const char16_t(&data)[N]) IPADDRESS_NOEXCEPT {
    return fixed_string<N - 1>(data);
}

/**
 * Make fixed string.
 * 
 * Creates a fixed-length string from UTF-32 character array.
 * 
 * @tparam N is maximum number of characters
 * @param[in] data character array of string
 * @return Fixed string.
 */
template <size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string<N - 1> make_fixed_string(const char32_t(&data)[N]) IPADDRESS_NOEXCEPT {
    return fixed_string<N - 1>(data);
}

#if __cpp_char8_t >= 201811L

/**
 * Make fixed string.
 * 
 * Creates a fixed-length string from UTF-8 character array.
 * 
 * @tparam N is maximum number of characters
 * @param[in] data character array of string
 * @return Fixed string.
 */
template <size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_string<N - 1> make_fixed_string(const char8_t(&data)[N]) IPADDRESS_NOEXCEPT {
    return fixed_string<N - 1>(data);
}

#endif // __cpp_char8_t

#if IPADDRESS_CPP_VERSION >= 17

#if __cpp_char8_t >= 201811L
    template <size_t N> fixed_string(const char8_t(&)[N]) -> fixed_string<N - 1>;
#endif // __cpp_char8_t
    template <size_t N> fixed_string(fixed_string<N>) -> fixed_string<N>;
    template <size_t N> fixed_string(const char(&)[N]) -> fixed_string<N - 1>;
    template <size_t N> fixed_string(const wchar_t(&)[N]) -> fixed_string<N - 1>;
    template <size_t N> fixed_string(const char16_t(&)[N]) -> fixed_string<N - 1>;
    template <size_t N> fixed_string(const char32_t(&)[N]) -> fixed_string<N - 1>;
    fixed_string() -> fixed_string<0>;

#endif

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_FIXED_STRING_HPP
