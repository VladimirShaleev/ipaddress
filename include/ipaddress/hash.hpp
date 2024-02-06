#ifndef IPADDRESS_HASH_HPP
#define IPADDRESS_HASH_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

template <std::size_t Size>
struct hash_combine;

template <>
struct hash_combine<4> {
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t operator()(std::uint32_t value) const IPADDRESS_NOEXCEPT {
        value ^= value >> 16;
        value *= 0x21f0aaad;
        value ^= value >> 15;
        value *= 0x735a2d97;
        value ^= value >> 15;
        return std::size_t(value);
    }
};

template <>
struct hash_combine<8> {
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t operator()(std::uint64_t value) const IPADDRESS_NOEXCEPT {
        value ^= value >> 32;
        value *= 0xe9846af9b1a615d;
        value ^= value >> 32;
        value *= 0xe9846af9b1a615d;
        value ^= value >> 28;
        return std::size_t(value);
    }
};

IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t hash_sum(std::size_t seed, std::size_t value) IPADDRESS_NOEXCEPT {
    hash_combine<sizeof(std::size_t)> hash{};
    return hash(seed + 0x9e3779b9 + value);
}

template <typename Arg>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t calc_hash(std::size_t seed, Arg arg) IPADDRESS_NOEXCEPT {
    return hash_sum(seed, arg);
}

template <typename Arg, typename... Args>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t calc_hash(std::size_t seed, Arg arg, Args... args) IPADDRESS_NOEXCEPT {
    seed = hash_sum(seed, std::size_t(arg));
    return calc_hash(seed, args...);
}

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_HASH_HPP
