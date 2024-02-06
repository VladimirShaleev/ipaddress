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

    using ip_address_type   = typename value_type::ip_address_type;
    using uint_type         = typename value_type::uint_type;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator() IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator(const ip_network_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator(ip_network_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator=(const ip_network_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator=(ip_network_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator(const ip_address_type& begin, const ip_address_type& end, const ip_address_type& ref, const uint_type& step, size_t prefixlen) IPADDRESS_NOEXCEPT
        : _current(value_type::from_address(ref, prefixlen)), _it(ip_address_iterator<ip_address_type>(begin, end, ref)), _step(step), _prefixlen(prefixlen) {
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint_type uint_diff(const ip_network_iterator& other) const IPADDRESS_NOEXCEPT {
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
        return it[uint_type(n)];
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator[](const uint_type& n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
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

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator+=(const uint_type& n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        add(n);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator-=(difference_type n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        sub(n);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_network_iterator& operator-=(const uint_type& n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        sub(n);
        return *this;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator+(difference_type n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator+(const uint_type& n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    friend IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator+(difference_type n, const ip_network_iterator& it) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return it + n;
    }

    friend IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator+(const uint_type& n, const ip_network_iterator& it) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return it + n;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator-(difference_type n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_iterator operator-(const uint_type& n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
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
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void add(const uint_type& n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        _it += _step * n;
        _current = value_type::from_address(*_it, _prefixlen);
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void sub(const uint_type& n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        _it -= _step * n;
        _current = value_type::from_address(*_it, _prefixlen);
    }

    value_type _current{};
    value_type _at{};
    ip_address_iterator<ip_address_type> _it{};
    uint_type _step{};
    size_t _prefixlen{};
}; // ip_network_iterator

template <typename T>
class ip_exclude_network_iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = T;
    using difference_type   = std::int64_t;
    using pointer           = const value_type*;
    using reference         = const value_type&;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_exclude_network_iterator() IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_exclude_network_iterator(const ip_exclude_network_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_exclude_network_iterator(ip_exclude_network_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_exclude_network_iterator& operator=(const ip_exclude_network_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_exclude_network_iterator& operator=(ip_exclude_network_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_exclude_network_iterator(reference network, reference other) IPADDRESS_NOEXCEPT : _other(other), _current(network) {
        const auto subnets = network.subnets();
        _s1 = subnets[0];
        _s2 = subnets[1];
        ++(*this);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator*() const IPADDRESS_NOEXCEPT {
        return _current;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer operator->() const IPADDRESS_NOEXCEPT {
        return &_current;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_exclude_network_iterator& operator++() IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
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

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_exclude_network_iterator operator++(int) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return _current == other._current;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef GALAXY_HPP_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return _current <=> other._current;
    }

#else // !GALAXY_HPP_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return _current < other._current;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_exclude_network_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this < other);
    }

#endif // !GALAXY_HPP_HAS_SPACESHIP_OPERATOR

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

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using ip_address_type = typename value_type::ip_address_type;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence(const subnets_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence(subnets_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE subnets_sequence& operator=(const subnets_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE subnets_sequence& operator=(subnets_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit subnets_sequence(const ip_address_type& network_address, size_t new_prefixlen) IPADDRESS_NOEXCEPT {
        const auto begin = ip_address_type::from_bytes(network_address.bytes());
        const auto end = ip_address_type::from_uint(network_address.to_uint() + 1);
        const auto step = difference_type(1);
        _begin = const_iterator(begin, end, begin, step, new_prefixlen);
        _end = const_iterator(begin, end, end, step, new_prefixlen);
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence(const ip_address_type& network_address, const ip_address_type& broadcast_address, const ip_address_type& hostmask, size_t prefixlen_diff, size_t new_prefixlen) IPADDRESS_NOEXCEPT {
        const auto begin = ip_address_type::from_bytes(network_address.bytes());
        const auto end = ip_address_type::from_uint(broadcast_address.to_uint() + 1);
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
        return _begin == _end;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type size() const IPADDRESS_NOEXCEPT {
        return _end.uint_diff(_begin);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type operator[](difference_type n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return at(n);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type at(difference_type n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return *(_begin + n);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type front() const IPADDRESS_NOEXCEPT {
        return *_begin;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type back() const IPADDRESS_NOEXCEPT {
        return *(_end - 1);
    }

private:
    const_iterator _begin{};
    const_iterator _end{};
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

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE exclude_network_sequence& operator=(const exclude_network_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE exclude_network_sequence& operator=(exclude_network_sequence&&) IPADDRESS_NOEXCEPT = default;

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
