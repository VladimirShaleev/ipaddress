/**
 * @file      unicode.hpp
 * @brief     Unicode character processing
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This file contains definitions and templates for working with various
 * Unicode encodings such as UTF-8, UTF-16, UTF-32, and Wide chars.
 * It provides functionality to convert unicode characters to char and 
 * handling errors associated with invalid Unicode characters.
 */

#ifndef IPADDRESS_UNICODE_HPP
#define IPADDRESS_UNICODE_HPP

#include "errors.hpp"

namespace IPADDRESS_NAMESPACE {

namespace internal {

template <typename T>
struct char_reader;

template <typename T>
struct char_or_throw {
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char next(const T*& it, const T* begin, const T* end) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        uint32_t error_symbol = 0;
        auto code = error_code::no_error;
        auto prev_it = it;
        const auto result = char_reader<T>::next_or_error(it, end, code, error_symbol);
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
struct utf8_reader {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char utf8_next_or_error(const T*& it, const T* end, error_code& error, uint32_t& error_symbol) {
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
        ++it;
        if (!correct) {
            error = error_code::wrong_encoding_sequence;
            return '\0';
        } else if (symbol.value > 127) {
            error = error_code::unexpected_symbol;
            error_symbol = symbol.value;
            return '\0';
        }
        return char(symbol.value);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE symbol utf8_code_point(uint8_t byte, bool& correct) IPADDRESS_NOEXCEPT {
        if ((byte & 0b10000000) == 0b00000000) {
            return {static_cast<uint32_t>(byte), 1};
        }
        if ((byte & 0b11100000) == 0b11000000) {
            return {static_cast<uint32_t>(byte & 0b00011111), 2};
        }
        if ((byte & 0b11110000) == 0b11100000) { 
            return {static_cast<uint32_t>(byte & 0b00001111), 3};
        }
        if ((byte & 0b11111000) == 0b11110000) { 
            return {static_cast<uint32_t>(byte & 0b00000111), 4};
        }
        correct = false;
        return {0, 0};
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t trailing_utf8_code_point(const T*& it, const T* end, bool& correct) IPADDRESS_NOEXCEPT {
        if (++it >= end) {
            correct = false;
            return 0;
        } 
        if ((uint8_t(*it) & 0b11000000) == 0b10000000) {
            correct = true;
            return uint8_t(*it) & 0b00111111;
        } 
        correct = false;
        return 0;
    }
};

template <typename T>
struct utf16_reader {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char utf16_next_or_error(const T*& it, const T* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        error = error_code::no_error;
        error_symbol = 0;
        bool correct = true;
        auto symbol = utf16_code_point(uint16_t(*it));
        switch (symbol.length) {
            case 1:
                symbol.value = uint32_t(*it);
                break;
            
            case 2:
                if (++it >= end) {
                    correct = false;
                    break;
                } 
                if ((*it & 0b1111110000000000) == 0b1101110000000000) {
                    symbol.value = ((symbol.value << 10) | (uint16_t(*it) & 0b0000001111111111)) + 0x10000;
                } else {
                    correct = false;
                }
                break;

            default:
                correct = false;
                break;
        }
        ++it;
        if (!correct) {
            error = error_code::wrong_encoding_sequence;
            return '\0';
        } else if (symbol.value > 127) {
            error = error_code::unexpected_symbol;
            error_symbol = symbol.value;
            return '\0';
        }
        return char(symbol.value);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE symbol utf16_code_point(uint16_t value) IPADDRESS_NOEXCEPT {
        if ((value & 0b1111110000000000) == 0b1101100000000000) {
            return {static_cast<uint32_t>(value & 0b0000001111111111), 2};
        } else {
            return {value, 1};
        }
    }
};

template <typename T>
struct utf32_reader {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char utf32_next_or_error(const T*& it, const T* /*end*/, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        error = error_code::no_error;
        error_symbol = 0;
        if (uint32_t(*it) > 127) {
            error = error_code::unexpected_symbol;
            error_symbol = uint32_t(*it++);
            return '\0';
        }
        return char(*it++);
    }
};

template <>
struct char_reader<char>
#ifdef IPADDRESS_CHAR_IS_UTF8
: utf8_reader<char>, char_or_throw<char>
#endif
{
#ifdef IPADDRESS_CHAR_IS_UTF8

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char next_or_error(const char*& it, const char* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return utf8_next_or_error(it, end, error, error_symbol);
    }

#else // !IPADDRESS_CHAR_IS_UTF8

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char next(const char*& it, const char* /*begin*/, const char* /*end*/) IPADDRESS_NOEXCEPT {
        return *it++;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char next_or_error(const char*& it, const char* /*end*/, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        error = error_code::no_error;
        error_symbol = 0;
        return *it++;
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void has_throw() IPADDRESS_NOEXCEPT;

#endif // !IPADDRESS_CHAR_IS_UTF8
};

#if __cpp_char8_t >= 201811L

template <>
struct char_reader<char8_t> : utf8_reader<char8_t>, char_or_throw<char8_t> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char next_or_error(const char8_t*& it, const char8_t* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return utf8_next_or_error(it, end, error, error_symbol);
    }
};

#endif // __cpp_char8_t

template <>
struct char_reader<char16_t> : utf16_reader<char16_t>, char_or_throw<char16_t> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char next_or_error(const char16_t*& it, const char16_t* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return utf16_next_or_error(it, end, error, error_symbol);
    }
};

template <>
struct char_reader<char32_t> : utf32_reader<char32_t>, char_or_throw<char32_t> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char next_or_error(const char32_t*& it, const char32_t* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        return utf32_next_or_error(it, end, error, error_symbol);
    }
};

template <>
struct char_reader<wchar_t> : utf16_reader<wchar_t>, utf32_reader<wchar_t>, char_or_throw<wchar_t> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE char next_or_error(const wchar_t*& it, const wchar_t* end, error_code& error, uint32_t& error_symbol) IPADDRESS_NOEXCEPT {
        error = error_code::no_error;
        error_symbol = 0;
    #ifdef _WIN32
        if (sizeof(wchar_t) == sizeof(char16_t)) {
            return utf16_next_or_error(it, end, error, error_symbol);
        }
    #else
        if (sizeof(wchar_t) == sizeof(char32_t)) {
            return utf32_next_or_error(it, end, error, error_symbol);
        }
    #endif
        if (*it > 127) {
            error = error_code::unexpected_symbol;
            error_symbol = uint32_t(*it++);
            return '\0';
        }
        return char(*it++);
    }
};

IPADDRESS_FORCE_INLINE void print_symbol_code(std::ostringstream& out, uint32_t symbol) {
    out << "{U+" << std::setw(4) << std::setfill('0') << std::hex << symbol << '}';
}

} // IPADDRESS_NAMESPACE::internal

IPADDRESS_FORCE_INLINE std::ostringstream& error::print(std::ostringstream& out, const symbol& arg) {
    internal::print_symbol_code(out, arg.value);
    return out;
}

template <typename T, size_t N>
IPADDRESS_FORCE_INLINE std::ostringstream& error::print(std::ostringstream& out, const T (&str)[N]) {
    auto code = error_code::no_error;
    uint32_t error_symbol = 0;
    const T* it = str;
    const T* end = str + N;
    while (it < end) {
        const auto result = internal::char_reader<T>::next_or_error(it, end, code, error_symbol);
        if (code == error_code::no_error) {
            if (result == '\0') {
                break;
            }
            out << result;
        } else {
            if (error_symbol == 0) {
                break;
            }
            internal::print_symbol_code(out, error_symbol);
        }
    }
    return out;
}

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_UNICODE_HPP
