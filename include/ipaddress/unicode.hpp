/**
 * @file      unicode.hpp
 * @brief     TODO:
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * TODO:
 */

#ifndef IPADDRESS_UNICODE_HPP
#define IPADDRESS_UNICODE_HPP

#include "errors.hpp"

namespace IPADDRESS_NAMESPACE {

namespace internal {

template <typename T>
struct char_converter {
    static_assert(!std::is_same<T, T>::value, "not supported char type");
};

template <typename T>
struct char_or_throw_converter {
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char(const T*& it, const T* begin, const T* end) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        uint32_t error_symbol = 0;
        auto code = error_code::no_error;
        auto prev_it = it;
        const auto result = char_converter<T>::get_char_or_error(it, end, code, error_symbol);
        if (code != error_code::no_error) {
            raise_error(code, error_symbol, begin, end - begin);
        }
        return result;
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void has_throw() IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS;
};

struct symbol {
    uint32_t value;
    uint32_t length;
};

template <typename T>
struct utf8_converter {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_utf8_char(const T*& it, const T* end, error_code& error, uint32_t& error_symbol) {
        error = error_code::no_error;
        error_symbol = 0;
        auto correct = true;
        auto symbol = utf8_code_point(uint8_t(*it), correct);
        if (correct) {
            switch (symbol.length) {
                case 1:
                    break;
                
                case 2:
                    symbol.value = (symbol.value << 6) | trailing_utf8_code_point(it, end, correct);
                    break;
                    
                case 3:
                    symbol.value = (symbol.value << 6) | trailing_utf8_code_point(it, end, correct);
                    symbol.value = (symbol.value << 6) | trailing_utf8_code_point(it, end, correct);
                    break;
                    
                case 4:
                    symbol.value = (symbol.value << 6) | trailing_utf8_code_point(it, end, correct);
                    symbol.value = (symbol.value << 6) | trailing_utf8_code_point(it, end, correct);
                    symbol.value = (symbol.value << 6) | trailing_utf8_code_point(it, end, correct);
                    break;
                    
                default:
                    correct = false;
                    break;
            }
        }
        if (symbol.value > 127 || !correct) {
            error = error_code::unexpected_symbol;
            error_symbol = symbol.value;
            return '\0';
        }
        return char(symbol.value);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE symbol utf8_code_point(uint8_t byte, bool& correct) IPADDRESS_NOEXCEPT {
        if ((byte & 0b1000'0000) == 0b0000'0000) {
            return {static_cast<uint32_t>(byte), 1};
        }
        if ((byte & 0b1110'0000) == 0b1100'0000) {
            return {static_cast<uint32_t>(byte & 0b0001'1111), 2};
        }
        if ((byte & 0b1111'0000) == 0b1110'0000) { 
            return {static_cast<uint32_t>(byte & 0b0000'1111), 3};
        }
        if ((byte & 0b1111'1000) == 0b1111'0000) { 
            return {static_cast<uint32_t>(byte & 0b0000'0111), 4};
        }
        correct = false;
        return {0, 0};
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t trailing_utf8_code_point(const T*& it, const T* end, bool& correct) IPADDRESS_NOEXCEPT {
        if (++it >= end) {
            correct = false;
            return 0;
        } else if ((uint8_t(*it) & 0b1100'0000) == 0b1000'0000) {
            correct = true;
            return uint8_t(*it) & 0b0011'1111;
        } else {
            correct = false;
            return 0;
        }
    }
};

template <>
struct char_converter<char>
#ifdef IPADDRESS_CHAR_IS_UTF8
: utf8_converter<char>, char_or_throw_converter<char>
#endif
{
#ifdef IPADDRESS_CHAR_IS_UTF8

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char_or_error(const char*& it, const char* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return get_utf8_char(it, end, error, error_symbol);
    }

#else // IPADDRESS_CHAR_IS_UTF8

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char(const char* it, const char* begin, const char* end) IPADDRESS_NOEXCEPT {
        return *it;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char_or_error(const char* it, const char* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        error = error_code::no_error;
        error_symbol = 0;
        return *it;
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void has_throw() IPADDRESS_NOEXCEPT;

#endif
};

#if __cpp_char8_t >= 201811L

template <>
struct char_converter<char8_t> : utf8_converter<char8_t>, char_or_throw_converter<char8_t> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char_or_error(const char8_t*& it, const char8_t* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return get_utf8_char(it, end, error, error_symbol);
    }
};

#endif // __cpp_char8_t

template <>
struct char_converter<char16_t> : char_or_throw_converter<char16_t> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char_or_error(const char16_t*& it, const char16_t* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return utf16_get_char_or_error(it, end, error, error_symbol);
    }

    template <typename T>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char utf16_get_char_or_error(const T*& it, const T* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        error = error_code::no_error;
        error_symbol = 0;
        bool correct = true;
        auto symbol = utf16_code_point(uint16_t(*it));
        switch (symbol.length) {
            case 1:
                symbol.value = uint32_t(*it);
                break;
            
            case 2:
                if ((*++it & 0b1111'1100'0000'0000) == 0b1101'1100'0000'0000) {
                    symbol.value = ((symbol.value << 10) | (uint16_t(*it) & 0b0000'0011'1111'1111)) + 0x10000;
                } else {
                    correct = false;
                }
                break;

            default:
                correct = false;
                break;
        }
        if (symbol.value > 127 || !correct) {
            error = error_code::unexpected_symbol;
            error_symbol = symbol.value;
            return '\0';
        }
        return char(symbol.value);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE symbol utf16_code_point(uint16_t value) IPADDRESS_NOEXCEPT {
        if ((value & 0b1111'1100'0000'0000) == 0b1101'1000'0000'0000) {
            return {static_cast<uint32_t>(value & 0b0000'0011'1111'1111), 2};
        } else {
            return {value, 1};
        }
    }
};

template <>
struct char_converter<char32_t> : char_or_throw_converter<char32_t> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char_or_error(const char32_t*& it, const char32_t* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return utf32_get_char_or_error(it, end, error, error_symbol);
    }

    template <typename T>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char utf32_get_char_or_error(const T*& it, const T* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        error = error_code::no_error;
        error_symbol = 0;
        if (uint32_t(*it) > 127) {
            error = error_code::unexpected_symbol;
            error_symbol = uint32_t(*it);
            return '\0';
        }
        return char(*it);
    }
};

template <>
struct char_converter<wchar_t> : char_or_throw_converter<wchar_t> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char_or_error(const wchar_t*& it, const wchar_t* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        error = error_code::no_error;
        error_symbol = 0;
    #ifdef _WIN32
        if (sizeof(wchar_t) == sizeof(char16_t)) {
            return char_converter<char16_t>::utf16_get_char_or_error(it, end, error, error_symbol);
        }
    #else
        if (sizeof(wchar_t) == sizeof(char32_t)) {
            return char_converter<char32_t>::utf32_get_char_or_error(it, end, error, error_symbol);
        }
    #endif
        if (*it > 127) {
            error = error_code::unexpected_symbol;
            error_symbol = uint32_t(*it);
            return '\0';
        }
        return char(*it);
    }
};

} // IPADDRESS_NAMESPACE::internal

template <typename T>
#ifndef IPADDRESS_NO_EXCEPTIONS 
[[noreturn]] 
#endif
IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void raise_error(error_code code, uint32_t value, const T* address, size_t length) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
    #ifndef IPADDRESS_NO_EXCEPTIONS
        std::ostringstream ss;
        auto stub_code = error_code::no_error;
        uint32_t error_symbol = 0;
        const T* it = address;
        const T* end = it + length;
        while (it < end) {
            const auto result = internal::char_converter<T>::get_char_or_error(it, end, stub_code, error_symbol);
            ++it;
            if (stub_code == error_code::no_error) {
                if (result == '\0') {
                    break;
                }
                ss << result;
            } else {
                if (error_symbol == 0) {
                    break;
                }
                ss << "{U+" << std::setw(4) << std::setfill('0') << std::hex << error_symbol << '}';
            }
        }
        const auto error_str = ss.str();
        raise_error(code, value, error_str.data(), error_str.size()); // unexpected unicode symbol
    #else
        raise_error(code, value, "", 0);
    #endif
}

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_UNICODE_HPP
