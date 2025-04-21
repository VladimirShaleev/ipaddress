/**
 * @file      ip-functions.hpp
 * @brief     
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * ...
 */

#ifndef IPADDRESS_IP_FUNCTIONS_HPP
#define IPADDRESS_IP_FUNCTIONS_HPP
 
#include "ip-any-network.hpp"
 
namespace IPADDRESS_NAMESPACE {

namespace internal {

template <typename T1, typename T2>
struct ip_address_type {
    static_assert(
        std::is_same<T1, ip_address>::value ||
        std::is_same<T2, ip_address>::value ||
        std::is_same<T1, T2>::value,
        "The IP address versions must match or at least be ip_address."
    );

    using type = typename std::conditional<
        std::is_same<T1, ip_address>::value || std::is_same<T2, ip_address>::value,
        ip_address,
        T1
    >::type;
};

template <typename>
struct summarize_sequence_type;

template <>
struct summarize_sequence_type<ipv4_address> {
    using type = summarize_sequence<ipv4_network>;
};

template <>
struct summarize_sequence_type<ipv6_address> {
    using type = summarize_sequence<ipv6_network>;
};

template <>
struct summarize_sequence_type<ip_address> {
    using type = summarize_sequence<ip_network, ip_any_summarize_iterator>;
};

template <typename Ip>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE typename summarize_sequence_type<Ip>::type summarize_address_range(const Ip& first, const Ip& last, error_code& code) IPADDRESS_NOEXCEPT {
    code = error_code::no_error;
    if (first.version() != last.version()) {
        code = error_code::invalid_version;
        return {};
    }
    if (first > last) {
        code = error_code::last_address_must_be_greater_than_first;
        return {};
    }
    return { first, last };
}

} // namespace IPADDRESS_NAMESPACE::internal

template <typename FirstIp, typename LastIp>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto summarize_address_range(const FirstIp& first, const LastIp& last, error_code& code) IPADDRESS_NOEXCEPT
    -> decltype(internal::summarize_address_range(
        typename internal::ip_address_type<FirstIp, LastIp>::type{first}, 
        typename internal::ip_address_type<FirstIp, LastIp>::type{last}, 
        code)) {
    return internal::summarize_address_range(typename internal::ip_address_type<FirstIp, LastIp>::type{first}, typename internal::ip_address_type<FirstIp, LastIp>::type{last}, code);
}

template <typename FirstIp, typename LastIp>
IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto summarize_address_range(const FirstIp& first, const LastIp& last) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS
    -> decltype(summarize_address_range(first, last, *std::declval<error_code*>())) {
    error_code code = error_code::no_error;
    const auto result = summarize_address_range(first, last, code);
    if (code != error_code::no_error) {
        raise_error(code, 0, "", 0);
    }
    return result;
}

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_FUNCTIONS_HPP
