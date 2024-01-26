#ifndef IPADDRESS_CONFIG_HPP
#define IPADDRESS_CONFIG_HPP

#include <cstdint>
#include <cstddef>

#include <array>
#include <tuple>
#include <cassert>
#include <sstream>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <stdexcept>

#ifndef IPADDRESS_NAMESPACE
#define IPADDRESS_NAMESPACE ipaddress
#endif

#if defined(_MSVC_LANG)
#  define IPADDRESS_CPLUSPLUS _MSVC_LANG
#else
#  define IPADDRESS_CPLUSPLUS __cplusplus
#endif

#if 201703L < IPADDRESS_CPLUSPLUS
#  define IPADDRESS_CPP_VERSION 20
#elif 201402L < IPADDRESS_CPLUSPLUS
#  define IPADDRESS_CPP_VERSION 17
#elif 201103L < IPADDRESS_CPLUSPLUS
#  define IPADDRESS_CPP_VERSION 14
#elif 199711L < IPADDRESS_CPLUSPLUS
#  define IPADDRESS_CPP_VERSION 11
#else
#  error ipaddress needs at least C++ standard version 11
#endif

#ifdef __cpp_constexpr
#  define IPADDRESS_CONSTEXPR constexpr
#  if __cpp_constexpr >= 201304L
#    define IPADDRESS_CONSTEXPR_14 constexpr
#  else
#    define IPADDRESS_CONSTEXPR_14
#  endif
#  if __cpp_constexpr >= 201603L
#    define IPADDRESS_CONSTEXPR_17 constexpr
#  else
#    define IPADDRESS_CONSTEXPR_17
#  endif
#else
#  define IPADDRESS_CONSTEXPR
#  define IPADDRESS_CONSTEXPR_14
#  define IPADDRESS_CONSTEXPR_17
#endif

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
#  define IPADDRESS_NOEXCEPT
#else
#  define IPADDRESS_NOEXCEPT noexcept
#  ifdef IPADDRESS_NO_EXCEPTIONS
#    define IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS noexcept
#  else
#    define IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS
#  endif
#endif

#if (IPADDRESS_CPP_VERSION >= 17) && !defined(IPADDRESS_NO_NODISCARD)
#  define IPADDRESS_NODISCARD [[nodiscard]]
#  ifdef IPADDRESS_NO_EXCEPTIONS
#    define IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS [[nodiscard]]
#  else
#    define IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS
#  endif
#else
#  define IPADDRESS_NODISCARD
#  define IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS
#endif

#if !defined(IPADDRESS_NO_SPACESHIP_OPERATOR) && defined(__has_include)
#  if (__cpp_lib_three_way_comparison >= 201907) && __has_include(<compare>)
#    define IPADDRESS_HAS_SPACESHIP_OPERATOR
#    include <compare>
#  endif
#endif

#if defined(__cpp_nontype_template_parameter_class)
#  define IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
#elif defined(__cpp_nontype_template_args)
#  if __cpp_nontype_template_args >= 201911L
#    define IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
#  elif __cpp_nontype_template_args >= 201411L && IPADDRESS_CPP_VERSION >= 20
#    if defined(__apple_build_version__)
#      if defined(__clang_major__) && __clang_major__ >= 13
#        define IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
#      endif
#    elif defined(__clang_major__) && __clang_major__ >= 12
#      define IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
#    endif
#  endif
#endif

#if IPADDRESS_CPP_VERSION >= 17
#  include <string_view>
#endif

#endif // IPADDRESS_CONFIG_HPP
