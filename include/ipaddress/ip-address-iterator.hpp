/**
 * @file      ip-address-iterator.hpp
 * @brief     Provides iterator classes for sequential IP address traversal
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This file defines iterator classes that enable sequential traversal over IP addresses within a network range.
 * It includes the ip_reverse_iterator for reverse iteration, ip_address_iterator for forward iteration,
 * and hosts_sequence for iterating over all host addresses in a network. These classes are essential for
 * applications that perform operations on multiple IP addresses, such as network scanners, IP address management
 * systems, and other network utilities. The iterators are designed to be flexible and work with both IPv4 and IPv6
 */

#ifndef IPADDRESS_IP_ADDRESS_ITERATOR_HPP
#define IPADDRESS_IP_ADDRESS_ITERATOR_HPP

#include "config.hpp"
#include "ipv4-address.hpp"
#include "ipv6-address.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * A reverse iterator template class for IP addresses.
 * 
 * The ip_reverse_iterator class template provides reverse iteration capabilities over a range of IP addresses.
 * It is designed to work with both IPv4 and IPv6 addresses, allowing for backward traversal of IP address sequences.
 * This class template is particularly useful in scenarios where IP addresses need to be processed in reverse order,
 * such as deallocating IP addresses in a network. The ip_reverse_iterator leverages underlying iterator functionality
 * to provide a robust and flexible mechanism for reverse IP address iteration.
 * 
 * @tparam Iterator The underlying iterator type over which this reverse iterator operates.
 */
template <typename Iterator>
class ip_reverse_iterator {
public:
#ifdef IPADDRESS_HAS_CONCEPTS
    using iterator_concept  = std::random_access_iterator_tag; /**< The iterator concept, defined if concepts are available. */
#endif
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category; /**< The category of the iterator. */
    using iterator_type	    = Iterator; /**< The underlying iterator type. */
    using value_type        = typename std::iterator_traits<Iterator>::value_type; /**< The type of the values iterated over. */
    using difference_type   = typename std::iterator_traits<Iterator>::difference_type; /**< The type representing the difference between two iterators. */
    using pointer           = typename std::iterator_traits<Iterator>::pointer; /**< The pointer type of the iterated values. */
    using reference         = typename std::iterator_traits<Iterator>::reference; /**< The reference type of the iterated values. */

    using uint_type         = typename Iterator::uint_type; /**< The unsigned integer type used by the iterator. */

    /**
     * Default constructor.
     * 
     * Constructs an ip_reverse_iterator that points to no object.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator() IPADDRESS_NOEXCEPT = default;
    
    /**
     * Constructs an ip_reverse_iterator from an underlying iterator.
     * 
     * Creates an ip_reverse_iterator that is the reverse of the given iterator.
     * 
     * @param[in] it The underlying iterator to reverse.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_reverse_iterator(Iterator it) IPADDRESS_NOEXCEPT
        : _it(it.reverse()) {
    }

    /**
     * Calculates the difference in the number of elements between this and another ip_reverse_iterator.
     * 
     * @param[in] other The ip_reverse_iterator to compare with.
     * @return The number of elements between this and the other iterator.
     * @remark This is a special function for calculate the difference between iterators, 
     *         which can correctly represent all addresses using the integer number uint128_t
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint_type uint_diff(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return other._it.uint_diff(_it);
    }

    /**
     * Returns the underlying base iterator.
     * 
     * @return The base iterator.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator_type base() const IPADDRESS_NOEXCEPT {
        return _it.base();
    }

    /**
     * Returns a reference to the current element.
     * 
     * @return A reference to the element pointed to by the iterator.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator*() const IPADDRESS_NOEXCEPT {
        return *_it;
    }

    /**
     * Returns a pointer to the current element.
     * 
     * @return A pointer to the element pointed to by the iterator.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer operator->() const IPADDRESS_NOEXCEPT {
        return &*_it;
    }

    /**
     * Accesses an element by index.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type operator[](difference_type n) const IPADDRESS_NOEXCEPT {
        return *(_it - n);
    }

    /**
     * Accesses an element by index.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type operator[](const uint_type& n) const IPADDRESS_NOEXCEPT {
        return *(_it - n);
    }

    /**
     * Pre-increment operator.
     * 
     * @return A reference to the incremented iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator++() IPADDRESS_NOEXCEPT {
        --_it;
        return *this;
    }

    /**
     * Post-increment operator.
     * 
     * @return The iterator before incrementing.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator++(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    /**
     * Pre-decrement operator.
     * 
     * @return A reference to the decremented iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator--() IPADDRESS_NOEXCEPT {
        ++_it;
        return *this;
    }

    /**
     * Post-decrement operator.
     * 
     * @return The iterator before decrementing.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator--(int) IPADDRESS_NOEXCEPT {
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
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator+=(difference_type n) IPADDRESS_NOEXCEPT {
        _it -= n;
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
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator+=(const uint_type& n) IPADDRESS_NOEXCEPT {
        _it -= n;
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
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator-=(difference_type n) IPADDRESS_NOEXCEPT {
        _it += n;
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
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator-=(const uint_type& n) IPADDRESS_NOEXCEPT {
        _it += n;
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
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator+(difference_type n) const IPADDRESS_NOEXCEPT {
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
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator+(const uint_type& n) const IPADDRESS_NOEXCEPT {
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
    IPADDRESS_NODISCARD friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator+(difference_type n, const ip_reverse_iterator& it) IPADDRESS_NOEXCEPT {
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
    IPADDRESS_NODISCARD friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator+(const uint_type& n, const ip_reverse_iterator& it) IPADDRESS_NOEXCEPT {
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
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator-(difference_type n) const IPADDRESS_NOEXCEPT {
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
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator-(const uint_type& n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    /**
     * Subtraction operator.
     * 
     * Calculates the difference in the number of elements between this and another ip_reverse_iterator.
     * 
     * @param[in] other The ip_reverse_iterator to compare with.
     * @return The number of elements between this and the other iterator.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type operator-(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return difference_type(other._it - _it);
    }

    /**
     * Equality operator.
     * 
     * Compares two ip_reverse_iterators for equality.
     * 
     * @param[in] other The ip_reverse_iterator to compare with.
     * @return `true` if the iterators are equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return  other._it == _it;
    }

    /**
     * Inequality operator.
     * 
     * Compares two ip_reverse_iterators for inequality.
     * 
     * @param[in] other The ip_reverse_iterator to compare with.
     * @return `true` if the iterators are not equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Three-way comparison operator (spaceship operator).
     * 
     * Compares two ip_reverse_iterators for ordering.
     * 
     * @param[in] other The ip_reverse_iterator to compare with.
     * @return The result of the comparison as a std::strong_ordering value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return other._it <=> _it;
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Less-than operator.
     * 
     * Compares two ip_reverse_iterators to determine if the left one is less than the right one.
     * 
     * @param[in] other The ip_reverse_iterator to compare with.
     * @return `true` if the left iterator is less than the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return other._it < _it;
    }

    /**
     * Less-than-or-equal-to operator.
     * 
     * Compares two ip_reverse_iterators to determine if the left one is less than or equal to the right one.
     * 
     * @param[in] other The ip_reverse_iterator to compare with.
     * @return `true` if the left iterator is less than or equal to the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }

    /**
     * Greater-than operator.
     * 
     * Compares two ip_reverse_iterators to determine if the left one is greater than the right one.
     * 
     * @param[in] other The ip_reverse_iterator to compare with.
     * @return `true` if the left iterator is greater than the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }

    /**
     * Greater-than-or-equal-to operator.
     * 
     * Compares two ip_reverse_iterators to determine if the left one is greater than or equal to the right one.
     * 
     * @param[in] other The ip_reverse_iterator to compare with.
     * @return `true` if the left iterator is greater than or equal to the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this < other);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    Iterator _it;
};

template <typename>
class ip_address_iterator;

/**
 * An iterator for traversing IP addresses.
 * 
 * The `ip_address_iterator` class template is specialized for `ip_address_base<Base>` to provide iteration functionality over IP addresses.
 * It supports all operations typical for a random access iterator, allowing both forward and backward traversal with constant time complexity
 * for increment, decrement, and arithmetic operations. This specialization is crucial for enabling seamless iteration over IP addresses,
 * abstracting away the underlying IP version specifics.
 * 
 * @tparam Base The base type from which the ip_address_base is derived, representing the underlying IP address type.
 */
template <typename Base>
class ip_address_iterator<ip_address_base<Base>> {
public:
#ifdef IPADDRESS_HAS_CONCEPTS
    using iterator_concept  = std::random_access_iterator_tag; /**< The iterator concept, defined if concepts are available. */
#endif
    using iterator_category = std::random_access_iterator_tag; /**< The category of the iterator. */
    using value_type        = ip_address_base<Base>; /**< The type of the values iterated over. */
    using difference_type   = std::int64_t; /**< The type representing the difference between two iterators. */
    using pointer           = const value_type*; /**< The pointer type of the iterated values. */
    using reference         = const value_type&; /**< The reference type of the iterated values. */

    using uint_type         = typename value_type::uint_type; /**< The unsigned integer type used by the */

    /**
     * Default constructor.
     * 
     * Constructs an ip_address_iterator that points to no object.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator() IPADDRESS_NOEXCEPT = default;

    /**
     * Constructs an ip_address_iterator with a specific range and reference point.
     * 
     * Initializes the iterator to point to a specific IP address within a range, with an optional carry for overflow handling.
     * 
     * @param[in] ref The reference IP address to which the iterator initially points.
     * @param[in] carry An optional carry value for overflow handling (default is 0).
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_address_iterator(reference ref, int carry = 0) IPADDRESS_NOEXCEPT
        : _current(ref), _offset(ref.to_uint()), _carry(carry) {
    }

    /**
     * Calculates the difference in the number of elements between this and another ip_address_iterator.
     * 
     * @param[in] other The ip_address_iterator to compare with.
     * @return The number of elements between this and the other iterator.
     * @remark This is a special function for calculate the difference between iterators, 
     *         which can correctly represent all addresses using the integer number uint128_t
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint_type uint_diff(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return _offset - other._offset;
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
        return it[uint_type(n)];
    }

    /**
     * Accesses an element by index.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type operator[](const uint_type& n) const IPADDRESS_NOEXCEPT {
        return value_type::from_uint(_offset + n);
    }

    /**
     * Pre-increment operator.
     * 
     * Increments the iterator to the next element.
     * 
     * @return A reference to the incremented iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator& operator++() IPADDRESS_NOEXCEPT {
        add(1);
        return *this;
    }

    /**
     * Post-increment operator.
     * 
     * Increments the iterator to the next element and returns the iterator before the increment.
     * 
     * @return The iterator before the increment.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator++(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    /**
     * Pre-decrement operator.
     * 
     * Decrements the iterator to the previous element.
     * 
     * @return A reference to the decremented iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator& operator--() IPADDRESS_NOEXCEPT {
        sub(1);
        return *this;
    }

    /**
     * Post-decrement operator.
     * 
     * Decrements the iterator to the previous element and returns the iterator before the decrement.
     * 
     * @return The iterator before the decrement.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator--(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    /**
     * Addition assignment operator.
     * 
     * Adds a difference_type value to the iterator.
     * 
     * @param[in] n The number to add.
     * @return A reference to the updated iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator& operator+=(difference_type n) IPADDRESS_NOEXCEPT {
        add(n);
        return *this;
    }

    /**
     * Addition assignment operator.
     * 
     * Adds a uint_type value to the iterator.
     * 
     * @param[in] n The number to add.
     * @return A reference to the updated iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator& operator+=(const uint_type& n) IPADDRESS_NOEXCEPT {
        add(n);
        return *this;
    }

    /**
     * Subtraction assignment operator.
     * 
     * Subtracts a difference_type value from the iterator.
     * 
     * @param[in] n The number to subtract.
     * @return A reference to the updated iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator& operator-=(difference_type n) IPADDRESS_NOEXCEPT {
        sub(n);
        return *this;
    }

    /**
     * Subtraction assignment operator.
     * 
     * Subtracts a uint_type value from the iterator.
     * 
     * @param[in] n The number to subtract.
     * @return A reference to the updated iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator& operator-=(const uint_type& n) IPADDRESS_NOEXCEPT {
        sub(n);
        return *this;
    }

    /**
     * Addition operator.
     * 
     * Creates a new iterator that is the sum of the iterator and a difference_type value.
     * 
     * @param[in] n The number to add.
     * @return A new iterator that is the sum of the iterator and the number.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator+(difference_type n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    /**
     * Addition operator.
     * 
     * Creates a new iterator that is the sum of the iterator and a uint_type value.
     * 
     * @param[in] n The number to add.
     * @return A new iterator that is the sum of the iterator and the number.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator+(const uint_type& n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    /**
     * Addition operator.
     * 
     * Creates a new iterator that is the sum of a difference_type value and the iterator.
     * 
     * @param[in] n The number to add.
     * @param[in] it The iterator to add the number to.
     * @return A new iterator that is the sum of the number and the iterator.
     */
    IPADDRESS_NODISCARD friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator+(difference_type n, const ip_address_iterator& it) IPADDRESS_NOEXCEPT {
        return it + n;
    }

    /**
     * Addition operator.
     * 
     * Creates a new iterator that is the sum of a uint_type value and the iterator.
     * 
     * @param[in] n The number to add.
     * @param[in] it The iterator to add the number to.
     * @return A new iterator that is the sum of the number and the iterator.
     */
    IPADDRESS_NODISCARD friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator+(const uint_type& n, const ip_address_iterator& it) IPADDRESS_NOEXCEPT {
        return it + n;
    }

    /**
     * Subtraction operator.
     * 
     * Creates a new iterator that is the difference of the iterator and a difference_type value.
     * 
     * @param[in] n The number to subtract.
     * @return A new iterator that is the difference of the iterator and the number.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator-(difference_type n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    /**
     * Subtraction operator.
     * 
     * Creates a new iterator that is the difference of the iterator and a uint_type value.
     * 
     * @param[in] n The number to subtract.
     * @return A new iterator that is the difference of the iterator and the number.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator-(const uint_type& n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    /**
     * Subtraction operator.
     * 
     * Calculates the difference in the number of elements between this and another ip_address_iterator.
     * 
     * @param[in] other The ip_address_iterator to compare with.
     * @return The number of elements between this and the other iterator.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type operator-(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return difference_type(_offset - other._offset);
    }

    /**
     * Equality operator.
     * 
     * Compares two ip_address_iterators for equality.
     * 
     * @param[in] other The ip_address_iterator to compare with.
     * @return `true` if the iterators are equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return _carry == other._carry && _current == other._current;
    }

    /**
     * Inequality operator.
     * 
     * Compares two ip_address_iterators for inequality.
     * 
     * @param[in] other The ip_address_iterator to compare with.
     * @return `true` if the iterators are not equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Three-way comparison operator (spaceship operator).
     * 
     * Compares two ip_address_iterators for ordering using the spaceship operator.
     * 
     * @param[in] other The ip_address_iterator to compare with.
     * @return The result of the comparison as a std::strong_ordering value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        if (const auto result = _carry <=> other._carry; result == std::strong_ordering::equivalent) {
            return _current <=> other._current;
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Less-than operator.
     * 
     * Compares two ip_address_iterators to determine if the left one is less than the right one.
     * 
     * @param[in] other The ip_address_iterator to compare with.
     * @return `true` if the left iterator is less than the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return _carry < other._carry || (_carry == other._carry && _current < other._current);
    }

    /**
     * Less-than-or-equal-to operator.
     * 
     * Compares two ip_address_iterators to determine if the left one is less than or equal to the right one.
     * 
     * @param[in] other The ip_address_iterator to compare with.
     * @return `true` if the left iterator is less than or equal to the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }

    /**
     * Greater-than operator.
     * 
     * Compares two ip_address_iterators to determine if the left one is greater than the right one.
     * 
     * @param[in] other The ip_address_iterator to compare with.
     * @return `true` if the left iterator is greater than the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }

    /**
     * Greater-than-or-equal-to operator.
     * 
     * Compares two ip_address_iterators to determine if the left one is greater than or equal to the right one.
     * 
     * @param[in] other The ip_address_iterator to compare with.
     * @return `true` if the left iterator is greater than or equal to the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this < other);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator base() const IPADDRESS_NOEXCEPT {
        auto result = *this;
        const auto old = result._offset;
        ++result._offset;
        if (result._offset < old) {
            result._carry = 1 - result._carry;
        }
        result._current = value_type::from_uint(result._offset);
        return result;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator reverse() const IPADDRESS_NOEXCEPT {
        auto result = *this;
        const auto old = result._offset;
        --result._offset;
        if (result._offset > old) {
            result._carry = 1 - result._carry;
        }
        result._current = value_type::from_uint(result._offset);
        return result;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void add(const uint_type& n) IPADDRESS_NOEXCEPT {
        if (n != 0) {
            const auto old = _offset;
            _offset += n;
            if (_offset < old) {
                _carry = 1 - _carry;
            }
            _current = value_type::from_uint(_offset);
        }
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void sub(const uint_type& n) IPADDRESS_NOEXCEPT {
        if (n != 0) {
            const auto old = _offset;
            _offset -= n;
            if (_offset > old) {
                _carry = 1 - _carry;
            }
            _current = value_type::from_uint(_offset);
        }
    }

    template <typename>
    friend class ip_reverse_iterator;

    template <typename>
    friend class ip_network_iterator;

    template <typename, typename, typename>
    friend class ip_any_iterator;

    value_type _current{};
    uint_type _offset{};
    int _carry{};
};

template <typename>
class hosts_sequence;

/**
 * A sequence of host IP addresses.
 * 
 * The hosts_sequence class template is specialized for `ip_address_base<Base>` to provide a container-like interface for IP address sequences.
 * It allows for iteration over the host addresses in a network or subnet, excluding the network and broadcast addresses when appropriate.
 * This specialization is essential for network-related operations that require processing of individual host addresses within a given range.
 * 
 * @tparam Base The base type from which the ip_address_base is derived, representing the underlying IP address type.
 * @remark When iterating, obtaining addresses occurs through lazy calculations.
 */
template <typename Base>
class hosts_sequence<ip_address_base<Base>> {
public:
    using value_type      = ip_address_base<Base>; /**< The type of the IP addresses in the sequence. */
    using size_type       = std::size_t; /**< The type used for representing the size of the sequence. */
    using difference_type = typename value_type::uint_type; /**< The type used for representing differences between iterators. */
    using pointer         = value_type*; /**< The pointer type for the value_type. */
    using const_pointer   = const value_type*; /**< The const pointer type for the value_type. */
    using reference       = value_type&; /**< The reference type for the value_type. */
    using const_reference = const value_type&; /**< The const reference type for the value_type. */

    using iterator       = ip_address_iterator<value_type>; /**< The iterator type for iterating over the sequence. */
    using const_iterator = ip_address_iterator<value_type>; /**< The const iterator type for iterating over the sequence. */

    using reverse_iterator       = ip_reverse_iterator<iterator>; /**< The reverse iterator type for iterating over the sequence in reverse. */
    using const_reverse_iterator = ip_reverse_iterator<const_iterator>; /**< The const reverse iterator type for iterating over the sequence in reverse. */

    /**
     * Constructs a hosts_sequence with specified network parameters.
     * 
     * Initializes the sequence based on the provided network address, broadcast address, and prefix lengths.
     * The sequence excludes the network and broadcast addresses when appropriate, according to the prefix length.
     * 
     * @param[in] network_address The network address of the subnet.
     * @param[in] broadcast_address The broadcast address of the subnet.
     * @param[in] prefixlen The prefix length of the subnet.
     * @param[in] max_prefixlen The maximum prefix length possible for the IP version.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_sequence(const_reference network_address, const_reference broadcast_address, size_t prefixlen, size_t max_prefixlen) IPADDRESS_NOEXCEPT  {
        if (prefixlen == max_prefixlen - 1) {
            const auto begin = value_type::from_uint(network_address.to_uint());
            const auto end = value_type::from_uint(broadcast_address.to_uint() + 1);
            _begin = const_iterator(begin);
            _end = const_iterator(end);
        } else if (prefixlen == max_prefixlen) {
            const auto begin = value_type::from_uint(network_address.to_uint());
            const auto end = value_type::from_uint(network_address.to_uint() + 1);
            _begin = const_iterator(begin);
            _end = const_iterator(end, end < begin ? 1 : 0);
        } else {
            const auto begin = value_type::from_uint(network_address.to_uint() + 1);
            const auto end = value_type::from_uint(broadcast_address.to_uint() + (std::is_same<value_type, ipv6_address>::value ? 1 : 0));
            _begin = const_iterator(begin);
            _end = const_iterator(end);
        }
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
};

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_HOSTS_HPP
