#ifndef IPADDRESS_IP_ADDRESS_ITERATOR_HPP
#define IPADDRESS_IP_ADDRESS_ITERATOR_HPP

#include "config.hpp"
#include "ipv4-address.hpp"
#include "ipv6-address.hpp"

namespace IPADDRESS_NAMESPACE {

template <typename Iterator>
class ip_reverse_iterator {
public:
#ifdef IPADDRESS_HAS_CONCEPTS
    using iterator_concept  = std::random_access_iterator_tag;
#endif
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    using iterator_type	    = Iterator;
    using value_type        = typename std::iterator_traits<Iterator>::value_type;
    using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
    using pointer           = typename std::iterator_traits<Iterator>::pointer;
    using reference         = typename std::iterator_traits<Iterator>::reference;

    using uint_type         = typename Iterator::uint_type;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator() IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator(const ip_reverse_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator(ip_reverse_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator=(const ip_reverse_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator=(ip_reverse_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_reverse_iterator(Iterator it) IPADDRESS_NOEXCEPT
        : _it(it.reverse()) {
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint_type uint_diff(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return other._it - _it;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator_type base() const IPADDRESS_NOEXCEPT {
        return _it.base();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator*() const IPADDRESS_NOEXCEPT {
        return *_it;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer operator->() const IPADDRESS_NOEXCEPT {
        return &*_it;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator[](difference_type n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return _it - n;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator[](const uint_type& n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return _it - n;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator++() IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        --_it;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_reverse_iterator operator++(int) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator--() IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        ++_it;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_reverse_iterator operator--(int) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator+=(difference_type n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        _it -= n;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator+=(const uint_type& n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        _it -= n;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator-=(difference_type n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        _it += n;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_reverse_iterator& operator-=(const uint_type& n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        _it += n;
        return *this;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator+(difference_type n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator+(const uint_type& n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    friend IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator+(difference_type n, const ip_reverse_iterator& it) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return it - n;
    }

    friend IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator+(const uint_type& n, const ip_reverse_iterator& it) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return it - n;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator-(difference_type n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_reverse_iterator operator-(const uint_type& n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type operator-(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return difference_type(other._it - _it);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return  other._it == _it;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this == other);
    }

#ifdef GALAXY_HPP_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return _current <=> other._current;
    }

#else // !GALAXY_HPP_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return other._it < _it;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(other < *this);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return other < *this;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_reverse_iterator& other) const IPADDRESS_NOEXCEPT {
        return !(*this < other);
    }

#endif // !GALAXY_HPP_HAS_SPACESHIP_OPERATOR

private:
    Iterator _it;
};

template <typename>
class ip_address_iterator;

template <typename Base>
class ip_address_iterator<ip_address_base<Base>> {
public:
#ifdef IPADDRESS_HAS_CONCEPTS
    using iterator_concept  = std::random_access_iterator_tag;
#endif
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = ip_address_base<Base>;
    using difference_type   = std::int64_t;
    using pointer           = const value_type*;
    using reference         = const value_type&;

    using uint_type         = typename value_type::uint_type;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator() IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator(const ip_address_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator(ip_address_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator& operator=(const ip_address_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator& operator=(ip_address_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator(reference begin, reference end, reference ref) IPADDRESS_NOEXCEPT
        : _current(ref), _offset(ref.to_uint()), _begin(begin.to_uint()), _end(end.to_uint()) {
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint_type uint_diff(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return _offset - other._offset;
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
        const auto offset = _offset + n;
        if (offset < _end) {
            _at = value_type::from_uint(offset);
            return _at;
        }
    #ifndef IPADDRESS_NO_EXCEPTIONS
        throw std::out_of_range("index out of range");
    #endif
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator& operator++() IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        add(1);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator operator++(int) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator& operator--() IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        sub(1);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator operator--(int) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator& operator+=(difference_type n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        add(n);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator& operator+=(const uint_type& n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        add(n);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator& operator-=(difference_type n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        sub(n);
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE ip_address_iterator& operator-=(const uint_type& n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        sub(n);
        return *this;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator+(difference_type n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator+(const uint_type& n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    friend IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator+(difference_type n, const ip_address_iterator& it) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return it + n;
    }

    friend IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator+(const uint_type& n, const ip_address_iterator& it) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return it + n;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator-(difference_type n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator operator-(const uint_type& n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type operator-(const ip_address_iterator& other) const IPADDRESS_NOEXCEPT {
        return difference_type(_offset - other._offset);
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
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator base() const IPADDRESS_NOEXCEPT {
        auto result = *this;
        ++result._offset;
        ++result._begin;
        ++result._end;
        result._current = value_type::from_uint(result._offset);
        return result;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_iterator reverse() const IPADDRESS_NOEXCEPT {
        auto result = *this;
        --result._offset;
        --result._begin;
        --result._end;
        result._current = value_type::from_uint(result._offset);
        return result;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void add(const uint_type& n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_offset != 0) {
            _offset += n;
            if (_offset > _end) {
            #ifdef IPADDRESS_NO_EXCEPTIONS
                _offset = _end;
            #else
                throw std::out_of_range("index out of range");
            #endif
            }
            _current = value_type::from_uint(_offset);
        }
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void sub(const uint_type& n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_offset != 0) {
            const auto value = n;
            if (_offset >= value) {
                _offset -= value;      
                if (_offset < _begin) {
                #ifdef IPADDRESS_NO_EXCEPTIONS
                    _offset = _begin;
                #else
                    throw std::out_of_range("index out of range");
                #endif
                }
            } else {
            #ifdef IPADDRESS_NO_EXCEPTIONS
                _offset = _begin;
            #else
                throw std::out_of_range("index out of range");
            #endif
            }
            _current = value_type::from_uint(_offset);
        }
    }

    template <typename>
    friend class ip_reverse_iterator;

    template <typename>
    friend class ip_network_iterator;

    value_type _current{};
    value_type _at{};
    uint_type _offset{};
    uint_type _begin{};
    uint_type _end{};
};

template <typename>
class hosts_sequence;

template <typename Base>
class hosts_sequence<ip_address_base<Base>> {
public:
    using value_type      = ip_address_base<Base>;
    using size_type       = std::size_t;
    using difference_type = typename value_type::uint_type;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator       = ip_address_iterator<value_type>;
    using const_iterator = ip_address_iterator<value_type>;

    using reverse_iterator       = ip_reverse_iterator<iterator>;
    using const_reverse_iterator = ip_reverse_iterator<const_iterator>;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_sequence(const hosts_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_sequence(hosts_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE hosts_sequence& operator=(const hosts_sequence&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE hosts_sequence& operator=(hosts_sequence&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_sequence(const_reference network_address, const_reference broadcast_address, size_t prefixlen, size_t max_prefixlen) IPADDRESS_NOEXCEPT  {
        if (std::is_same<value_type, ipv4_address>::value && prefixlen == max_prefixlen - 1) {
            const auto begin = value_type::from_uint(network_address.to_uint());
            const auto end = value_type::from_uint(broadcast_address.to_uint() + 1);
            _begin = const_iterator(begin, end, begin);
            _end = const_iterator(begin, end, end);
        } else if (std::is_same<value_type, ipv4_address>::value && prefixlen == max_prefixlen) {
            const auto begin = value_type::from_uint(network_address.to_uint());
            const auto end = value_type::from_uint(network_address.to_uint() + 1);
            _begin = const_iterator(begin, end, begin);
            _end = const_iterator(begin, end, end);
        } else {
            const auto begin = value_type::from_uint(network_address.to_uint() + 1);
            const auto end = value_type::from_uint(broadcast_address.to_uint() + (std::is_same<value_type, ipv6_address>::value ? 1 : 0));
            _begin = const_iterator(begin, end, begin);
            _end = const_iterator(begin, end, end);
        }
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
        return *(_end - 1U);
    }

private:
    const_iterator _begin{};
    const_iterator _end{};
};

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_HOSTS_HPP
