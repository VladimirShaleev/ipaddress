/**
 * \file      endian.hpp
 * \brief     Defines macros to determine byte order
 * \author    Vladimir Shaleev
 * \copyright MIT License
 * 
 * This header file contains macros and utility functions to determine
 * and work with the byte order (endianness) of the system. It provides
 * a set of preprocessor directives that establish the byte order based
 * on various conditions, such as user definitions, C++20 standard features,
 * or platform-specific macros.
 * 
 * The file defines `IPADDRESS_LITTLE_ENDIAN` and `IPADDRESS_BIG_ENDIAN`
 * as constants for comparison and sets `IPADDRESS_ENDIAN` accordingly.
 * It also includes functions to check the system's endianness and to
 * swap bytes in an integer, which are useful for network programming
 * and handling data with different endianness.
 * 
 * The determination of endianness is done with a priority order:
 * 1. User-defined `IPADDRESS_ENDIAN`.
 * 2. C++20's `std::endian`.
 * 3. Platform/compiler-provided macros.
 * 
 * If the endianness cannot be determined, a compilation error is raised
 * prompting the user to define `IPADDRESS_ENDIAN`.
 */

#ifndef IPADDRESS_ENDIAN_HPP
#define IPADDRESS_ENDIAN_HPP

#include "config.hpp"

#if (IPADDRESS_CPP_VERSION >= 20) && defined(__has_include)
#  if __has_include(<bit>)
#    define IPADDRESS_HAS_STD_ENDIAN
#    include <bit>
#  endif
#endif

#define IPADDRESS_LITTLE_ENDIAN 0
#define IPADDRESS_BIG_ENDIAN 1

#ifndef IPADDRESS_ENDIAN
//  GCC 4.6
#  ifdef __BYTE_ORDER__
#    if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#      define IPADDRESS_ENDIAN IPADDRESS_LITTLE_ENDIAN
#    elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#      define IPADDRESS_ENDIAN IPADDRESS_BIG_ENDIAN
#    else
#      error Unknown endianness detected. Needs to define IPADDRESS_ENDIAN
#    endif
// GLIBC
#  elif defined(__GLIBC__)
#    include <endian.h>
#    if (__BYTE_ORDER == __LITTLE_ENDIAN)
#      define IPADDRESS_ENDIAN IPADDRESS_LITTLE_ENDIAN
#    elif (__BYTE_ORDER == __BIG_ENDIAN)
#      define IPADDRESS_ENDIAN IPADDRESS_BIG_ENDIAN
#    else
#      error Unknown endianness detected. Needs to define IPADDRESS_ENDIAN
#   endif 
// _LITTLE_ENDIAN and _BIG_ENDIAN
#  elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
#    define IPADDRESS_ENDIAN IPADDRESS_LITTLE_ENDIAN
#  elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)
#    define IPADDRESS_ENDIAN IPADDRESS_BIG_ENDIAN
// For architecture
#  elif defined(__hppa) || \
        defined(__hpux) || \
        defined(__powerpc__) || \
        defined(__ppc__) || \
        defined(__s390__)
        defined(__sparc__) || \
        defined(__sparc) || \
        defined(_MIPSEB) || \
        defined(_POWER)
#    define IPADDRESS_ENDIAN IPADDRESS_BIG_ENDIAN
#  elif defined(__alpha__) || \
        defined(__amd64__) || \
        defined(__amd64) || \
        defined(__bfin__) || \
        defined(__i386__) || \
        defined(__ia64__) || \
        defined(__ia64) || \
        defined(__x86_64__) || \
        defined(__x86_64) || \
        defined(_M_ALPHA) || \
        defined(_M_AMD64) || \
        defined(_M_IA64) || \
        defined(_M_IX86) || \
        defined(_M_X64)
#    define IPADDRESS_ENDIAN IPADDRESS_LITTLE_ENDIAN
#  elif defined(_MSC_VER) && (defined(_M_ARM) || defined(_M_ARM64))
#    define IPADDRESS_ENDIAN IPADDRESS_LITTLE_ENDIAN
#  else
#    error Unknown endianness detected. Needs to define IPADDRESS_ENDIAN
#  endif
#endif

namespace IPADDRESS_NAMESPACE {

/**
 * Checks if the system is little-endian.
 * 
 * @return `true` if the system is little-endian, `false` otherwise.
 */
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_little_endian() IPADDRESS_NOEXCEPT {
#if defined(IPADDRESS_HAS_STD_ENDIAN)
    return std::endian::native == std::endian::little;
#elif defined(IPADDRESS_ENDIAN)
    return !IPADDRESS_ENDIAN;
#else
#   error Unknown endianness detected. Needs to define IPADDRESS_ENDIAN
#endif
}

/**
 * Swaps the bytes of a 32-bit unsigned integer.
 * 
 * @param[in] value the integer value to swap bytes of
 * @return    The value with its bytes swapped.
 */
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t swap_bytes(uint32_t value) IPADDRESS_NOEXCEPT {
    value = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0x00FF00FF);
    value = (value << 16) | (value >> 16);
    return value;
}

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_ENDIAN_HPP
