/**
 * @file      ip-any-iterator.hpp
 * @brief     Provides iterator classes for sequential unified traversal of IPv4 and IPv6
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This file defines the `ip_any_iterator` class template, which facilitates the
 * unified traversal over both IPv4 and IPv6 address spaces. It abstracts the
 * differences between the two IP versions, allowing users to write code that can
 * operate on either IP version without modification. This is particularly useful
 * for applications that need to be agnostic to the type of IP network they are
 * working with, such as network utilities, security scanners, or dual-stack network
 * implementations. The iterator provides random access capabilities, making it
 * suitable for a wide range of algorithms that require iteration over IP addresses.
 */

#ifndef IPADDRESS_IP_ANY_ITERATOR_HPP
#define IPADDRESS_IP_ANY_ITERATOR_HPP

#include "ip-any-address.hpp"
#include "ip-network-iterator.hpp"
#include "ipv4-network.hpp"
#include "ipv6-network.hpp"

namespace IPADDRESS_NAMESPACE {
/**
 * An iterator for unified traversal over IPv4 and IPv6 address spaces.
 * 
 * The `ip_any_iterator` class template provides a mechanism to iterate over
 * both IPv4 and IPv6 addresses using a single, unified interface. It abstracts
 * the complexities associated with handling the two different IP versions,
 * allowing for code that is agnostic to the IP version it operates on. This
 * is particularly useful for applications that must work with both IPv4 and
 * IPv6 networks, such as dual-stack network configurations, without the need
 * for separate code paths. The iterator supports random access, which enables
 * efficient traversal and manipulation of IP address ranges in various network
 * operations.
 * 
 * @tparam T The type of IP address or network (IPv4 or IPv6) the iterator will handle.
 * @tparam Ipv4Iter The type of IPv4 iterator
 * @tparam Ipv6Iter The type of IPv6 iterator
 */
template <typename T, typename Ipv4Iter, typename Ipv6Iter>
class ip_any_iterator {
public:
#ifdef IPADDRESS_HAS_CONCEPTS
    using iterator_concept  = std::random_access_iterator_tag; /**< Iterator concept (C++20). */
#endif
    using iterator_category = std::random_access_iterator_tag; /**< Iterator category. */
    using value_type        = T; /**< Value type iterated over. */
    using difference_type   = std::int64_t; /**< Difference type between iterators. */
    using pointer           = const value_type*; /**< Pointer to value type. */
    using reference         = const value_type&; /**< Reference to value type. */

    using uint_type         = uint128_t; /**< Unsigned integer type for addressing. */

    /**
     * Default constructor.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator() IPADDRESS_NOEXCEPT = default;
    
    /**
     * Copy constructor.
     * 
     * @param[in] other The ip_any_iterator to copy.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator(const ip_any_iterator& other) IPADDRESS_NOEXCEPT = default;
    
    /**
     * Move constructor.
     * 
     * @param[in] other The ip_any_iterator to move.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator(ip_any_iterator&& other) IPADDRESS_NOEXCEPT = default;

    /**
     * Copy assignment operator.
     * 
     * @param[in] other The ip_any_iterator to copy.
     * @return A reference to the assigned ip_any_iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator& operator=(const ip_any_iterator& other) IPADDRESS_NOEXCEPT = default;
    
    /**
     * Move assignment operator.
     * 
     * @param[in] other The ip_any_iterator to move.
     * @return A reference to the moved ip_any_iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator& operator=(ip_any_iterator&& other) IPADDRESS_NOEXCEPT = default;

    /**
     * Constructs an ip_any_iterator from an Ipv4Iter iterator.
     * 
     * @param[in] iter The underlying iterator to reverse.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator(Ipv4Iter iter) IPADDRESS_NOEXCEPT : _iter(iter), _version(ip_version::V4), _current(*iter) {
    }

    /**
     * Constructs an ip_any_iterator from an Ipv6Iter iterator.
     * 
     * @param[in] iter The underlying iterator to reverse.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator(Ipv6Iter iter) IPADDRESS_NOEXCEPT : _iter(iter), _version(ip_version::V6), _current(*iter) {
    }

    /**
     * Calculates the difference in the number of elements between this and another ip_any_iterator.
     * 
     * @param[in] other The ip_any_iterator to compare with.
     * @return The number of elements between this and the other iterator.
     * @remark This is a special function for calculate the difference between iterators, 
     *         which can correctly represent all addresses using the integer number uint128_t
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t uint_diff(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        if (_version != other._version) {
            return 0;
        }
        return _version == ip_version::V4 ? uint128_t(_iter.iter4.uint_diff(other._iter.iter4)) : _iter.iter6.uint_diff(other._iter.iter6);
    }

    /**
     * Returns a reference to the current element.
     * 
     * @return A reference to the element pointed to by the iterator.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator*() const IPADDRESS_NOEXCEPT {
        return _current;
    }

    /**
     * Returns a pointer to the current element.
     * 
     * @return A pointer to the element pointed to by the iterator.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer operator->() const IPADDRESS_NOEXCEPT {
        return &_current;
    }

    /**
     * Accesses an element by index.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type operator[](difference_type n) const IPADDRESS_NOEXCEPT {
        const auto& it = *this;
        return it[uint128_t(n)];
    }

    /**
     * Accesses an element by index.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type operator[](const uint128_t& n) const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? value_type(_iter.iter4[uint32_t(n)]) : value_type(_iter.iter6[n]);
    }

    /**
     * Pre-increment operator.
     * 
     * @return A reference to the incremented iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator& operator++() IPADDRESS_NOEXCEPT {
        add(1);
        return *this;
    }

    /**
     * Post-increment operator.
     * 
     * @return The iterator before incrementing.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator++(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    /**
     * Pre-decrement operator.
     * 
     * @return A reference to the decremented iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator& operator--() IPADDRESS_NOEXCEPT {
        sub(1);
        return *this;
    }

    /**
     * Post-decrement operator.
     * 
     * @return The iterator before decrementing.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator--(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    /**
     * Addition assignment operator.
     * 
     * Moves the iterator forward by \a n positions.
     * 
     * @param[in] n The number of positions to move the iterator forward.
     * @return A reference to the updated iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator& operator+=(difference_type n) IPADDRESS_NOEXCEPT {
        add(n);
        return *this;
    }

    /**
     * Addition assignment operator.
     * 
     * Moves the iterator forward by \a n positions.
     * 
     * @param[in] n The number of positions to move the iterator forward.
     * @return A reference to the updated iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator& operator+=(const uint128_t& n) IPADDRESS_NOEXCEPT {
        add(n);
        return *this;
    }

    /**
     * Subtraction assignment operator.
     * 
     * Moves the iterator backward by \a n positions.
     * 
     * @param[in] n The number of positions to move the iterator backward.
     * @return A reference to the updated iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator& operator-=(difference_type n) IPADDRESS_NOEXCEPT {
        sub(n);
        return *this;
    }

    /**
     * Subtraction assignment operator.
     * 
     * Moves the iterator backward by \a n positions.
     * 
     * @param[in] n The number of positions to move the iterator backward.
     * @return A reference to the updated iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator& operator-=(const uint128_t& n) IPADDRESS_NOEXCEPT {
        sub(n);
        return *this;
    }

    /**
     * Addition operator.
     * 
     * Creates a new iterator that is \a n positions ahead of the current one.
     * 
     * @param[in] n The number of positions to move ahead.
     * @return A new iterator that is \a n positions ahead.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator+(difference_type n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    /**
     * Addition operator.
     * 
     * Creates a new iterator that is \a n positions ahead of the current one.
     * 
     * @param[in] n The number of positions to move ahead.
     * @return A new iterator that is \a n positions ahead.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator+(const uint128_t& n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    /**
     * Addition operator.
     * 
     * Creates a new iterator that is \a n positions ahead of the specified iterator.
     * 
     * @param[in] n The number of positions to move ahead.
     * @param[in] it The iterator to move ahead from.
     * @return A new iterator that is \a n positions ahead of it.
     */
    IPADDRESS_NODISCARD friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator+(difference_type n, const ip_any_iterator& it) IPADDRESS_NOEXCEPT {
        return it + n;
    }

    /**
     * Addition operator.
     * 
     * Creates a new iterator that is \a n positions ahead of the specified iterator.
     * 
     * @param[in] n The number of positions to move ahead.
     * @param[in] it The iterator to move ahead from.
     * @return A new iterator that is \a n positions ahead of it.
     */
    IPADDRESS_NODISCARD friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator+(const uint128_t& n, const ip_any_iterator& it) IPADDRESS_NOEXCEPT {
        return it + n;
    }

    /**
     * Subtraction operator.
     * 
     * Creates a new iterator that is \a n positions behind the current one.
     * 
     * @param[in] n The number of positions to move behind.
     * @return A new iterator that is \a n positions behind.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator-(difference_type n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    /**
     * Subtraction operator.
     * 
     * Creates a new iterator that is \a n positions behind the current one.
     * 
     * @param[in] n The number of positions to move behind.
     * @return A new iterator that is \a n positions behind.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator-(const uint128_t& n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    /**
     * Subtraction operator.
     * 
     * Calculates the difference in the number of elements between this and another ip_any_iterator.
     * 
     * @param[in] other The ip_any_iterator to compare with.
     * @return The number of elements between this and the other iterator.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type operator-(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        if (_version != other._version) {
            return 0;
        }
        return _version == ip_version::V4 ? _iter.iter4 - other._iter.iter4 : _iter.iter6 - other._iter.iter6;
    }

    /**
     * Equality operator.
     * 
     * Compares two ip_any_iterator for equality.
     * 
     * @param[in] other The ip_any_iterator to compare with.
     * @return `true` if the iterators are equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        if (_version != other._version) {
            return false;
        }
        return _version == ip_version::V4 ? _iter.iter4 == other._iter.iter4 : _iter.iter6 == other._iter.iter6;
    }

    /**
     * Inequality operator.
     * 
     * Compares two ip_any_iterator for inequality.
     * 
     * @param[in] other The ip_any_iterator to compare with.
     * @return `true` if the iterators are not equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Three-way comparison operator (spaceship operator).
     * 
     * Compares two ip_any_iterator for ordering.
     * 
     * @param[in] other The ip_any_iterator to compare with.
     * @return The result of the comparison as a std::strong_ordering value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        if (const auto result = _version <=> other._version; result == std::strong_ordering::equivalent) {
            return _version == ip_version::V4 ? _iter.iter4 <=> other._iter.iter4 : _iter.iter6 <=> other._iter.iter6;
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Less-than operator.
     * 
     * Compares two ip_any_iterator to determine if the left one is less than the right one.
     * 
     * @param[in] other The ip_any_iterator to compare with.
     * @return `true` if the left iterator is less than the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        if (_version != other._version) {
            return false;
        }
        return _version == ip_version::V4 ? _iter.iter4 < other._iter.iter4 : _iter.iter6 < other._iter.iter6;
    }

    /**
     * Less-than-or-equal-to operator.
     * 
     * Compares two ip_any_iterator to determine if the left one is less than or equal to the right one.
     * 
     * @param[in] other The ip_any_iterator to compare with.
     * @return `true` if the left iterator is less than or equal to the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }

    /**
     * Greater-than operator.
     * 
     * Compares two ip_any_iterator to determine if the left one is greater than the right one.
     * 
     * @param[in] other The ip_any_iterator to compare with.
     * @return `true` if the left iterator is greater than the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }

    /**
     * Greater-than-or-equal-to operator.
     * 
     * Compares two ip_any_iterator to determine if the left one is greater than or equal to the right one.
     * 
     * @param[in] other The ip_any_iterator to compare with.
     * @return `true` if the left iterator is greater than or equal to the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this < other);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator base() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? ip_any_iterator(_iter.iter4.base()) : ip_any_iterator(_iter.iter6.base());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator reverse() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? ip_any_iterator(_iter.iter4.reverse()) : ip_any_iterator(_iter.iter6.reverse());
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void add(const uint128_t& n) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V4) {
            _iter.iter4 += uint32_t(n);
            _current = *_iter.iter4;
        } else {
            _iter.iter6 += n;
            _current = *_iter.iter6;
        }
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void sub(const uint128_t& n) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V4) {
            _iter.iter4 -= uint32_t(n);
            _current = *_iter.iter4;
        } else {
            _iter.iter6 -= n;
            _current = *_iter.iter6;
        }
    }

    template <typename>
    friend class ip_reverse_iterator;

    union ip_any_iter {
        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iter() IPADDRESS_NOEXCEPT : iter4() {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iter(const Ipv4Iter& iter) IPADDRESS_NOEXCEPT : iter4(iter) {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iter(const Ipv6Iter& iter) IPADDRESS_NOEXCEPT : iter6(iter) {
        }

        Ipv4Iter iter4;
        Ipv6Iter iter6;
    } _iter {};
    ip_version _version = ip_version::V4;
    value_type _current {};
}; // ip_any_iterator

/**
 * A sequence of host IP addresses.
 * 
 * The subnets_any_sequence class template to provide a container-like interface for IP address sequences.
 * It allows for iteration over the host addresses in a network or subnet, excluding the network and broadcast addresses when appropriate.
 * This specialization is essential for network-related operations that require processing of individual host addresses within a given range.
 * 
 * @tparam Base The base type from which the ip_address_base is derived, representing the underlying IP address type.
 * @remark When iterating, obtaining addresses occurs through lazy calculations.
 */
class hosts_any_sequence {
public:
    using value_type      = ip_address; /**< The type of the IP addresses in the sequence. */
    using size_type       = std::size_t; /**< The type used for representing the size of the sequence. */
    using difference_type = uint128_t; /**< The type used for representing differences between iterators. */
    using pointer         = value_type*; /**< The pointer type for the value_type. */
    using const_pointer   = const value_type*; /**< The const pointer type for the value_type. */
    using reference       = value_type&; /**< The reference type for the value_type. */
    using const_reference = const value_type&; /**< The const reference type for the value_type. */

    using iterator       = ip_any_iterator<value_type, ip_address_iterator<ipv4_address>, ip_address_iterator<ipv6_address>>; /**< The iterator type for iterating over the sequence.  */
    using const_iterator = iterator; /**< The const iterator type for iterating over the sequence.  */

    using reverse_iterator       = ip_reverse_iterator<iterator>; /**< The reverse iterator type for iterating over the sequence in reverse. */
    using const_reverse_iterator = ip_reverse_iterator<const_iterator>; /**< The const reverse iterator type for iterating over the sequence in reverse. */

    /**
     * Copy constructor.
     * 
     * @param[in] other The hosts_any_sequence to copy.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_any_sequence(const hosts_any_sequence& other) IPADDRESS_NOEXCEPT = default;
    
    /**
     * Move constructor.
     * 
     * @param[in] other The hosts_any_sequence to move.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_any_sequence(hosts_any_sequence&& other) IPADDRESS_NOEXCEPT = default;

    /**
     * Copy assignment operator.
     * 
     * @param[in] other The hosts_any_sequence to copy.
     * @return A reference to the assigned hosts_any_sequence.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_any_sequence& operator=(const hosts_any_sequence& other) IPADDRESS_NOEXCEPT = default;
    
    /**
     * Move assignment operator.
     * 
     * @param[in] other The hosts_any_sequence to move.
     * @return A reference to the moved hosts_any_sequence.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_any_sequence& operator=(hosts_any_sequence&& other) IPADDRESS_NOEXCEPT = default;

    /**
     * Constructs a hosts_any_sequence with specified network parameters.
     * 
     * @param[in] begin The begin ipv4 address iterator.
     * @param[in] end The end ipv4 address iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_any_sequence(ip_address_iterator<ipv4_address> begin, ip_address_iterator<ipv4_address> end) IPADDRESS_NOEXCEPT : _begin(begin), _end(end) {
    }

    /**
     * Constructs a hosts_any_sequence with specified network parameters.
     * 
     * @param[in] begin The begin ipv6 address iterator.
     * @param[in] end The end ipv6 address iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_any_sequence(ip_address_iterator<ipv6_address> begin, ip_address_iterator<ipv6_address> end) IPADDRESS_NOEXCEPT : _begin(begin), _end(end) {
    }

    /**
     * Gets the beginning iterator of the sequence.
     * 
     * @return A const_iterator to the first element in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return _begin;
    }

    /**
     * Gets the end iterator of the sequence.
     * 
     * @return A const_iterator to the element following the last element in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return _end;
    }

    /**
     * Gets the beginning reverse iterator of the sequence.
     * 
     * @return A const_reverse_iterator to the first element of the reversed sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    /**
     * Gets the end reverse iterator of the sequence.
     * 
     * @return A const_reverse_iterator to the element following the last element of the reversed sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }
    
    /**
     * Gets the beginning const iterator of the sequence.
     * 
     * @return A const_iterator to the first element in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    /**
     * Gets the end const iterator of the sequence.
     * 
     * @return A const_iterator to the element following the last element in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    /**
     * Gets the beginning const reverse iterator of the sequence.
     * 
     * @return A const_reverse_iterator to the first element of the reversed sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    /**
     * Gets the end const reverse iterator of the sequence.
     * 
     * @return A const_reverse_iterator to the element following the last element of the reversed sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    /**
     * Checks if the sequence is empty.
     * 
     * @return `true` if the sequence is empty, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool empty() const IPADDRESS_NOEXCEPT {
        return _begin == _end;
    }
    
    /**
     * Gets the size of the sequence.
     * 
     * @return The number of elements in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type size() const IPADDRESS_NOEXCEPT {
        return _end.uint_diff(_begin);
    }

    /**
     * Accesses an element by index.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type operator[](difference_type n) const IPADDRESS_NOEXCEPT {
        return at(n);
    }

    /**
     * Accesses an element by index with bounds checking.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type at(difference_type n) const IPADDRESS_NOEXCEPT {
        return *(_begin + n);
    }

    /**
     * Accesses the first element in the sequence.
     * 
     * @return A reference to the first element in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type front() const IPADDRESS_NOEXCEPT {
        return *_begin;
    }

    /**
     * Accesses the last element in the sequence.
     * 
     * @return A reference to the last element in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type back() const IPADDRESS_NOEXCEPT {
        return *(_end - 1U);
    }

private:
    const_iterator _begin{};
    const_iterator _end{};
}; // hosts_any_sequence

/**
 * A sequence container for subnet ranges within a network.
 * 
 * This class template represents a sequence of subnets within a network.
 * It provides forward and reverse iterators to traverse the subnets and
 * offers insight into the structure of a network by breaking it down into
 * smaller, manageable parts.
 * 
 * @tparam T The type of IP network to be divided into subnets.
 * @remark When iterating, obtaining networks occurs through lazy calculations.
 */
template <typename T>
class subnets_any_sequence {
public:
    using value_type      = T; /**< The type of subnet value. */
    using size_type       = std::size_t; /**< An unsigned integral type. */
    using difference_type = uint128_t; /**< Unsigned integer type for differences. */
    using pointer         = value_type*; /**< Pointer to the subnet type. */
    using const_pointer   = const value_type*; /**< Const pointer to the subnet type. */
    using reference       = value_type&; /**< Reference to the subnet type. */
    using const_reference = const value_type&; /**< Const reference to the subnet type. */

    using iterator       = ip_any_iterator<value_type, ip_network_iterator<ipv4_network>, ip_network_iterator<ipv6_network>>; /**< Forward iterator for subnet traversal. */
    using const_iterator = iterator; /**< Const forward iterator for subnet traversal. */

    using reverse_iterator       = ip_reverse_iterator<iterator>; /**< Reverse iterator for subnet traversal. */
    using const_reverse_iterator = ip_reverse_iterator<const_iterator>; /**< Const reverse iterator for subnet */

    /**
     * Copy constructor.
     * 
     * @param[in] other The subnets_any_sequence to copy.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_any_sequence(const subnets_any_sequence& other) IPADDRESS_NOEXCEPT = default;
    
    /**
     * Move constructor.
     * 
     * @param[in] other The subnets_any_sequence to move.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_any_sequence(subnets_any_sequence&& other) IPADDRESS_NOEXCEPT = default;

    /**
     * Copy assignment operator.
     * 
     * @param[in] other The subnets_any_sequence to copy.
     * @return A reference to the assigned subnets_any_sequence.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_any_sequence& operator=(const subnets_any_sequence& other) IPADDRESS_NOEXCEPT = default;
    
    /**
     * Move assignment operator.
     * 
     * @param[in] other The subnets_any_sequence to move.
     * @return A reference to the moved subnets_any_sequence.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_any_sequence& operator=(subnets_any_sequence&& other) IPADDRESS_NOEXCEPT = default;

    /**
     * Constructs a subnets_any_sequence with specified network parameters.
     * 
     * @param[in] begin The begin ipv4 network iterator.
     * @param[in] end The end ipv4 network iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_any_sequence(ip_network_iterator<ipv4_network> begin, ip_network_iterator<ipv4_network> end) IPADDRESS_NOEXCEPT : _begin(begin), _end(end) {
    }

    /**
     * Constructs a subnets_any_sequence with specified network parameters.
     * 
     * @param[in] begin The begin ipv6 network iterator.
     * @param[in] end The end ipv6 network iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_any_sequence(ip_network_iterator<ipv6_network> begin, ip_network_iterator<ipv6_network> end) IPADDRESS_NOEXCEPT : _begin(begin), _end(end) {
    }

    /**
     * Gets the beginning iterator of the sequence.
     * 
     * @return A const_iterator to the first element in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return _begin;
    }

    /**
     * Gets the end iterator of the sequence.
     * 
     * @return A const_iterator to the element following the last element in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return _end;
    }

    /**
     * Gets the beginning reverse iterator of the sequence.
     * 
     * @return A const_reverse_iterator to the first element of the reversed sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    /**
     * Gets the end reverse iterator of the sequence.
     * 
     * @return A const_reverse_iterator to the element following the last element of the reversed sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }
    
    /**
     * Gets the beginning const iterator of the sequence.
     * 
     * @return A const_iterator to the first element in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    /**
     * Gets the end const iterator of the sequence.
     * 
     * @return A const_iterator to the element following the last element in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    /**
     * Gets the beginning const reverse iterator of the sequence.
     * 
     * @return A const_reverse_iterator to the first element of the reversed sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    /**
     * Gets the end const reverse iterator of the sequence.
     * 
     * @return A const_reverse_iterator to the element following the last element of the reversed sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    /**
     * Checks if the sequence is empty.
     * 
     * @return `true` if the sequence is empty, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool empty() const IPADDRESS_NOEXCEPT {
        return _begin == _end;
    }
    
    /**
     * Gets the size of the sequence.
     * 
     * @return The number of elements in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type size() const IPADDRESS_NOEXCEPT {
        return _end.uint_diff(_begin);
    }

    /**
     * Accesses an element by index.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type operator[](difference_type n) const IPADDRESS_NOEXCEPT {
        return at(n);
    }

    /**
     * Accesses an element by index with bounds checking.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type at(difference_type n) const IPADDRESS_NOEXCEPT {
        return *(_begin + n);
    }

    /**
     * Accesses the first element in the sequence.
     * 
     * @return A reference to the first element in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type front() const IPADDRESS_NOEXCEPT {
        return *_begin;
    }

    /**
     * Accesses the last element in the sequence.
     * 
     * @return A reference to the last element in the sequence.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type back() const IPADDRESS_NOEXCEPT {
        return *(_end - 1U);
    }

private:
    const_iterator _begin{};
    const_iterator _end{};
}; // subnets_any_sequence

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_ANY_ITERATOR_HPP
