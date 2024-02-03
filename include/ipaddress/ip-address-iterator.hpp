#ifndef IPADDRESS_IP_ADDRESS_ITERATOR_HPP
#define IPADDRESS_IP_ADDRESS_ITERATOR_HPP

#include "config.hpp"
#include "ipv4-address.hpp"
#include "ipv6-address.hpp"

namespace IPADDRESS_NAMESPACE {

struct big_integer {
    std::uint64_t far;
    std::uint64_t low;
};

template <typename>
struct ip_address_iterator;

template <typename Base>
struct ip_address_iterator<ip_address_base<Base>> {
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = ip_address_base<Base>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = const value_type*;
    using reference         = const value_type&;

    IPADDRESS_CONSTEXPR ip_address_iterator() IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR ip_address_iterator(const ip_address_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR ip_address_iterator(ip_address_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 ip_address_iterator& operator=(const ip_address_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 ip_address_iterator& operator=(ip_address_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator(reference ref) IPADDRESS_NOEXCEPT : _current(ref) {
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator*() const IPADDRESS_NOEXCEPT {
        return _current;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer operator->() const IPADDRESS_NOEXCEPT {
        return &_current;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator[](difference_type n) const IPADDRESS_NOEXCEPT {
        return *(_current + n);
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator& operator++() IPADDRESS_NOEXCEPT {
        *this += 1;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator operator++(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator& operator--() IPADDRESS_NOEXCEPT {
        *this -= 1;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator operator--(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator& operator+=(difference_type n) IPADDRESS_NOEXCEPT {
        add(n);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator& operator-=(difference_type n) IPADDRESS_NOEXCEPT {
        add(-n);
        return *this;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator+(difference_type n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator-(difference_type n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE big_integer operator-(const ip_address_iterator& other) IPADDRESS_NOEXCEPT {
        return other.diff(*this);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return _current == other._current;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef GALAXY_HPP_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return _current <=> other._current;
    }

#else // !GALAXY_HPP_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return _current < other._current;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this < other);
    }

#endif // !GALAXY_HPP_HAS_SPACESHIP_OPERATOR

private:
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void add(difference_type n) IPADDRESS_NOEXCEPT;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE big_integer diff(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT;

    value_type _current;
};

template <>
IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void ip_address_iterator<ipv4_address>::add(difference_type n) IPADDRESS_NOEXCEPT {
    _current = ipv4_address::from_uint32(_current.to_uint32() + n);
}

template <>
IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void ip_address_iterator<ipv6_address>::add(difference_type n) IPADDRESS_NOEXCEPT {
    const auto& bytes = _current.bytes();
    uint64_t far = 0;
    uint64_t low = 0;
    uint64_t shift = 0;
    uint64_t inc = 8;
    if (is_little_endian()) {
        shift = 56;
        inc = -8;
    }
    for (int i = 0, s = shift; i < 8; ++i, s += inc) {
        far |= uint64_t(bytes[i]) << s;
        low |= uint64_t(bytes[i + 8]) << s;
    }

    far += n + low < low ? 1 : 0;
    low += n;

    for (int i = 0, s = shift; i < 8; ++i, s += inc) {
        auto& fb = const_cast<uint8_t&>(bytes[i]);
        auto& lw = const_cast<uint8_t&>(bytes[i + 8]);
        fb = uint8_t(far >> s);
        lw = uint8_t(low >> s);
    }
}

template <>
IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE big_integer ip_address_iterator<ipv4_address>::diff(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
    return { 0, other._current.to_uint32() - _current.to_uint32() };
}

template <>
IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE big_integer ip_address_iterator<ipv6_address>::diff(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
    const auto& lhs_bytes = _current.bytes();
    const auto& rhs_bytes = other._current.bytes();
    uint64_t lhs_far = 0;
    uint64_t lhs_low = 0;
    uint64_t rhs_far = 0;
    uint64_t rhs_low = 0;
    uint64_t shift = 0;
    uint64_t inc = 8;
    if (is_little_endian()) {
        shift = 56;
        inc = -8;
    }
    for (int i = 0, s = shift; i < 8; ++i, s += inc) {
        lhs_far |= uint64_t(lhs_bytes[i]) << s;
        rhs_far |= uint64_t(rhs_bytes[i]) << s;
        lhs_low |= uint64_t(lhs_bytes[i + 8]) << s;
        rhs_low |= uint64_t(rhs_bytes[i + 8]) << s;
    }

    return { rhs_far - lhs_far - (lhs_low > rhs_low ? 1 : 0), rhs_low - lhs_low };
}

template <typename>
class hosts_sequence;

template <typename Base>
class hosts_sequence<ip_address_base<Base>> {
public:
    using value_type      = ip_address_base<Base>;
    using size_type       = std::size_t;
    using difference_type = std::size_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator       = ip_address_iterator<value_type>;
    using const_iterator = ip_address_iterator<value_type>;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    IPADDRESS_CONSTEXPR hosts_sequence(const hosts_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR hosts_sequence(hosts_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 hosts_sequence& operator=(const hosts_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 hosts_sequence& operator=(hosts_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR hosts_sequence(const_reference network_address, const_reference broadcast_address) IPADDRESS_NOEXCEPT 
    : _network_address(network_address), _broadcast_address(broadcast_address)
    {
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return ++const_iterator(_network_address);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(_broadcast_address);
    }

private:
    value_type _network_address;
    value_type _broadcast_address;
};

} // IPADDRESS_NAMESPACE

#endif // IPADDRESS_HOSTS_HPP
