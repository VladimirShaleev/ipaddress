/**
 * @file      hash.hpp
 * @brief     Algorithms for calculating hash sums
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * The `hash.hpp` file contains hash templates and functions that can be used to 
 * hash IP addresses or other numeric data.
 * 
 * This code uses patterns and specializations to optimize the hashing process 
 * for different sizes of data. Magic numbers and bitwise operations in hash_combine 
 * functions help distribute hash values evenly, which is important for avoiding 
 * collisions in hash tables. The hash_sum and calc_hash functions provide a convenient 
 * interface for calculating hash sums and computing the hash sum of multiple values.
 */

#ifndef IPADDRESS_HASH_HPP
#define IPADDRESS_HASH_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

namespace internal {

template <size_t Size>
struct hash_combine;

template <>
struct hash_combine<4> {
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t operator()(uint32_t value) const IPADDRESS_NOEXCEPT {
        value ^= value >> 16;
        value *= 0x21f0aaad;
        value ^= value >> 15;
        value *= 0x735a2d97;
        value ^= value >> 15;
        return size_t(value);
    }
};

template <>
struct hash_combine<8> {
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t operator()(uint64_t value) const IPADDRESS_NOEXCEPT {
        value ^= value >> 32;
        value *= 0xe9846af9b1a615d;
        value ^= value >> 32;
        value *= 0xe9846af9b1a615d;
        value ^= value >> 28;
        return size_t(value);
    }
};

IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t hash_sum(size_t seed, size_t value) IPADDRESS_NOEXCEPT {
    const hash_combine<sizeof(size_t)> hash{};
    return hash(seed + 0x9e3779b9 + value);
}

template <typename Arg>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t calc_hash(size_t seed, Arg arg) IPADDRESS_NOEXCEPT {
    return hash_sum(seed, arg);
}

template <typename Arg, typename... Args>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t calc_hash(size_t seed, Arg arg, Args... args) IPADDRESS_NOEXCEPT {
    seed = hash_sum(seed, size_t(arg));
    return calc_hash(seed, args...);
}

} // namespace IPADDRESS_NAMESPACE::internal

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_HASH_HPP
