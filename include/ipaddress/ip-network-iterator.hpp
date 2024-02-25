/**
 * @file      ip-network-iterator.hpp
 * @brief     Iterator utilities for IP network address ranges
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This header provides iterator classes for traversing and manipulating
 * IP network address ranges. It includes iterators for iterating over individual
 * IP addresses within a network, excluding certain subnets, and iterating over
 * subnets within a larger network. These utilities are essential for applications
 * that require detailed control over IP address management and enumeration.
 */

#ifndef IPADDRESS_IP_NETWORK_ITERATOR_HPP
#define IPADDRESS_IP_NETWORK_ITERATOR_HPP

#include "config.hpp"
#include "ip-address-iterator.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * An iterator for traversing IP addresses within a network range.
 * 
 * This class template provides a random-access iterator that allows
 * traversal over a range of IP addresses within a network. It supports
 * operations typical of random-access iterators, such as increment,
 * decrement, and direct access to elements at a specific offset.
 * 
 * @tparam T The type of IPv4 or IPv6 network to iterate over.
 */
template <typename T>
class ip_network_iterator {
public:
    using iterator_category = std::random_access_iterator_tag; /**< The category of the iterator. */
    using value_type        = T; /**< The type of value iterated over. */
    using difference_type   = std::int64_t; /**< Type to represent the difference between two iterators. */
    using pointer           = const value_type*; /**< Pointer to the value type. */
    using reference         = const value_type&; /**< Reference to the value type. */
 
    using ip_address_type   = typename value_type::ip_address_type; /**< The underlying IP address type. */
    using uint_type         = typename value_type::uint_type; /**< Unsigned integer type used for addressing. */

    /**
     * Default constructor.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator() IPADDRESS_NOEXCEPT = default;

    /**
     * Copy constructor.
     * 
     * @param[in] other The ip_network_iterator to copy.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator(const ip_network_iterator& other) IPADDRESS_NOEXCEPT = default;

    /**
     * Move constructor.
     * 
     * @param[in] other The ip_network_iterator to move.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator(ip_network_iterator&&) IPADDRESS_NOEXCEPT = default;

    /**
     * Copy assignment operator.
     * 
     * @param[in] other The ip_network_iterator to copy.
     * @return A reference to the assigned ip_network_iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator& operator=(const ip_network_iterator& other) IPADDRESS_NOEXCEPT = default;
    
    /**
     * Move assignment operator.
     * 
     * Moves the value of one ip_network_iterator to another.
     * 
     * @param[in] other The ip_network_iterator to move.
     * @return A reference to the moved ip_network_iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator& operator=(ip_network_iterator&& other) IPADDRESS_NOEXCEPT = default;

    /**
     * Constructs an iterator with a reference IP address, step size, prefix length, and carry.
     * 
     * @param[in] ref The reference IP address for the iterator.
     * @param[in] step The step size for each iteration.
     * @param[in] prefixlen The prefix length defining the network.
     * @param[in] carry An optional carry value for overflow handling.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator(const ip_address_type& ref, const uint_type& step, size_t prefixlen, int carry = 0) IPADDRESS_NOEXCEPT
        : _current(value_type::from_address(ref, prefixlen)), _it(ip_address_iterator<ip_address_type>(ref, carry)), _step(step), _prefixlen(prefixlen) {
    }

    /**
     * Calculates the difference in the number of elements between this and another ip_network_iterator.
     * 
     * @param[in] other The ip_network_iterator to compare with.
     * @return The number of elements between this and the other iterator.
     * @remark This is a special function for calculate the difference between iterators, 
     *         which can correctly represent all addresses using the integer number uint128_t
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint_type uint_diff(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return _it.uint_diff(other._it) / _step;
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
        const auto address = _it + _step * n;
        return value_type::from_address(*address, _prefixlen);
    }

    /**
     * Pre-increment operator.
     * 
     * Increments the iterator to the next element.
     * 
     * @return A reference to the incremented iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator& operator++() IPADDRESS_NOEXCEPT {
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
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator++(int) IPADDRESS_NOEXCEPT {
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
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator& operator--() IPADDRESS_NOEXCEPT {
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
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator--(int) IPADDRESS_NOEXCEPT {
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
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator& operator+=(difference_type n) IPADDRESS_NOEXCEPT {
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
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator& operator+=(const uint_type& n) IPADDRESS_NOEXCEPT {
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
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator& operator-=(difference_type n) IPADDRESS_NOEXCEPT {
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
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator& operator-=(const uint_type& n) IPADDRESS_NOEXCEPT {
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
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator+(difference_type n) const IPADDRESS_NOEXCEPT {
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
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator+(const uint_type& n) const IPADDRESS_NOEXCEPT {
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
    IPADDRESS_NODISCARD friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator+(difference_type n, const ip_network_iterator& it) IPADDRESS_NOEXCEPT {
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
    IPADDRESS_NODISCARD friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator+(const uint_type& n, const ip_network_iterator& it) IPADDRESS_NOEXCEPT {
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
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator-(difference_type n) const IPADDRESS_NOEXCEPT {
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
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator-(const uint_type& n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    /**
     * Subtraction operator.
     * 
     * Calculates the difference in the number of elements between this and another ip_network_iterator.
     * 
     * @param[in] other The ip_network_iterator to compare with.
     * @return The number of elements between this and the other iterator.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type operator-(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return difference_type(_it - other._it);
    }

    /**
     * Equality operator.
     * 
     * Compares two ip_network_iterators for equality.
     * 
     * @param[in] other The ip_network_iterator to compare with.
     * @return `true` if the iterators are equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return _it._carry == other._it._carry && _current == other._current;
    }

    /**
     * Inequality operator.
     * 
     * Compares two ip_network_iterators for inequality.
     * 
     * @param[in] other The ip_network_iterator to compare with.
     * @return `true` if the iterators are not equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Three-way comparison operator (spaceship operator).
     * 
     * Compares two ip_network_iterators for ordering using the spaceship operator.
     * 
     * @param[in] other The ip_network_iterator to compare with.
     * @return The result of the comparison as a std::strong_ordering value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        if (const auto result = _it._carry <=> other._it._carry; result == std::strong_ordering::equivalent) {
            return _current <=> other._current;
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Less-than operator.
     * 
     * Compares two ip_network_iterators to determine if the left one is less than the right one.
     * 
     * @param[in] other The ip_network_iterator to compare with.
     * @return `true` if the left iterator is less than the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return _it._carry < other._it._carry || (_it._carry == other._it._carry && _current < other._current);
    }

    /**
     * Less-than-or-equal-to operator.
     * 
     * Compares two ip_network_iterators to determine if the left one is less than or equal to the right one.
     * 
     * @param[in] other The ip_network_iterator to compare with.
     * @return `true` if the left iterator is less than or equal to the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }

    /**
     * Greater-than operator.
     * 
     * Compares two ip_network_iterators to determine if the left one is greater than the right one.
     * 
     * @param[in] other The ip_network_iterator to compare with.
     * @return `true` if the left iterator is greater than the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }

    /**
     * Greater-than-or-equal-to operator.
     * 
     * Compares two ip_network_iterators to determine if the left one is greater than or equal to the right one.
     * 
     * @param[in] other The ip_network_iterator to compare with.
     * @return `true` if the left iterator is greater than or equal to the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this < other);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator base() const IPADDRESS_NOEXCEPT {
        auto result = *this;
        const auto old = result._it._offset;
        result._it._offset += _step;
        if (result._it._offset < old) {
            result._it._carry = 1 - result._it._carry;
        }
        result._it._current = ip_address_type::from_uint(result._it._offset);
        result._current = value_type::from_address(*result._it, result._prefixlen);
        return result;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator reverse() const IPADDRESS_NOEXCEPT {
        auto result = *this;
        const auto old = result._it._offset;
        result._it._offset -= _step;
        if (result._it._offset > old) {
        result._it._carry = 1 - result._it._carry;
        }
        result._it._current = ip_address_type::from_uint(result._it._offset);
        result._current = value_type::from_address(*result._it, result._prefixlen);
        return result;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void add(const uint_type& n) IPADDRESS_NOEXCEPT {
        _it += _step * n;
        _current = value_type::from_address(*_it, _prefixlen);
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void sub(const uint_type& n) IPADDRESS_NOEXCEPT {
        _it -= _step * n;
        _current = value_type::from_address(*_it, _prefixlen);
    }

    template <typename>
    friend class ip_reverse_iterator;

    template <typename, typename, typename>
    friend class ip_any_iterator;

    value_type _current{};
    value_type _at{};
    ip_address_iterator<ip_address_type> _it{};
    uint_type _step{};
    size_t _prefixlen{};
}; // ip_network_iterator

/**
 * An iterator to traverse IP addresses within a network, excluding specified subnets.
 * 
 * This iterator advances through IP addresses within a specified network,
 * skipping over those that belong to a subnet that should be excluded from
 * the traversal. This is particularly useful for operations where certain
 * ranges of IP addresses are reserved or otherwise should not be included.
 * 
 * @tparam T The type of IPv4 or IPv6 network to iterate over, excluding specified subnets.
 */
template <typename T>
class ip_exclude_network_iterator {
public:
    using iterator_category = std::forward_iterator_tag; /**< The category of the iterator. */
    using value_type        = T; /**< The type of value iterated over. */
    using difference_type   = std::int64_t; /**< Type to represent the difference between two iterators. */
    using pointer           = const value_type*; /**< Pointer to the value type. */
    using reference         = const value_type&; /**< Reference to the value type. */

    /**
     * Default constructor.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_exclude_network_iterator() IPADDRESS_NOEXCEPT = default;

    /**
     * Copy constructor.
     * 
     * @param[in] other The ip_exclude_network_iterator to copy.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_exclude_network_iterator(const ip_exclude_network_iterator& other) IPADDRESS_NOEXCEPT = default;
    
    /**
     * Move constructor.
     * 
     * @param[in] other The ip_exclude_network_iterator to move.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_exclude_network_iterator(ip_exclude_network_iterator&& other) IPADDRESS_NOEXCEPT = default;

    /**
     * Copy assignment operator.
     * 
     * @param[in] other The ip_exclude_network_iterator to copy.
     * @return A reference to the assigned ip_exclude_network_iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_exclude_network_iterator& operator=(const ip_exclude_network_iterator& other) IPADDRESS_NOEXCEPT = default;
    
    /**
     * Move assignment operator.
     * 
     * @param[in] other The ip_exclude_network_iterator to move.
     * @return A reference to the moved ip_exclude_network_iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_exclude_network_iterator& operator=(ip_exclude_network_iterator&&) IPADDRESS_NOEXCEPT = default;

    /**
     * Constructs an iterator for a network, excluding addresses from another network.
     * 
     * @param[in] network The network to iterate over.
     * @param[in] other The network whose addresses are to be excluded.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_exclude_network_iterator(reference network, reference other) IPADDRESS_NOEXCEPT : _other(other), _current(network) {
        const auto subnets = network.subnets();
        _s1 = subnets[0];
        _s2 = subnets[1];
        ++(*this);
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
     * Pre-increment operator.
     * 
     * @return A reference to the incremented iterator.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_exclude_network_iterator& operator++() IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_s1 != _other && _s2 != _other) {
            if (_other.subnet_of(_s1)) {
                if (_current != _s2) {
                    _current = _s2;
                    const auto subnets = _s1.subnets();
                    _s1 = subnets[0];
                    _s2 = subnets[1];
                } else {
                    *this = ip_exclude_network_iterator();
                }
            } else if (_other.subnet_of(_s2)) {
                if (_current != _s1) {
                    _current = _s1;
                    const auto subnets = _s2.subnets();
                    _s1 = subnets[0];
                    _s2 = subnets[1];
                } else {
                    *this = ip_exclude_network_iterator();
                }
            } else {
            #ifndef IPADDRESS_NO_EXCEPTIONS
                throw std::out_of_range("index out of range");
            #endif
            }
        } else {
            if (_s1 == _other) {
                if (_current != _s2) {
                    _current = _s2;
                } else {
                    *this = ip_exclude_network_iterator();
                }
            } else if (_s2 == _other) {
                if (_current != _s1) {
                    _current = _s1;
                } else {
                    *this = ip_exclude_network_iterator();
                }
            } else {
            #ifndef IPADDRESS_NO_EXCEPTIONS
                throw std::out_of_range("index out of range");
            #endif
            }
        }
        return *this;
    }

    /**
     * Post-increment operator.
     * 
     * @return The iterator before incrementing.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_exclude_network_iterator operator++(int) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    /**
     * Equality operator.
     * 
     * Compares two ip_exclude_network_iterator for equality.
     * 
     * @param[in] other The ip_exclude_network_iterator to compare with.
     * @return `true` if the iterators are equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return _current == other._current;
    }

    /**
     * Inequality operator.
     * 
     * Compares two ip_exclude_network_iterator for inequality.
     * 
     * @param[in] other The ip_exclude_network_iterator to compare with.
     * @return `true` if the iterators are not equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Three-way comparison operator (spaceship operator).
     * 
     * Compares two ip_exclude_network_iterator for ordering.
     * 
     * @param[in] other The ip_exclude_network_iterator to compare with.
     * @return The result of the comparison as a std::strong_ordering value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return other._current <=> _current;
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Less-than operator.
     * 
     * Compares two ip_exclude_network_iterator to determine if the left one is less than the right one.
     * 
     * @param[in] other The ip_exclude_network_iterator to compare with.
     * @return `true` if the left iterator is less than the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return other._current < _current;
    }

    /**
     * Less-than-or-equal-to operator.
     * 
     * Compares two ip_exclude_network_iterator to determine if the left one is less than or equal to the right one.
     * 
     * @param[in] other The ip_exclude_network_iterator to compare with.
     * @return `true` if the left iterator is less than or equal to the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }

    /**
     * Greater-than operator.
     * 
     * Compares two ip_exclude_network_iterator to determine if the left one is greater than the right one.
     * 
     * @param[in] other The ip_exclude_network_iterator to compare with.
     * @return `true` if the left iterator is greater than the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }

    /**
     * Greater-than-or-equal-to operator.
     * 
     * Compares two ip_exclude_network_iterator to determine if the left one is greater than or equal to the right one.
     * 
     * @param[in] other The ip_exclude_network_iterator to compare with.
     * @return `true` if the left iterator is greater than or equal to the right iterator, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this < other);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    value_type _other{};
    value_type _s1{};
    value_type _s2{};
    value_type _current{};
}; // ip_exclude_network_iterator

template <typename T>
class subnets_sequence {
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using difference_type = typename value_type::uint_type;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator       = ip_network_iterator<value_type>;
    using const_iterator = ip_network_iterator<value_type>;

    using reverse_iterator       = ip_reverse_iterator<iterator>;
    using const_reverse_iterator = ip_reverse_iterator<const_iterator>;

    using ip_address_type = typename value_type::ip_address_type;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence() IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence(const subnets_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence(subnets_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence& operator=(const subnets_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence& operator=(subnets_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence(const ip_address_type& network_address, size_t new_prefixlen) IPADDRESS_NOEXCEPT {
        const auto begin = ip_address_type::from_bytes(network_address.bytes());
        const auto end = ip_address_type::from_uint(network_address.to_uint() + 1);
        const auto step = difference_type(1);
        _begin = const_iterator(begin, step, new_prefixlen);
        _end = const_iterator(end, step, new_prefixlen, end < begin ? 1 : 0);
        _size = 1U;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence(const ip_address_type& network_address, const ip_address_type& broadcast_address, const ip_address_type& hostmask, size_t prefixlen_diff, size_t new_prefixlen) IPADDRESS_NOEXCEPT {
        const auto begin_uint = network_address.to_uint();
        const auto end_uint = broadcast_address.to_uint();
        const auto begin = ip_address_type::from_uint(begin_uint);
        const auto end = ip_address_type::from_uint(end_uint + 1);
        const auto step = (hostmask.to_uint() >> prefixlen_diff) + 1;
        _begin = const_iterator(begin, step, new_prefixlen);
        _end = const_iterator(end, step, new_prefixlen, begin == end ? 1 : 0);
        _size = (end_uint - begin_uint) / step + 1;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return _begin;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return _end;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool empty() const IPADDRESS_NOEXCEPT {
        return _begin == _end;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type size() const IPADDRESS_NOEXCEPT {
        return _size;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type operator[](difference_type n) const IPADDRESS_NOEXCEPT {
        return at(n);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type at(difference_type n) const IPADDRESS_NOEXCEPT {
        return *(_begin + n);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type front() const IPADDRESS_NOEXCEPT {
        return *_begin;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type back() const IPADDRESS_NOEXCEPT {
        return *(_end - 1U);
    }

private:
    const_iterator _begin{};
    const_iterator _end{};
    difference_type _size{};
}; // subnets_sequence

template <typename T>
class exclude_network_sequence {
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using difference_type = typename value_type::uint_type;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using iterator       = ip_exclude_network_iterator<value_type>;
    using const_iterator = ip_exclude_network_iterator<value_type>;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE exclude_network_sequence(const exclude_network_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE exclude_network_sequence(exclude_network_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE exclude_network_sequence& operator=(const exclude_network_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE exclude_network_sequence& operator=(exclude_network_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit exclude_network_sequence() IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE exclude_network_sequence(const_reference network, const_reference other) IPADDRESS_NOEXCEPT 
        : _begin(const_iterator(network, other)) {
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return _begin;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return _end;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool empty() const IPADDRESS_NOEXCEPT {
        return _begin == _end;
    }
    
private:
    const_iterator _begin{};
    const_iterator _end{};
}; // exclude_network_sequence

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_NETWORK_ITERATOR_HPP
