#ifndef IPADDRESS_IP_ANY_ITERATOR_HPP
#define IPADDRESS_IP_ANY_ITERATOR_HPP

#include "ip-any-address.hpp"
#include "ip-network-iterator.hpp"

namespace IPADDRESS_NAMESPACE {

template <typename T, typename Ipv4Iter, typename Ipv6Iter>
class ip_any_iterator {
public:
#ifdef IPADDRESS_HAS_CONCEPTS
    using iterator_concept  = std::random_access_iterator_tag;
#endif
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = T;
    using difference_type   = std::int64_t;
    using pointer           = const value_type*;
    using reference         = const value_type&;

    using uint_type         = uint128_t;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator() IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator(const ip_any_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator(ip_any_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_any_iterator& operator=(const ip_any_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_any_iterator& operator=(ip_any_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator(Ipv4Iter iter) IPADDRESS_NOEXCEPT : _iter(iter), _version(ip_version::V4), _current(*iter) {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator(Ipv6Iter iter) IPADDRESS_NOEXCEPT : _iter(iter), _version(ip_version::V6), _current(*iter) {
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t uint_diff(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        if (_version != other._version) {
            return 0;
        }
        return _version == ip_version::V4 ? uint128_t(_iter.iter4.uint_diff(other._iter.iter4)) : _iter.iter6.uint_diff(other._iter.iter6);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator*() const IPADDRESS_NOEXCEPT {
        return _current;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer operator->() const IPADDRESS_NOEXCEPT {
        return &_current;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type operator[](difference_type n) const IPADDRESS_NOEXCEPT {
        const auto& it = *this;
        return it[uint128_t(n)];
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE value_type operator[](const uint128_t& n) const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? value_type(_iter.iter4[uint32_t(n)]) : value_type(_iter.iter6[n]);
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_any_iterator& operator++() IPADDRESS_NOEXCEPT {
        add(1);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_any_iterator operator++(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_any_iterator& operator--() IPADDRESS_NOEXCEPT {
        sub(1);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_any_iterator operator--(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_any_iterator& operator+=(difference_type n) IPADDRESS_NOEXCEPT {
        add(n);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_any_iterator& operator+=(const uint128_t& n) IPADDRESS_NOEXCEPT {
        add(n);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_any_iterator& operator-=(difference_type n) IPADDRESS_NOEXCEPT {
        sub(n);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_any_iterator& operator-=(const uint128_t& n) IPADDRESS_NOEXCEPT {
        sub(n);
        return *this;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator+(difference_type n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator+(const uint128_t& n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    IPADDRESS_NODISCARD friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator+(difference_type n, const ip_any_iterator& it) IPADDRESS_NOEXCEPT {
        return it + n;
    }

    IPADDRESS_NODISCARD friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator+(const uint128_t& n, const ip_any_iterator& it) IPADDRESS_NOEXCEPT {
        return it + n;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator-(difference_type n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_iterator operator-(const uint128_t& n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type operator-(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        if (_version != other._version) {
            return 0;
        }
        return _version == ip_version::V4 ? _iter.iter4 - other._iter.iter4 : _iter.iter6 - other._iter.iter6;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        if (_version != other._version) {
            return false;
        }
        return _version == ip_version::V4 ? _iter.iter4 == other._iter.iter4 : _iter.iter6 == other._iter.iter6;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        if (const auto result = _version <=> other._version; result == std::strong_ordering::equivalent) {
            return _version == ip_version::V4 ? _iter.iter4 <=> other._iter.iter4 : _iter.iter6 <=> other._iter.iter6;
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        if (_version != other._version) {
            return false;
        }
        return _version == ip_version::V4 ? _iter.iter4 < other._iter.iter4 : _iter.iter6 < other._iter.iter6;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_any_iterator& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }

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

class hosts_any_sequence {
public:
    using value_type      = ip_address;
    using size_type       = std::size_t;
    using difference_type = uint128_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator       = ip_any_iterator<value_type, ip_address_iterator<ipv4_address>, ip_address_iterator<ipv6_address>>;
    using const_iterator = iterator;

    using reverse_iterator       = ip_reverse_iterator<iterator>;
    using const_reverse_iterator = ip_reverse_iterator<const_iterator>;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_any_sequence(const hosts_any_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_any_sequence(hosts_any_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE hosts_any_sequence& operator=(const hosts_any_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE hosts_any_sequence& operator=(hosts_any_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_any_sequence(ip_address_iterator<ipv4_address> begin, ip_address_iterator<ipv4_address> end) IPADDRESS_NOEXCEPT : _begin(begin), _end(end) {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_any_sequence(ip_address_iterator<ipv6_address> begin, ip_address_iterator<ipv6_address> end) IPADDRESS_NOEXCEPT : _begin(begin), _end(end) {
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
        return _end.uint_diff(_begin);
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
}; // hosts_any_sequence

template <typename T>
class subnets_any_sequence {
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using difference_type = uint128_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator       = ip_any_iterator<value_type, ip_network_iterator<ipv4_network>, ip_network_iterator<ipv6_network>>;
    using const_iterator = iterator;

    using reverse_iterator       = ip_reverse_iterator<iterator>;
    using const_reverse_iterator = ip_reverse_iterator<const_iterator>;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_any_sequence(const subnets_any_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_any_sequence(subnets_any_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE subnets_any_sequence& operator=(const subnets_any_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE subnets_any_sequence& operator=(subnets_any_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_any_sequence(ip_network_iterator<ipv4_network> begin, ip_network_iterator<ipv4_network> end) IPADDRESS_NOEXCEPT : _begin(begin), _end(end) {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_any_sequence(ip_network_iterator<ipv6_network> begin, ip_network_iterator<ipv6_network> end) IPADDRESS_NOEXCEPT : _begin(begin), _end(end) {
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
        return _end.uint_diff(_begin);
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
}; // subnets_any_sequence

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_ANY_ITERATOR_HPP
