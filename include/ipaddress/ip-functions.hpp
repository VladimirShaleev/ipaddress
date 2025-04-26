/**
 * @file      ip-functions.hpp
 * @brief     Utility functions for IP address
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This file contains functions for processing IP address ranges and networks.
 * It provides functionalities `summarize_address_range` and  `collapse_addresses`.
 * These utilities enable efficient handling and manipulation of IP address data.
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

/**
 * Summarizes an IP address range into the smallest set of contiguous network blocks.
 *
 * This function is designed specifically for iterating over the minimal number of 
 * contiguous network blocks that fully cover a given IP address range. By summarizing 
 * the range, it reduces redundancy and enhances efficiency, making it ideal for 
 * scenarios like routing table generation or network optimization tasks.
 * 
 * Example:
 * @code{.cpp}
 *   error_code code{};
 *   auto summarized = summarize_address_range(
 *       ipv6_address::parse("2001:db8::1"), 
 *       ipv6_address::parse("2001:db8::8"), 
 *       code
 *   );
 *   if (code == error_code::no_error) {
 *       for (const auto& net : summarized) {
 *           std::cout << net << std::endl;
 *       }
 *   }
 * 
 *   // out:
 *   // 2001:db8::1/128
 *   // 2001:db8::2/127
 *   // 2001:db8::4/126
 *   // 2001:db8::8/128
 * 
 *   // When working with ip_address, you can also use ipv4_address or ipv6_address as one of the arguments.
 *   // In this case, this address will be interpreted as ip_address, and iteration will occur for ip_network
 *   // summarize_address_range(ipv6_address::parse("2001:db8::1"), ip_address::parse("2001:db8::8"), code); // OK
 * @endcode
 * 
 * @tparam FirstIp The type representing an IP address.
 * @tparam LastIp The type representing an IP address.
 * @param[in] start The starting IP address of the range.
 * @param[in] end The ending IP address of the range.
 * @param[out] code A reference to an `error_code` object that will be set if the operation is not possible.
 * @return A container of summarized network blocks.
 * @note This function performs an efficient summarization of IP ranges, but the computational 
 *       complexity may depend on the size of the range.
 */
IPADDRESS_EXPORT template <typename FirstIp, typename LastIp>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto summarize_address_range(const FirstIp& first, const LastIp& last, error_code& code) IPADDRESS_NOEXCEPT
    -> decltype(internal::summarize_address_range(
        typename internal::ip_address_type<FirstIp, LastIp>::type{first}, 
        typename internal::ip_address_type<FirstIp, LastIp>::type{last}, 
        code)) {
    return internal::summarize_address_range(typename internal::ip_address_type<FirstIp, LastIp>::type{first}, typename internal::ip_address_type<FirstIp, LastIp>::type{last}, code);
}

/**
 * Summarizes an IP address range into the smallest set of contiguous network blocks.
 *
 * This function is designed specifically for iterating over the minimal number of 
 * contiguous network blocks that fully cover a given IP address range. By summarizing 
 * the range, it reduces redundancy and enhances efficiency, making it ideal for 
 * scenarios like routing table generation or network optimization tasks.
 * 
 * Example:
 * @code{.cpp}
 *   for (const auto& net : summarize_address_range(ipv4_address::parse("192.0.2.0"), ipv4_address::parse("192.0.2.130"))) {
 *       std::cout << net << std::endl;
 *   }
 * 
 *   // out:
 *   // 192.0.2.0/25
 *   // 192.0.2.128/31
 *   // 192.0.2.130/32
 * 
 *   // When working with ip_address, you can also use ipv4_address or ipv6_address as one of the arguments.
 *   // In this case, this address will be interpreted as ip_address, and iteration will occur for ip_network
 *   // summarize_address_range(ipv4_address::parse("192.0.2.0"), ip_address::parse("192.0.2.130")); // OK
 * @endcode
 * 
 * @tparam FirstIp The type representing an IP address.
 * @tparam LastIp The type representing an IP address.
 * @param[in] start The starting IP address of the range.
 * @param[in] end The ending IP address of the range.
 * @return A container of summarized network blocks.
 * @note This function performs an efficient summarization of IP ranges, but the computational 
 *       complexity may depend on the size of the range.
 */
IPADDRESS_EXPORT template <typename FirstIp, typename LastIp>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto summarize_address_range(const FirstIp& first, const LastIp& last) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS
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
