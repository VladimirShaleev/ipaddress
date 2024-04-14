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
            raise_error(code, error_symbol, begin, prev_it - begin);
        }
        return result;
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void has_throw() IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS;
};

struct symbol {
    uint32_t value;
    uint32_t length;
};

struct utf8_converter {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_utf8_char(const char*& it, const char* end, error_code& error, uint32_t& error_symbol) {
        auto correct = true;
        auto symbol = utf8_code_point(*it, correct);
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

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE symbol utf8_code_point(char byte, bool& correct) IPADDRESS_NOEXCEPT {
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

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t trailing_utf8_code_point(const char*& it, const char* end, bool& correct) IPADDRESS_NOEXCEPT {
        if (++it >= end) {
            correct = false;
            return 0;
        } else if ((*it & 0b1100'0000) == 0b1000'0000) {
            correct = true;
            return *it & 0b0011'1111;
        } else {
            correct = false;
            return 0;
        }
    }
};

template <>
struct char_converter<char>
#ifdef IPADDRESS_CHAR_IS_UTF8
: utf8_converter, char_or_throw_converter<char>
#endif
{
#ifdef IPADDRESS_CHAR_IS_UTF8

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char_or_error(const char*& it, const char* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return get_utf8_char((const uint8_t*&) it, (const uint8_t*) end, error, error_symbol);
    }

#else // IPADDRESS_CHAR_IS_UTF8

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char(const char* it, const char* begin, const char* end) IPADDRESS_NOEXCEPT {
        return *it;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char_or_error(const char* it, const char* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return *it;
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void has_throw() IPADDRESS_NOEXCEPT;

#endif
};

template <>
struct char_converter<char8_t> : utf8_converter, char_or_throw_converter<char8_t> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char_or_error(const char8_t*& it, const char8_t* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return get_utf8_char((const char*&) it, (const char*) end, error, error_symbol);
    }
};

template <>
struct char_converter<char16_t> : char_or_throw_converter<char16_t> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char_or_error(const char16_t*& it, const char16_t* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return char(*it); // TODO: add implementation
    }
};

template <>
struct char_converter<char32_t> : char_or_throw_converter<char32_t> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char_or_error(const char32_t*& it, const char32_t* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return char(*it); // TODO: add implementation
    }
};

template <>
struct char_converter<wchar_t> : char_or_throw_converter<wchar_t> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char get_char_or_error(const wchar_t*& it, const wchar_t* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return char(*it); // TODO: add implementation
    }
};

} // IPADDRESS_NAMESPACE::internal

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_UNICODE_HPP
