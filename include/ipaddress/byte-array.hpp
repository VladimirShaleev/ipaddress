/**
 * @file      byte-array.hpp
 * @brief     Provides the byte_array template class for handling fixed-size byte arrays
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * The byte_array template class is designed to encapsulate a fixed-size array of bytes (uint8_t),
 * providing various type definitions for ease of use, and methods for accessing the array elements. 
 * Intended for use in managing IP address representations and manipulations. The class ensures that 
 * the array size is constant and known at compile-time, offering a compile-time guarantee of array 
 * bounds, which can prevent common errors associated with dynamic arrays.
 */

#ifndef IPADDRESS_BYTE_ARRAY_HPP
#define IPADDRESS_BYTE_ARRAY_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * A template class for creating and managing a fixed-size array of bytes.
 * 
 * The byte_array class encapsulates a static-size array of bytes, providing type definitions
 * for element access and iteration in both normal and reverse order. It is designed to be used
 * where a constant size byte buffer is needed, such as in handling network addresses like IP addresses.
 * 
 * @tparam N The number of bytes in the array.
 * @remark The purpose of the byte_array class is to provide functionality similar to std::array in .
 *         environments where std::array cannot be used to its full extent during compile-time operations.
 */
IPADDRESS_EXPORT template <size_t N>
struct byte_array {
    using value_type      = uint8_t; /**< The type of elements contained in the byte_array. */
    using size_type       = size_t; /**< The type representing sizes and counts. */
    using difference_type = ptrdiff_t; /**< The type representing the difference between two pointers. */
    using pointer         = value_type*; /**< A pointer to an element in the byte_array. */
    using const_pointer   = const value_type*; /**< A pointer to a constant element in the byte_array. */
    using reference       = value_type&; /**< A reference to an element in the byte_array. */
    using const_reference = const value_type&; /**< A reference to a constant element in the byte_array. */

    using iterator       = pointer; /**< A random access iterator to byte_array that allows modification of its elements. */
    using const_iterator = const_pointer; /**< A random access iterator to constant byte_array that does not allow modification of its elements. */

    using reverse_iterator       = std::reverse_iterator<iterator>; /**< A reverse random access iterator to byte_array that allows modification of its elements. */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>; /**< A reverse random access iterator to constant byte_array that does not allow modification of its elements. */

    value_type _data[N];

    /**
     * Returns a const_iterator to the beginning of the byte_array.
     * 
     * @return A const_iterator pointing to the first element in the array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data);
    }
    
    /**
     * Returns a const_iterator to the end of the byte_array.
     * 
     * @return A const_iterator pointing to the past-the-end element in the array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data) + N;
    }
    
    /**
     * Returns a const_reverse_iterator to the beginning of the reversed byte_array.
     * 
     * @return A const_reverse_iterator pointing to the first element of the reversed array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    /**
     * Returns a const_reverse_iterator to the end of the reversed byte_array.
     * 
     * @return A const_reverse_iterator pointing to the past-the-end element in the reversed array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }
    
    /**
     * Returns a const_iterator to the beginning of the byte_array (const version).
     * 
     * @return A const_iterator pointing to the first element in the array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    /**
     * Returns a const_iterator to the end of the byte_array (const version).
     * 
     * @return A const_iterator pointing to the past-the-end element in the array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    /**
     * Returns a const_reverse_iterator to the beginning of the reversed byte_array (const version).
     * 
     * @return A const_reverse_iterator pointing to the first element of the reversed array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    /**
     * Returns a const_reverse_iterator to the end of the reversed byte_array (const version).
     * 
     * @return A const_reverse_iterator pointing to the past-the-end element in the reversed array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 IPADDRESS_FORCE_INLINE const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    /**
     * Checks if the byte_array is empty.
     * 
     * @return `true` if the byte_array is empty, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool empty() const IPADDRESS_NOEXCEPT {
        return false;
    }
    
    /**
     * Returns the size of the byte_array.
     * 
     * @return The number of elements in the byte_array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return N;
    }

    /**
     * Returns the maximum size of the byte_array.
     * 
     * @return The number of elements in the byte_array (same as size()).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t max_size() const IPADDRESS_NOEXCEPT {
        return N;
    }

    /**
     * Accesses the element at the specified index with bounds checking.
     * 
     * @param[in] n The index of the element to access.
     * @return A reference to the element at the specified index.
     * @throw std::out_of_range When going beyond the bounds of the array.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator[](size_t n) IPADDRESS_NOEXCEPT {
        return at(n);
    }

    /**
     * Accesses the element at the specified index with bounds checking (const version).
     * 
     * @param[in] n The index of the element to access.
     * @return A const reference to the element at the specified index.
     * @throw std::out_of_range When going beyond the bounds of the array.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference operator[](size_t n) const IPADDRESS_NOEXCEPT {
        return at(n);
    }

    /**
     * Accesses the element at the specified index with bounds checking.
     * 
     * @param[in] n The index of the element to access.
     * @return A reference to the element at the specified index.
     * @throw std::out_of_range When going beyond the bounds of the array.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference at(size_t n) IPADDRESS_NOEXCEPT {
        return _data[n];
    }

    /**
     * Accesses the element at the specified index with bounds checking (const version).
     * 
     * @param[in] n The index of the element to access.
     * @return A const reference to the element at the specified index.
     * @throw std::out_of_range When going beyond the bounds of the array.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference at(size_t n) const IPADDRESS_NOEXCEPT {
        return _data[n];
    }

    /**
     * Accesses the first element in the byte_array.
     * 
     * @return A reference to the first element in the byte_array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference front() IPADDRESS_NOEXCEPT {
        return _data[0];
    }

    /**
     * Accesses the first element in the byte_array (const version).
     * 
     * @return A const reference to the first element in the byte_array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference front() const IPADDRESS_NOEXCEPT {
        return _data[0];
    }

    /**
     * Accesses the last element in the byte_array.
     * 
     * @return A reference to the last element in the byte_array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference back() IPADDRESS_NOEXCEPT {
        return _data[N - 1];
    }

    /**
     * Accesses the last element in the byte_array (const version).
     * 
     * @return A const reference to the last element in the byte_array.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference back() const IPADDRESS_NOEXCEPT {
        return _data[N - 1];
    }

    /**
     * Returns a pointer to the underlying array.
     * 
     * @return A pointer to the underlying array of type value_type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer data() IPADDRESS_NOEXCEPT {
        return _data;
    }

    /**
     * Returns a const pointer to the underlying array (const version).
     * 
     * @return A const pointer to the underlying array of type value_type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_pointer data() const IPADDRESS_NOEXCEPT {
        return _data;
    }

    /**
     * Swaps the contents of this byte_array with another byte_array.
     * 
     * @param[in,out] other The other byte_array to swap contents with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(byte_array& other) IPADDRESS_NOEXCEPT {
        for (size_t i = 0; i < N; ++i) {
            const auto tmp = _data[i];
            _data[i] = other._data[i];
            other._data[i] = tmp;
        }
    }
}; // byte_array<N>

IPADDRESS_EXPORT template <>
class byte_array<0> {
public:
    using value_type      = uint8_t;
    using size_type       = size_t;
    using difference_type = std::ptrdiff_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator       = pointer;
    using const_iterator = const_pointer;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(data());
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(data());
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

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t max_size() const IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator[](size_t n) IPADDRESS_NOEXCEPT {
        return at(n);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference operator[](size_t n) const IPADDRESS_NOEXCEPT {
        return at(n);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference at(size_t /*n*/) IPADDRESS_NOEXCEPT {
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference at(size_t /*n*/) const IPADDRESS_NOEXCEPT {
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference front() IPADDRESS_NOEXCEPT {
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference front() const IPADDRESS_NOEXCEPT {
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference back() IPADDRESS_NOEXCEPT {
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference back() const IPADDRESS_NOEXCEPT {
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer data() IPADDRESS_NOEXCEPT {
        return nullptr;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_pointer data() const IPADDRESS_NOEXCEPT {
        return nullptr;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(byte_array& other) IPADDRESS_NOEXCEPT {
    }
}; // byte_array<0>

/**
 * Checks if two byte_array objects are equal.
 * 
 * Compares two byte_array objects of the same size element-wise to determine if they are equal.
 * 
 * @tparam N The size of the byte array.
 * @param[in] lhs A reference to the left-hand side byte_array object.
 * @param[in] rhs A reference to the right-hand side byte_array object.
 * @return A boolean value indicating whether the two byte_array objects are equal.
 * @retval true the two byte_array objects are equal
 * @retval false the two byte_array objects are not equal
 */
IPADDRESS_EXPORT template <size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Checks if two byte_array objects are not equal.
 * 
 * Compares two byte_array objects of the same size element-wise to determine if they are not equal.
 * 
 * @tparam N The size of the byte array.
 * @param[in] lhs A reference to the left-hand side byte_array object.
 * @param[in] rhs A reference to the right-hand side byte_array object.
 * @return A boolean value indicating whether the two byte_array objects are not equal.
 * @retval true the two byte_array objects are not equal
 * @retval false the two byte_array objects are equal
 */
IPADDRESS_EXPORT template <size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
    return !(lhs == rhs);
}

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Compares two byte_array objects for ordering.
     * 
     * Performs a lexicographical comparison between two byte_array objects using the three-way comparison operator.
     * The comparison is done element-wise and stops at the first unequal pair of elements, returning the result
     * of comparing these two elements. If all elements are equal, the byte_arrays are considered equivalent.
     * 
     * @tparam N The size of the byte array.
     * @param[in] lhs A reference to the left-hand side byte_array object.
     * @param[in] rhs A reference to the right-hand side byte_array object.
     * @return An `std::strong_ordering` value indicating the ordering relationship.
     * @retval std::strong_ordering::less if lhs is lexicographically less than rhs
     * @retval std::strong_ordering::greater if lhs is lexicographically greater than rhs
     * @retval std::strong_ordering::equivalent if lhs is lexicographically equal to rhs
     */
    IPADDRESS_EXPORT template <size_t N>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
        for (size_t i = 0; i < N; ++i) {
            if (const auto result = lhs[i] <=> rhs[i]; result != std::strong_ordering::equivalent) {
                return result;
            }
        }
        return std::strong_ordering::equivalent;
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Determines if one byte_array is less than another.
     * 
     * Performs a lexicographical comparison of two byte_array objects. The comparison is done
     * element-wise and stops at the first unequal pair where the left-hand side is less than
     * the right-hand side.
     * 
     * @tparam N The size of the byte arrays.
     * @param[in] lhs A reference to the left-hand side byte_array object.
     * @param[in] rhs A reference to the right-hand side byte_array object.
     * @return `true` if lhs is lexicographically less than rhs, `false` otherwise.
     */
    IPADDRESS_EXPORT template <size_t N>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
        for (size_t i = 0; i < N; ++i) {
            if (lhs._data[i] < rhs._data[i]) {
                return true;
            } else if (lhs._data[i] != rhs._data[i]) {
                break;
            }
        }
        return false;
    }

    /**
     * Determines if one byte_array is greater than another.
     * 
     * @tparam N The size of the byte arrays.
     * @param[in] lhs A reference to the left-hand side byte_array object.
     * @param[in] rhs A reference to the right-hand side byte_array object.
     * @return `true` if lhs is lexicographically greater than rhs, `false` otherwise.
     */
    IPADDRESS_EXPORT template <size_t N>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
        return rhs < lhs;
    }

    /**
     * Determines if one byte_array is less than or equal to another.
     * 
     * @tparam N The size of the byte arrays.
     * @param[in] lhs A reference to the left-hand side byte_array object.
     * @param[in] rhs A reference to the right-hand side byte_array object.
     * @return `true` if lhs is lexicographically less than or equal to rhs, `false` otherwise.
     */
    IPADDRESS_EXPORT template <size_t N>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
        return !(rhs < lhs);
    }

    /**
     * Determines if one byte_array is greater than or equal to another.
     * 
     * @tparam N The size of the byte arrays.
     * @param[in] lhs A reference to the left-hand side byte_array object.
     * @param[in] rhs A reference to the right-hand side byte_array object.
     * @return `true` if lhs is lexicographically greater than or equal to rhs, `false` otherwise.
     */
    IPADDRESS_EXPORT template <size_t N>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
        return !(lhs < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_BYTE_ARRAY_HPP
