#ifndef IPADDRESS_BYTE_ARRAY_HPP
#define IPADDRESS_BYTE_ARRAY_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

template <std::size_t N>
struct byte_array {
    using value_type      = uint8_t;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator       = pointer;
    using const_iterator = const_pointer;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    value_type _data[N];

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data) + N;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool empty() const IPADDRESS_NOEXCEPT {
        return false;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t size() const IPADDRESS_NOEXCEPT {
        return N;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t max_size() const IPADDRESS_NOEXCEPT {
        return N;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR reference operator[](size_t n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return at(n);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR const_reference operator[](size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return at(n);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR reference at(size_t n) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (n >= 0 && n < N) {
            return _data[n];
        }
    #ifdef IPADDRESS_NO_EXCEPTIONS
        return *_data;
    #else
        throw std::out_of_range("index out of array");
    #endif
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR const_reference at(size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (n >= 0 && n < N) {
            return _data[n];
        }
    #ifdef IPADDRESS_NO_EXCEPTIONS
        return *_data;
    #else
        throw std::out_of_range("index out of array");
    #endif
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR reference front() IPADDRESS_NOEXCEPT {
        return _data[0];
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_reference front() const IPADDRESS_NOEXCEPT {
        return _data[0];
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR reference back() IPADDRESS_NOEXCEPT {
        return _data[N - 1];
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_reference back() const IPADDRESS_NOEXCEPT {
        return _data[N - 1];
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR pointer data() IPADDRESS_NOEXCEPT {
        return _data;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_pointer data() const IPADDRESS_NOEXCEPT {
        return _data;
    }

    IPADDRESS_CONSTEXPR void swap(byte_array& other) IPADDRESS_NOEXCEPT {
        for (size_t i = 0; i < N; ++i) {
            const auto tmp = _data[i];
            _data[i] = other._data[i];
            other._data[i] = tmp;
        }
    }
};

template <>
class byte_array<0> {
public:
    using value_type      = uint8_t;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator       = pointer;
    using const_iterator = const_pointer;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(data());
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(data());
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_17 const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool empty() const IPADDRESS_NOEXCEPT {
        return true;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t size() const IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t max_size() const IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR reference operator[](size_t n) IPADDRESS_NOEXCEPT {
        return at(n);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_reference operator[](size_t n) const IPADDRESS_NOEXCEPT {
        return at(n);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR reference at(size_t n) IPADDRESS_NOEXCEPT {
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_reference at(size_t n) const IPADDRESS_NOEXCEPT {
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR reference front() IPADDRESS_NOEXCEPT {
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_reference front() const IPADDRESS_NOEXCEPT {
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR reference back() IPADDRESS_NOEXCEPT {
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_reference back() const IPADDRESS_NOEXCEPT {
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR pointer data() IPADDRESS_NOEXCEPT {
        return nullptr;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_pointer data() const IPADDRESS_NOEXCEPT {
        return nullptr;
    }

    IPADDRESS_CONSTEXPR void swap(byte_array& other) IPADDRESS_NOEXCEPT {
    }
};

template <std::size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator==(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
    for (std::size_t i = 0; i < N; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

template <std::size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator!=(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
    return !(lhs == rhs);
}

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR
template <std::size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR std::strong_ordering operator<=>(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
    for (std::size_t i = 0; i < N; ++i) {
        if (const auto result = lhs[i] <=> rhs[i]; result != std::strong_ordering::equivalent) {
            return result;
        }
    }
    return std::strong_ordering::equivalent;
}
#else
template <std::size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
    for (std::size_t i = 0; i < N; ++i) {
        if (lhs._data[i] < rhs._data[i]) {
            return true;
        }
    }
    return false;
}

template <std::size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
    return rhs < lhs;
}

template <std::size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<=(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
    return !(rhs < lhs);
}

template <std::size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>=(const byte_array<N>& lhs, const byte_array<N>& rhs) IPADDRESS_NOEXCEPT {
    return !(lhs < rhs);
}
#endif

template <std::size_t N>
using byte_array_type = 
#if IPADDRESS_CPP_VERSION >= 20
    std::array<uint8_t, N>;
#else
    byte_array<N>;
#endif

}

#endif
