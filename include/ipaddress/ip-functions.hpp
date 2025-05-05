/**
 * @file      ip-functions.hpp
 * @brief     Utility functions for IP address
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This file contains functions for processing IP address ranges and networks.
 * It provides functionalities `summarize_address_range`. These utilities
 * enable efficient handling and manipulation of IP address data.
 */

#ifndef IPADDRESS_IP_FUNCTIONS_HPP
#define IPADDRESS_IP_FUNCTIONS_HPP
 
#include "ip-any-network.hpp"
#include "fixed-vector.hpp"
 
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

template <typename T1, typename T2>
struct ip_network_type {
    static_assert(
        std::is_same<T1, ip_network>::value ||
        std::is_same<T2, ip_network>::value ||
        std::is_same<T1, T2>::value,
        "The IP network versions must match or at least be ip_network."
    );

    using type = typename std::conditional<
        std::is_same<T1, ip_network>::value || std::is_same<T2, ip_network>::value,
        ip_network,
        T1
    >::type;
};

template <typename... Args>
struct is_ip_network_types {
    static constexpr bool value = false;
};

template <typename T, typename... Args>
struct is_ip_network_types<T, Args...> {
    static constexpr bool value = is_ip_network_types<T>::value && (sizeof...(Args) > 0 ? is_ip_network_types<Args...>::value : true);
};

template <typename T>
struct is_ip_network_types<T> {
    static constexpr bool value = std::is_same<T, ipv4_network>::value || std::is_same<T, ipv6_network>::value || std::is_same<T, ip_network>::value;
};

template <typename... Args>
struct ip_network_type_extract;

template <typename T1, typename T2, typename... Args>
struct ip_network_type_extract<T1, T2, Args...> {
    using type = typename ip_network_type<typename ip_network_type<T1, T2>::type, typename ip_network_type_extract<Args...>::type>::type;
};

template <typename T1, typename T2>
struct ip_network_type_extract<T1, T2> {
    using type = typename ip_network_type<T1, T2>::type;
};

template <typename T>
struct ip_network_type_extract<T> {
    using type = typename ip_network_type<T, T>::type;
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

template <typename Key, typename Value>
struct key_value {
    Key key;
    Value value;
};

template <typename Key, typename Value>
struct less_pair {
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator()(const key_value<Key, Value>& item, const Key& key) const IPADDRESS_NOEXCEPT {
        return item.key < key;
    }
};

template <typename It, typename T, typename Cmp = std::less<T>>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE It find_lower_bound(It first, It last, const T& value, Cmp&& cmp = {}) IPADDRESS_NOEXCEPT {
    auto size = last - first;
    while (size > 0) {
        auto half = size / 2;
        auto middle = first;
        middle += half;
        if (cmp(*middle, value)) {
            first = middle;
            ++first;
            size = size - half - 1;
        } else {
            size = half;
        }
    }
    return first;
}

IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_clang_8_and_below() IPADDRESS_NOEXCEPT {
#if defined(__clang__) && !defined(__apple_build_version__) && __clang_major__ <= 8
    return true;
#else
    return false;
#endif
}

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

template <size_t N, typename It>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto fixed_collapse_addresses(It first, It last, error_code& code) IPADDRESS_NOEXCEPT
    -> fixed_vector<typename std::iterator_traits<It>::value_type, N> {
    using network_type = typename std::iterator_traits<It>::value_type;
    using address_type = typename network_type::ip_address_type;

    const auto version = first->version();
    const auto max_prefixlen = version == ip_version::V4
        ? ipv4_network::base_max_prefixlen
        : ipv6_network::base_max_prefixlen;

    fixed_vector<network_type, N> nets;
    fixed_vector<address_type, N> ips;
    fixed_vector<key_value<network_type, network_type>, N> subnets;

    for (auto it = first; it != last; ++it) {
        const auto& net = *it;
        if (net.version() != version) {
            code = error_code::invalid_version;
            return {};
        }
        if (net.prefixlen() != max_prefixlen) {
            nets.emplace_back(net);
        } else {
            const auto network_address = net.network_address();
            auto lower = find_lower_bound(ips.begin(), ips.end(), network_address);
            if (lower == ips.end() || *lower != network_address) {
                ips.insert(lower, network_address);
            }
        }
    }
    if (!ips.empty()) {
        auto it = ips.begin();
        auto first = *it;
        auto last = *it++;
        auto lastUint = (typename network_type::uint_type) last;
        for (; it != ips.end(); ++it) {
            const auto ipUint = (typename network_type::uint_type) *it;
            if (ipUint != lastUint + 1) {
                auto range = summarize_address_range(first, last, code);
                if (code != error_code::no_error) {
                    return {};
                }
                for (const auto& net : range) {
                    nets.emplace_back(net);
                }
                first = *it;
            }
            last = *it;
            lastUint = ipUint;
        }
        auto range = summarize_address_range(first, last, code);
        if (code != error_code::no_error) {
            return {};
        }
        for (const auto& net : range) {
            nets.emplace_back(net);
        }
    }
    while (!nets.empty()) {
        const auto net = nets.back();
        const auto supernet = net.supernet();
        nets.pop_back();
        auto it = find_lower_bound(subnets.begin(), subnets.end(), supernet, less_pair<network_type, network_type>{});
        if (it != subnets.end() && it->key == supernet) {
            if (it->value != net) {
                subnets.erase(it);
                nets.emplace_back(supernet);
            }
        } else {
            subnets.emplace(it, supernet, net);
        }
    }
    fixed_vector<network_type, N> result;
    if (!subnets.empty()) {
        fixed_vector<network_type, N> subnet_values;
        for (auto it = subnets.begin(); it != subnets.end(); ++it) {
            auto lower = find_lower_bound(subnet_values.begin(), subnet_values.end(), it->key);
            subnet_values.insert(lower, it->value);
        }
        auto it = subnet_values.begin();
        auto last = *it;
        result.emplace_back(last);
        ++it;
        for (; it != subnet_values.end(); ++it) {
            if (last.broadcast_address() >= it->broadcast_address()) {
                continue;
            }
            last = *it;
            result.emplace_back(last);
        }
    }
    return result;
}

template <typename Result, typename It>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE Result runtime_collapse_addresses(It first, It last, error_code& code) IPADDRESS_NOEXCEPT {
    using network_type = typename std::iterator_traits<It>::value_type;
    using address_type = typename network_type::ip_address_type;

    Result result;
    if (first == last) {
        return result;
    }
    
    const auto version = first->version();
    const auto max_prefixlen = version == ip_version::V4
        ? ipv4_network::base_max_prefixlen
        : ipv6_network::base_max_prefixlen;

    std::vector<network_type> nets;
    std::set<address_type> ips;
    std::map<network_type, network_type> subnets;

    for (auto it = first; it != last; ++it) {
        const auto& net = *it;
        if (net.version() != version) {
            code = error_code::invalid_version;
            return {};
        }
        if (net.prefixlen() != max_prefixlen) {
            nets.emplace_back(net);
        } else {
            const auto network_address = net.network_address();
            ips.insert(network_address);
        }
    }
    if (!ips.empty()) {
        auto it = ips.begin();
        auto first = *it;
        auto last = *it++;
        auto lastUint = (typename network_type::uint_type) last;
        for (; it != ips.end(); ++it) {
            const auto ipUint = (typename network_type::uint_type) *it;
            if (ipUint != lastUint + 1) {
                auto range = summarize_address_range(first, last, code);
                if (code != error_code::no_error) {
                    return {};
                }
                for (const auto& net : range) {
                    nets.emplace_back(net);
                }
                first = *it;
            }
            last = *it;
            lastUint = ipUint;
        }
        auto range = summarize_address_range(first, last, code);
        if (code != error_code::no_error) {
            return {};
        }
        for (const auto& net : range) {
            nets.emplace_back(net);
        }
    }
    while (!nets.empty()) {
        const auto net = nets.back();
        const auto supernet = net.supernet();
        nets.pop_back();
        auto existing = subnets.find(supernet);
        if (existing != subnets.end()) {
            subnets.erase(existing);
            nets.emplace_back(supernet);
        } else {
            subnets.emplace(std::make_pair(supernet, net));
        }
    }
    if (!subnets.empty()) {
        std::vector<network_type> subnet_values;
        subnet_values.reserve(subnets.size());
        for (auto it = subnets.begin(); it != subnets.end(); ++it) {
            subnet_values.emplace_back(it->second);
        }
        std::sort(subnet_values.begin(), subnet_values.end());
        auto it = subnet_values.begin();
        auto last = *it;
        result.emplace_back(last);
        ++it;
        for (; it != subnet_values.end(); ++it) {
            if (last.broadcast_address() >= it->broadcast_address()) {
                continue;
            }
            last = *it;
            result.emplace_back(last);
        }
    }
    return result;
}

template <size_t N, typename It>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(It first, It last, error_code& code) IPADDRESS_NOEXCEPT
    -> fixed_vector<typename std::iterator_traits<It>::value_type, N> {
    using result_type = fixed_vector<typename std::iterator_traits<It>::value_type, N>;
    // fixed_collapse_addresses is called in the following 3 cases:
    //   1. the calculation is performed at compile time;
    //   2. if the number of elements in the input collection is less than or equal to 16,
    //      fixed_collapse_addresses is used, even though it will be executed at runtime;
    //      in fact, fixed_collapse_addresses is usually faster than runtime_collapse_addresses,
    //      but due to the limited stack size, it should not be used for large N;
    //   3. before clang 9 there was no clear way to tell if execution happens at compile time or
    //      runtime, so for clang 8 and below we always go with the fixed_collapse_addresses pipeline.
    return IPADDRESS_IS_CONST_EVALUATED(code) || N <= 16 || is_clang_8_and_below()
        ? fixed_collapse_addresses<N>(first, last, code)
        : runtime_collapse_addresses<result_type>(first, last, code);
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
 * @throw logic_error Thrown with a message corresponding to the error code.
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

/**
 * Collapses a collection of IP networks into the smallest set of contiguous networks.
 * 
 * This function is designed to take a collection of IP networks and reduce them into the
 * smallest number of contiguous networks. This is useful for optimizing routing tables or
 * enhancing network efficiency.
 * 
 * Example:
 * @code{.cpp}
 *   consteval ipv4_network get_last_collapsed() {
 *       constexpr std::array nets = {
 *           ipv4_network::parse("192.0.2.0/25"),
 *           ipv4_network::parse("192.0.2.128/25")
 *       };
 *  
 *       error_code code{};
 *       constexpr auto collapsed = collapse_addresses(nets, code);
 *       return code == error_code::no_error ? collapsed.back() : ipv4_network{};
 *   }
 *   
 *   int main() {
 *       constexpr auto net = get_last_collapsed();
 *       std::cout << net << std::endl;
 * 
 *       // out:
 *       // 192.0.2.0/24
 *       return 0;
 *   }
 * @endcode
 * 
 * @tparam Net The type of the IP network.
 * @tparam N The number of networks in the collection.
 * @param[in] nets The collection of IP networks to be collapsed.
 * @param[out] code A reference to an `error_code` object that will be set if the operation is not possible.
 * @return A container of collapsed networks.
 */
IPADDRESS_EXPORT template <typename Net, size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(const std::array<Net, N>& nets, error_code& code) IPADDRESS_NOEXCEPT
    -> decltype(N != 0 ? internal::collapse_addresses<N>(&nets[0], &nets[0] + nets.size(), code) : fixed_vector<Net, N>{}) {
    return N != 0 ? internal::collapse_addresses<N>(&nets[0], &nets[0] + nets.size(), code) : fixed_vector<Net, N>{};
}

/**
 * Collapses a collection of IP networks into the smallest set of contiguous networks.
 * 
 * This function is designed to take a collection of IP networks and reduce them into the
 * smallest number of contiguous networks. This is useful for optimizing routing tables or
 * enhancing network efficiency.
 * 
 * Example:
 * @code{.cpp}
 *   consteval ip_network get_last_collapsed() {
 *       constexpr ip_network nets[] = {
 *           ip_network::parse("192.0.2.0/25"),
 *           ip_network::parse("192.0.2.128/25")
 *       };
 *   
 *       error_code code{};
 *       constexpr auto collapsed = collapse_addresses(nets, code);
 *       return code == error_code::no_error ? collapsed.back() : ip_network{};
 *   }
 *   
 *   int main() {
 *       constexpr auto net = get_last_collapsed();
 *       std::cout << net << std::endl;
 *   
 *       // out:
 *       // 192.0.2.0/24
 *       return 0;
 *   }
 * @endcode
 * 
 * @tparam Net The type of the IP network.
 * @tparam N The number of networks in the collection.
 * @param[in] nets The collection of IP networks to be collapsed.
 * @param[out] code A reference to an `error_code` object that will be set if the operation is not possible.
 * @return A container of collapsed networks.
 */
IPADDRESS_EXPORT template <typename Net, size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(const Net (&nets)[N], error_code& code) IPADDRESS_NOEXCEPT
    -> decltype(internal::collapse_addresses<N>(&nets[0], &nets[0] + N, code)) {
    return internal::collapse_addresses<N>(&nets[0], &nets[0] + N, code);
}

/**
 * Collapses a collection of IP networks into the smallest set of contiguous networks.
 * 
 * This function is designed to take a collection of IP networks and reduce them into the
 * smallest number of contiguous networks. This is useful for optimizing routing tables or
 * enhancing network efficiency.
 * 
 * Example:
 * @code{.cpp}
 *   consteval ip_network get_last_collapsed() {
 *       error_code code{};
 *       auto collapsed = collapse_addresses(
 *           code,
 *           ipv4_network::parse("192.0.2.0/25"), 
 *           ip_network::parse("192.0.2.128/25"), 
 *           ip_network::parse("192.0.2.255/32"), 
 *           ip_network::parse("192.0.2.255/32"));
 *       return code == error_code::no_error ? collapsed.back() : ip_network{};
 *   }
 *
 *   int main() {
 *       constexpr auto net = get_collapsed_net();
 *       std::cout << net << std::endl;
 *
 *       // out:
 *       // 192.0.2.0/24
 *       return 0;
 *   }
 * @endcode
 * 
 * @tparam... Nets The types of the IP networks.
 * @param[out] code A reference to an `error_code` object that will be set if the operation is not possible.
 * @param[in] nets The collection of IP networks to be collapsed.
 * @return A container of collapsed networks.
 */
IPADDRESS_EXPORT template <typename... Nets, typename std::enable_if<internal::is_ip_network_types<Nets...>::value, bool>::type = true>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(error_code& code, const Nets&... nets) IPADDRESS_NOEXCEPT
    -> fixed_vector<typename internal::ip_network_type_extract<Nets...>::type, sizeof...(Nets)> {
    using Net = typename internal::ip_network_type_extract<Nets...>::type;
    const Net net_array[] = { Net(nets)... };
    return collapse_addresses(net_array, code);
}

/**
 * Collapses a collection of IP networks into the smallest set of contiguous networks.
 * 
 * This function is designed to take a collection of IP networks and reduce them into the
 * smallest number of contiguous networks. This is useful for optimizing routing tables or
 * enhancing network efficiency.
 * 
 * Example:
 * @code{.cpp}
 *   std::vector<ip_network> nets = {
 *       ip_network::parse("2001:db8::1/128"),
 *       ip_network::parse("2001:db8::2/128"),
 *       ip_network::parse("2001:db8::3/128"),
 *       ip_network::parse("2001:db8::5/128") };
 *       
 *   error_code code{};
 *   const auto collapsed = collapse_addresses(nets.begin(), nets.end(), code);
 *   if (code == error_code::no_error) {
 *       for (const auto& net : collapsed) {
 *           std::cout << net << std::endl;
 *       }
 *   }
 *   
 *   // out:
 *   // 2001:db8::1/128
 *   // 2001:db8::2/127
 *   // 2001:db8::5/128
 * @endcode
 * 
 * @tparam It The type of the iterator.
 * @param[in] first The beginning of the range of IP networks to be collapsed.
 * @param[in] last The end of the range of IP networks to be collapsed.
 * @param[out] code A reference to an `error_code` object that will be set if the operation is not possible.
 * @return A container of collapsed networks.
 */
IPADDRESS_EXPORT template <typename It>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(It first, It last, error_code& code) IPADDRESS_NOEXCEPT
    -> std::vector<typename std::iterator_traits<It>::value_type> {
    return internal::runtime_collapse_addresses<std::vector<typename std::iterator_traits<It>::value_type>>(first, last, code);
}

/**
 * Collapses a collection of IP networks into the smallest set of contiguous networks.
 * 
 * This function is designed to take a collection of IP networks and reduce them into the
 * smallest number of contiguous networks. This is useful for optimizing routing tables or
 * enhancing network efficiency.
 * 
 * Example:
 * @code{.cpp}
 *   constexpr std::array<ipv4_network, 2> nets = {
 *      ipv4_network::parse("192.0.2.0/25"),
 *      ipv4_network::parse("192.0.2.128/25")
 *   };
 * 
 *   constexpr auto collapsed = collapse_addresses(nets);
 *   for (const auto& net : collapsed) {
 *       std::cout << net << std::endl;
 *   }
 * 
 *   // out:
 *   // 192.0.2.0/24
 * @endcode
 * 
 * @tparam Net The type of the IP network.
 * @tparam N The number of networks in the collection.
 * @param[in] nets The collection of IP networks to be collapsed.
 * @return A container of collapsed networks.
 * @throw logic_error Thrown with a message corresponding to the error code.
 */
IPADDRESS_EXPORT template <typename Net, size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(const std::array<Net, N>& nets) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS
    -> decltype(collapse_addresses(nets, *std::declval<error_code*>())) {
    error_code code = error_code::no_error;
    const auto result = collapse_addresses(nets, code);
    if (code != error_code::no_error) {
        raise_error(code, 0, "", 0);
    }
    return result;
}

/**
 * Collapses a collection of IP networks into the smallest set of contiguous networks.
 * 
 * This function is designed to take a collection of IP networks and reduce them into the
 * smallest number of contiguous networks. This is useful for optimizing routing tables or
 * enhancing network efficiency.
 * 
 * Example:
 * @code{.cpp}
 *   constexpr ip_network nets[] = {
 *       ip_network::parse("192.0.2.0/25"),
 *       ip_network::parse("192.0.2.128/25")
 *   };
 * 
 *   constexpr auto collapsed = collapse_addresses(nets);
 *   for (const auto& net : collapsed) {
 *       std::cout << net << std::endl;
 *   }
 * 
 *   // out:
 *   // 192.0.2.0/24
 * @endcode
 * 
 * @tparam Net The type of the IP network.
 * @tparam N The number of networks in the collection.
 * @param[in] nets The collection of IP networks to be collapsed.
 * @return A container of collapsed networks.
 * @throw logic_error Thrown with a message corresponding to the error code.
 */
IPADDRESS_EXPORT template <typename Net, size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(const Net (&nets)[N]) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS
    -> decltype(collapse_addresses(nets, *std::declval<error_code*>())) {
    error_code code = error_code::no_error;
    const auto result = collapse_addresses(nets, code);
    if (code != error_code::no_error) {
        raise_error(code, 0, "", 0);
    }
    return result;
}

/**
 * Collapses a collection of IP networks into the smallest set of contiguous networks.
 * 
 * This function is designed to take a collection of IP networks and reduce them into the
 * smallest number of contiguous networks. This is useful for optimizing routing tables or
 * enhancing network efficiency.
 * 
 * Example:
 * @code{.cpp}
 *   constexpr auto collapsed = collapse_addresses( 
 *       ip_network::parse("192.0.2.255/32"),
 *       ipv4_network::parse("192.0.2.0/25"), 
 *       ipv4_network::parse("192.0.2.128/25"));
 *   
 *   for (const auto& net : collapsed) {
 *       std::cout << net << std::endl;
 *   }
 * 
 *   // out:
 *   // 192.0.2.0/24
 * @endcode
 * 
 * @tparam... Nets The types of the IP networks.
 * @param[in] nets The collection of IP networks to be collapsed.
 * @return A container of collapsed networks.
 * @throw logic_error Thrown with a message corresponding to the error code.
 */
IPADDRESS_EXPORT template <typename... Nets, typename std::enable_if<internal::is_ip_network_types<Nets...>::value, bool>::type = true>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(const Nets&... nets) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS
    -> fixed_vector<typename internal::ip_network_type_extract<Nets...>::type, sizeof...(Nets)> {
    error_code code = error_code::no_error;
    const auto result = collapse_addresses(code, nets...);
    if (code != error_code::no_error) {
        raise_error(code, 0, "", 0);
    }
    return result;
}

/**
 * Collapses a collection of IP networks into the smallest set of contiguous networks.
 * 
 * This function is designed to take a collection of IP networks and reduce them into the
 * smallest number of contiguous networks. This is useful for optimizing routing tables or
 * enhancing network efficiency.
 * 
 * Example:
 * @code{.cpp}
 *   std::vector<ip_network> nets = {
 *       ip_network::parse("2001:db8::1/128"),
 *       ip_network::parse("2001:db8::2/128"),
 *       ip_network::parse("2001:db8::3/128"),
 *       ip_network::parse("2001:db8::5/128") };
 *   
 *   const auto collapsed = collapse_addresses(nets.begin(), nets.end());
 *   for (const auto& net : collapsed) {
 *       std::cout << net << std::endl;
 *   }
 *   
 *   // out:
 *   // 2001:db8::1/128
 *   // 2001:db8::2/127
 *   // 2001:db8::5/128
 * @endcode
 *
 * @tparam It The type of the iterator.
 * @param[in] first The beginning of the range of IP networks to be collapsed.
 * @param[in] last The end of the range of IP networks to be collapsed.
 * @return A container of collapsed networks.
 * @throw logic_error Thrown with a message corresponding to the error code.
 */
IPADDRESS_EXPORT template <typename It>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(It first, It last) IPADDRESS_NOEXCEPT
    -> std::vector<typename std::iterator_traits<It>::value_type> {
    error_code code = error_code::no_error;
    const auto result = collapse_addresses(first, last, code);
    if (code != error_code::no_error) {
        raise_error(code, 0, "", 0);
    }
    return std::move(result);
}

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_FUNCTIONS_HPP
