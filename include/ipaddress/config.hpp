/**
 * @file      config.hpp
 * @brief     Defines macros for library configuration
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This header file contains a collection of preprocessor macros that are essential
 * for configuring the library to work across various platforms and compilers.
 * It also ensures compatibility with different versions of the C++ language standard.
 * The macros facilitate feature detection and conditional compilation, which are
 * crucial for maintaining cross-platform and cross-compiler support.
 * 
 * Overall, `config.hpp` serves as the backbone for the library's adaptability and
 * robustness in varied development environments.
 */

#ifndef IPADDRESS_CONFIG_HPP
#define IPADDRESS_CONFIG_HPP

#ifndef IPADDRESS_MODULE
#  include <cstdint>
#  include <cstddef>
#  include <set>
#  include <map>
#  include <array>
#  include <tuple>
#  include <cmath>
#  include <vector>
#  include <cassert>
#  include <sstream>
#  include <iomanip>
#  include <cstring>
#  include <numeric>
#  include <iterator>
#  include <algorithm>
#  include <stdexcept>
#  include <type_traits>
#endif

#ifndef IPADDRESS_NAMESPACE
#  define IPADDRESS_NAMESPACE ipaddress
#endif

#ifndef IPADDRESS_MODULE
#  define IPADDRESS_EXPORT
#else
#  define IPADDRESS_EXPORT export
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
#  if __cpp_constexpr >= 201304L
#    define IPADDRESS_CONSTEXPR constexpr
#  else
#    define IPADDRESS_CONSTEXPR
#  endif
#  if __cpp_constexpr >= 201603L
#    define IPADDRESS_CONSTEXPR_17 constexpr
#  else
#    define IPADDRESS_CONSTEXPR_17
#  endif
#else
#  error ipaddress needs at least C++ standard version 11
#endif

#if defined(__cpp_consteval) && __cpp_consteval >= 201811L
#  define IPADDRESS_CONSTEVAL consteval
#else
#  define IPADDRESS_CONSTEVAL IPADDRESS_CONSTEXPR
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

#ifdef _MSC_VER
#  define IPADDRESS_FORCE_INLINE __forceinline
#else
#  define IPADDRESS_FORCE_INLINE inline __attribute__((always_inline))
#endif

#if !defined(IPADDRESS_NO_SPACESHIP_OPERATOR) && defined(__has_include)
#  if (__cpp_lib_three_way_comparison >= 201907L) && __has_include(<compare>)
#    define IPADDRESS_HAS_SPACESHIP_OPERATOR
#    ifndef IPADDRESS_MODULE
#      include <compare>
#    endif
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

#if defined(__cpp_lib_is_constant_evaluated) && __cpp_lib_is_constant_evaluated >= 201811L
#  define IPADDRESS_IS_CONST_EVALUATED(x) std::is_constant_evaluated()
#elif (defined(__GNUC__) && !defined(__clang__) && __GNUC__ >= 9) || \
      (defined(__clang__) && !defined(__apple_build_version__) && __clang_major__ >= 9) || \
      (defined(__clang__) && defined(__apple_build_version__) && __clang_major__ >= 11) || \
      (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1910) || \
      (defined(_MSC_VER) && _MSC_VER >= 1925)
#  define IPADDRESS_IS_CONST_EVALUATED(x) __builtin_is_constant_evaluated()
#elif defined(__GNUC__) || defined(__clang__)
#  define IPADDRESS_IS_CONST_EVALUATED(x) __builtin_constant_p(x)
#else
#  define IPADDRESS_IS_CONST_EVALUATED(x) false
#endif

#if IPADDRESS_CPP_VERSION >= 17 && !defined(IPADDRESS_MODULE)
#  include <string_view>
#endif

#if defined(IPADDRESS_NO_IPV6_SCOPE)
#  undef IPADDRESS_IPV6_SCOPE_MAX_LENGTH
#  define IPADDRESS_IPV6_SCOPE_MAX_LENGTH 0
#elif defined(IPADDRESS_IPV6_SCOPE_MAX_LENGTH)
#  if IPADDRESS_IPV6_SCOPE_MAX_LENGTH < 0
#    undef IPADDRESS_IPV6_SCOPE_MAX_LENGTH
#    define IPADDRESS_IPV6_SCOPE_MAX_LENGTH 0
#  elif IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 64
#    undef IPADDRESS_IPV6_SCOPE_MAX_LENGTH
#    define IPADDRESS_IPV6_SCOPE_MAX_LENGTH 64
#  endif
#else
#  define IPADDRESS_IPV6_SCOPE_MAX_LENGTH 16
#endif

#endif // IPADDRESS_CONFIG_HPP
