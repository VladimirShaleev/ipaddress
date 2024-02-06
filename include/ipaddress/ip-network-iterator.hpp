#ifndef IPADDRESS_IP_NETWORK_ITERATOR_HPP
#define IPADDRESS_IP_NETWORK_ITERATOR_HPP

#include "config.hpp"
#include "ip-address-iterator.hpp"

namespace IPADDRESS_NAMESPACE {

template <typename T>
class ip_network_iterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = T;
    using difference_type   = std::int64_t;
    using pointer           = const value_type*;
    using reference         = const value_type&;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator() IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator(const ip_network_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator(ip_network_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator=(const ip_network_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator=(ip_network_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator(const value_type::ip_address_type& begin, const value_type::ip_address_type& end, const value_type::ip_address_type& ref, const value_type::uint_type& step, size_t prefixlen) IPADDRESS_NOEXCEPT
        : _current(value_type::from_address(ref, prefixlen)), _it(ip_address_iterator<typename value_type::ip_address_type>(begin, end, ref)), _step(step), _prefixlen(prefixlen) {
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type::uint_type uint_diff(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return _it.uint_diff(other._it);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator*() const IPADDRESS_NOEXCEPT {
        return _current;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer operator->() const IPADDRESS_NOEXCEPT {
        return &_current;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator[](difference_type n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        const auto& it = *this;
        return it[value_type::uint_type(n)];
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator[](const value_type::uint_type& n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto address = _it + _step * n;
        _at = value_type::from_address(*address, _prefixlen);
        return _at;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator++() IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        add(1);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator operator++(int) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator--() IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        sub(1);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator operator--(int) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator+=(difference_type n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        add(n);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator+=(const value_type::uint_type& n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        add(n);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator-=(difference_type n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        sub(n);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator-=(const value_type::uint_type& n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        sub(n);
        return *this;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator+(difference_type n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator+(const value_type::uint_type& n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    friend IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator+(difference_type n, const ip_network_iterator& it) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return it + n;
    }

    friend IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator+(const value_type::uint_type& n, const ip_network_iterator& it) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return it + n;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator-(difference_type n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator-(const value_type::uint_type& n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type operator-(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return difference_type(_it - other._it);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return _current == other._current;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef GALAXY_HPP_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return _current <=> other._current;
    }

#else // !GALAXY_HPP_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return _current < other._current;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this < other);
    }

#endif // !GALAXY_HPP_HAS_SPACESHIP_OPERATOR

private:
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void add(value_type::uint_type n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        _it += _step * n;
        _current = value_type::from_address(*_it, _prefixlen);
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void sub(value_type::uint_type n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        _it -= _step * n;
        _current = value_type::from_address(*_it, _prefixlen);
    }

    value_type _current{};
    value_type _at{};
    ip_address_iterator<typename value_type::ip_address_type> _it{};
    value_type::uint_type _step{};
    size_t _prefixlen{};
};

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

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence(const subnets_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence(subnets_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE subnets_sequence& operator=(const subnets_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE subnets_sequence& operator=(subnets_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit subnets_sequence(const value_type::ip_address_type& network_address, size_t new_prefixlen) IPADDRESS_NOEXCEPT {
        const auto begin = value_type::ip_address_type::from_bytes(network_address.bytes());
        const auto end = value_type::ip_address_type::from_uint(network_address.to_uint() + 1);
        const auto step = difference_type(1);
        _begin = const_iterator(begin, end, begin, step, new_prefixlen);
        _end = const_iterator(begin, end, end, step, new_prefixlen);
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence(const value_type::ip_address_type& network_address, const value_type::ip_address_type& broadcast_address, const value_type::ip_address_type& hostmask, size_t prefixlen_diff, size_t new_prefixlen) IPADDRESS_NOEXCEPT {
        const auto begin = value_type::ip_address_type::from_bytes(network_address.bytes());
        const auto end = value_type::ip_address_type::from_uint(broadcast_address.to_uint() + 1);
        const auto step = (hostmask.to_uint() + 1) >> prefixlen_diff;
        _begin = const_iterator(begin, end, begin, step, new_prefixlen);
        _end = const_iterator(begin, end, end, step, new_prefixlen);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return _begin;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return _end;
    }

private:
    const_iterator _begin{};
    const_iterator _end{};
};

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_NETWORK_ITERATOR_HPP
