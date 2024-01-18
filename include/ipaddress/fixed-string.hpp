#ifndef IPADDRESS_FIXED_STRING_HPP
#define IPADDRESS_FIXED_STRING_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

struct fixed_string_iterator {
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = char;
    using difference_type   = std::ptrdiff_t;
    using pointer           = const value_type*;
    using reference         = const value_type&;

    IPADDRESS_CONSTEXPR fixed_string_iterator() IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR fixed_string_iterator(const fixed_string_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR fixed_string_iterator(fixed_string_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR_14 fixed_string_iterator& operator=(const fixed_string_iterator&) IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR_14 fixed_string_iterator& operator=(fixed_string_iterator&&) IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR fixed_string_iterator(pointer ptr, std::size_t count) IPADDRESS_NOEXCEPT
        :
        _begin(ptr),
        _end(ptr + count),
        _ptr(ptr) {
    }

    IPADDRESS_CONSTEXPR reference operator*() const IPADDRESS_NOEXCEPT {
        return *_ptr;
    }

    IPADDRESS_CONSTEXPR pointer operator->() const IPADDRESS_NOEXCEPT {
        return _ptr;
    }

    IPADDRESS_CONSTEXPR reference operator[](difference_type n) const IPADDRESS_NOEXCEPT {
        return *(_ptr + n);
    }

    IPADDRESS_CONSTEXPR_14 fixed_string_iterator& operator++() IPADDRESS_NOEXCEPT {
        ++_ptr;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 fixed_string_iterator operator++(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 fixed_string_iterator& operator--() IPADDRESS_NOEXCEPT {
        --_ptr;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 fixed_string_iterator operator--(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    IPADDRESS_CONSTEXPR_14 fixed_string_iterator& operator+=(difference_type n) IPADDRESS_NOEXCEPT {
        _ptr += n;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 fixed_string_iterator& operator-=(difference_type n) IPADDRESS_NOEXCEPT {
        _ptr -= n;
        return *this;
    }

    IPADDRESS_CONSTEXPR fixed_string_iterator operator+(difference_type n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    IPADDRESS_CONSTEXPR fixed_string_iterator operator-(difference_type n) const IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        tmp -= n;
        return tmp;
    }

    IPADDRESS_CONSTEXPR difference_type operator-(const fixed_string_iterator& rhs) const IPADDRESS_NOEXCEPT {
        return _ptr - rhs._ptr;
    }

    IPADDRESS_CONSTEXPR bool operator==(const fixed_string_iterator& rhs) const IPADDRESS_NOEXCEPT {
        return _ptr == rhs._ptr;
    }

    IPADDRESS_CONSTEXPR bool operator!=(const fixed_string_iterator& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR std::strong_ordering operator<=>(const fixed_string_iterator& rhs) const IPADDRESS_NOEXCEPT {
        return _ptr <=> rhs._ptr;
    }
#else
    IPADDRESS_CONSTEXPR bool operator<(const fixed_string_iterator& rhs) const IPADDRESS_NOEXCEPT {
        return _ptr < rhs._ptr;
    }

    IPADDRESS_CONSTEXPR bool operator<=(const fixed_string_iterator& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }

    IPADDRESS_CONSTEXPR bool operator>(const fixed_string_iterator& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }

    IPADDRESS_CONSTEXPR bool operator>=(const fixed_string_iterator& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }
#endif

    friend IPADDRESS_CONSTEXPR fixed_string_iterator operator+(difference_type n, const fixed_string_iterator& it) IPADDRESS_NOEXCEPT {
        auto tmp = it;
        tmp += n;
        return tmp;
    }

private:
    pointer _begin = nullptr;
    pointer _end = nullptr;
    pointer _ptr = nullptr;
};

template <size_t N>
struct fixed_string {
    using const_pointer          = const char*;
    using const_reference        = const char&;
    using const_iterator         = fixed_string_iterator;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static IPADDRESS_CONSTEXPR size_t length = N;

    char _data[N] = {};

    IPADDRESS_CONSTEXPR fixed_string(const char (&data)[N + 1]) IPADDRESS_NOEXCEPT {
        for (size_t i = 0; i < N; ++i) {
            _data[i] = data[i];
        }
    }

    IPADDRESS_CONSTEXPR const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data, length);
    }
    
    IPADDRESS_CONSTEXPR const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data, length) + length;
    }
    
    IPADDRESS_CONSTEXPR_17 const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    IPADDRESS_CONSTEXPR_17 const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }
    
    IPADDRESS_CONSTEXPR const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    IPADDRESS_CONSTEXPR const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    IPADDRESS_CONSTEXPR_17 const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    IPADDRESS_CONSTEXPR_17 const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool empty() const IPADDRESS_NOEXCEPT {
        return size() == 0;
    }
    
    IPADDRESS_CONSTEXPR size_t size() const IPADDRESS_NOEXCEPT {
        return length;
    }

    IPADDRESS_CONSTEXPR const_reference operator[](size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return at(n);
    }

    IPADDRESS_CONSTEXPR const_reference at(size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (n >= 0 && n < N) {
            return _data[n];
        }
    #ifdef IPADDRESS_NO_EXCEPTIONS
        return *_data;
    #else
        throw std::out_of_range("index out of array");
    #endif
    }

    IPADDRESS_CONSTEXPR const_reference front() const IPADDRESS_NOEXCEPT {
        return _data[0];
    }

    IPADDRESS_CONSTEXPR const_reference back() const IPADDRESS_NOEXCEPT {
        return _data[N - 1];
    }

    IPADDRESS_CONSTEXPR const_pointer data() const IPADDRESS_NOEXCEPT {
        return _data;
    }
};

template <>
struct fixed_string<0> {
    using const_pointer          = const char*;
    using const_reference        = const char&;
    using const_iterator         = fixed_string_iterator;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static IPADDRESS_CONSTEXPR size_t length = 0;

    IPADDRESS_CONSTEXPR fixed_string() IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR fixed_string(const char*) IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data, length);
    }
    
    IPADDRESS_CONSTEXPR const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data, length) + length;
    }
    
    IPADDRESS_CONSTEXPR_17 const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    IPADDRESS_CONSTEXPR_17 const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }
    
    IPADDRESS_CONSTEXPR const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    IPADDRESS_CONSTEXPR const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    IPADDRESS_CONSTEXPR_17 const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    IPADDRESS_CONSTEXPR_17 const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool empty() const IPADDRESS_NOEXCEPT {
        return true;
    }
    
    IPADDRESS_CONSTEXPR size_t size() const IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_CONSTEXPR const_reference operator[](size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return at(n);
    }

    IPADDRESS_CONSTEXPR const_reference at(size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return front();
    }

    IPADDRESS_CONSTEXPR const_reference front() const IPADDRESS_NOEXCEPT {
        return *cbegin();
    }

    IPADDRESS_CONSTEXPR const_reference back() const IPADDRESS_NOEXCEPT {
        return *cbegin();
    }

    IPADDRESS_CONSTEXPR const_pointer data() const IPADDRESS_NOEXCEPT {
        return _data;
    }

private:
    static IPADDRESS_CONSTEXPR char _data[1] = {};
};

template <size_t N>
IPADDRESS_CONSTEXPR fixed_string<N - 1> make_fixed_string(const char(&data)[N]) IPADDRESS_NOEXCEPT {
    return fixed_string<N - 1>(data);
}

#if IPADDRESS_CPP_VERSION >= 17
    template <size_t N> fixed_string(fixed_string<N>) -> fixed_string<N>;
    template <size_t N> fixed_string(const char(&)[N]) -> fixed_string<N - 1>;
    fixed_string() -> fixed_string<0>;
#endif

}

#endif
