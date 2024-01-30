#ifndef IPADDRESS_FIXED_STRING_HPP
#define IPADDRESS_FIXED_STRING_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

template <size_t N>
struct fixed_string {
    using const_pointer          = const char*;
    using const_reference        = const char&;
    using const_iterator         = const_pointer;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static IPADDRESS_CONSTEXPR size_t max_length = N;

    size_t length = 0;

    char _data[N] = {};

    IPADDRESS_CONSTEXPR fixed_string(const char (&data)[N + 1]) IPADDRESS_NOEXCEPT {
        auto ended = false;
        for (size_t i = 0; i < N; ++i) {
            _data[i] = data[i];
            if (data[i] == '\0') {
                ended = true;
            }
            if (!ended) {
                ++length;
            }
        }
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data) + length;
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
        return size() == 0;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t size() const IPADDRESS_NOEXCEPT {
        return length;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t capacity() const IPADDRESS_NOEXCEPT {
        return max_length;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR const_reference operator[](size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return at(n);
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

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_reference front() const IPADDRESS_NOEXCEPT {
        return _data[0];
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_reference back() const IPADDRESS_NOEXCEPT {
        return _data[N - 1];
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_pointer data() const IPADDRESS_NOEXCEPT {
        return _data;
    }
    
    template <size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR int compare(const fixed_string<N2>& rhs) const IPADDRESS_NOEXCEPT {
        size_t i = 0;
        for (; i < size() && i < rhs.size(); ++i) {
            const auto c1 = at(i);
            const auto c2 = rhs.at(i);
            if (c1 != c2) {
                return int(c1) - int(c2);
            }
        }

        if (i == size() && i == rhs.size()) {
            return 0;
        } else if (i == size() && i < rhs.size()) {
            return -1;
        } else {
            return 1;
        }
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t hash() const IPADDRESS_NOEXCEPT {
        size_t value{};
        size_t prime{};

        if (sizeof(size_t) == 8) {
            value = 14695981039346656037ULL;
            prime = 1099511628211ULL;
        } else {
            value = 2166136261U;
            prime = 16777619U;
        }

        for (size_t i = 0; i < size(); ++i) {
            value ^= static_cast<size_t>(_data[i]);
            value *= prime;
        }
        return value;
    }

    IPADDRESS_CONSTEXPR void swap(fixed_string& other) IPADDRESS_NOEXCEPT {
        auto count = size() < other.size() ? other.size() : size();
        for (size_t i = 0; i < count; ++i) {
            std::swap(_data[i], other._data[i]);
        }
        std::swap(length, other.length);
    }
}; // fixed_string<N>

template <>
struct fixed_string<0> {
    using const_pointer          = const char*;
    using const_reference        = const char&;
    using const_iterator         = const_pointer;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static IPADDRESS_CONSTEXPR size_t max_length = 0;

    static IPADDRESS_CONSTEXPR size_t length = 0;

    static IPADDRESS_CONSTEXPR char _data[1] = {};

    IPADDRESS_CONSTEXPR fixed_string() IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR fixed_string(const char*) IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data) + length;
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

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t capacity() const IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR const_reference operator[](size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return at(n);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR const_reference at(size_t n) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return front();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_reference front() const IPADDRESS_NOEXCEPT {
        return *cbegin();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_reference back() const IPADDRESS_NOEXCEPT {
        return *cbegin();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const_pointer data() const IPADDRESS_NOEXCEPT {
        return _data;
    }

    template <size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR int compare(const fixed_string<N2>& rhs) const IPADDRESS_NOEXCEPT {
        return rhs.empty() ? 0 : -1;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t hash() const IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_CONSTEXPR void swap(fixed_string& other) IPADDRESS_NOEXCEPT {
    }
}; // fixed_string<0>

template <size_t N1, size_t N2>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator==(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
    return lhs.compare(rhs) == 0;
}

template <size_t N1, size_t N2>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator!=(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
    return !(lhs == rhs);
}

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR std::strong_ordering operator<=>(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        if (const auto result = lhs.compare(rhs); result == 0) {
            return std::strong_ordering::equivalent;
        } else if (result < 0) {
            return std::strong_ordering::less;
        } else {
            return std::strong_ordering::greater;
        }
    }
    
#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        return lhs.compare(rhs) < 0;
    }

    template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        return rhs < lhs;
    }

    template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<=(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        return !(rhs < lhs);
    }

    template <size_t N1, size_t N2>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>=(const fixed_string<N1>& lhs, const fixed_string<N2>& rhs) IPADDRESS_NOEXCEPT {
        return !(lhs < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

template <size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR fixed_string<N - 1> make_fixed_string(const char(&data)[N]) IPADDRESS_NOEXCEPT {
    return fixed_string<N - 1>(data);
}

#if IPADDRESS_CPP_VERSION >= 17
    template <size_t N> fixed_string(fixed_string<N>) -> fixed_string<N>;
    template <size_t N> fixed_string(const char(&)[N]) -> fixed_string<N - 1>;
    fixed_string() -> fixed_string<0>;
#endif

} // IPADDRESS_NAMESPACE

#endif // IPADDRESS_FIXED_STRING_HPP
