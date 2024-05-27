module;

#include <cstdint>
#include <cstddef>
#include <array>
#include <tuple>
#include <cmath>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <string_view>

#if __has_include(<compare>)
#  include <compare>
#endif

#if __has_include(<bit>)
#  include <bit>
#endif

export module ipaddress;

#define IPADDRESS_MODULE
#if defined(_MSVC_LANG)
#  define IPADDRESS_NO_SPACESHIP_OPERATOR
#endif

#pragma warning(push, 3)
#if defined(_MSC_VER)
#  pragma warning(disable:5244)
#endif

#include <ipaddress/ipaddress.hpp>

#if defined(_MSC_VER)
#  pragma warning(default:5244)
#endif
#pragma warning(pop)
