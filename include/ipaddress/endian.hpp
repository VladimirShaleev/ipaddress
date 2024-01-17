#ifndef IPADDRESS_ENDIAN_HPP
#define IPADDRESS_ENDIAN_HPP

#include "config.hpp"

#ifdef __has_include
#  if __has_include(<bit>)
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

constexpr bool is_little_endian() IPADDRESS_NOEXCEPT {
#if IPADDRESS_CPP_VERSION >= 20
    return std::endian::native == std::endian::little;
#elif defined(IPADDRESS_ENDIAN)
    return !IPADDRESS_ENDIAN;
#else
#   error Unknown endianness detected. Needs to define IPADDRESS_ENDIAN
#endif
}

constexpr uint32_t swap_bytes(uint32_t value) IPADDRESS_NOEXCEPT {
    value = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0x00FF00FF);
    value = (value << 16) | (value >> 16);
    return value;
}

}

#endif
