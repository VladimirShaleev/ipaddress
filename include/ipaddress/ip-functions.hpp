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

template <typename T>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto ip_network_type_extract(const T&) IPADDRESS_NOEXCEPT
    -> typename ip_network_type<T, T>::type {
    return typename ip_network_type<T, T>::type{};
}

template <typename T1, typename T2, typename... Args>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto ip_network_type_extract(const T1&, const T2&, const Args&... args) IPADDRESS_NOEXCEPT
    -> decltype(ip_network_type_extract(typename ip_network_type<T1, T2>::type{}, args...)) {
    return ip_network_type_extract(typename ip_network_type<T1, T2>::type{}, args...);
}

template <typename...>
IPADDRESS_NODISCARD IPADDRESS_CONSTEVAL IPADDRESS_FORCE_INLINE bool check_ip_network_types() IPADDRESS_NOEXCEPT {
    return true;
}

template <typename T>
IPADDRESS_NODISCARD IPADDRESS_CONSTEVAL IPADDRESS_FORCE_INLINE bool check_ip_network_types(T) IPADDRESS_NOEXCEPT {
    using type = typename std::remove_pointer<T>::type;
    return std::is_same<type, ipv4_network>::value || std::is_same<type, ipv6_network>::value || std::is_same<type, ip_network>::value;
}

template <typename T, typename... Args>
IPADDRESS_NODISCARD IPADDRESS_CONSTEVAL IPADDRESS_FORCE_INLINE bool check_ip_network_types(T arg, Args... args) IPADDRESS_NOEXCEPT {
    return check_ip_network_types(arg) && check_ip_network_types(args...);
}

template <typename... Args>
struct is_ip_network_types {
    static constexpr bool value = sizeof...(Args) == 0 ? false : check_ip_network_types<typename std::add_pointer<Args>::type...>(typename std::add_pointer<Args>::type{}...);
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

template <typename It, typename T, typename Cmp = std::less<>>
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

template <typename T, size_t N>
struct constexpr_vector {
    using value_type      = T;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    static constexpr size_t capacity = N;
    T _data[capacity];
    size_t _size{};

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE constexpr_vector() IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T& insert(T* it, const T& value) IPADDRESS_NOEXCEPT {
        auto pos = ptrdiff_t(it - _data);
        if (pos < _size) {
            for (auto i = ptrdiff_t(_size - pos - 1); i >= pos; --i) {
                _data[i + 1] = _data[i];
            }
        }
        ++_size;
        return _data[pos] = value;
    }

    template <typename... Args>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T& emplace_back(Args&&... args) IPADDRESS_NOEXCEPT {
        return _data[_size++] = T(std::forward<Args>(args)...);
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void pop_back() IPADDRESS_NOEXCEPT {
        --_size;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return _size;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool empty() const IPADDRESS_NOEXCEPT {
        return _size == 0;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T& operator[](size_t pos) IPADDRESS_NOEXCEPT {
        return _data[pos];
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const T& operator[](size_t pos) const IPADDRESS_NOEXCEPT {
        return _data[pos];
    }
};

template <size_t N, typename It>
IPADDRESS_NODISCARD IPADDRESS_CONSTEVAL IPADDRESS_FORCE_INLINE auto consteval_collapse_addresses(It first, It last, error_code& code) IPADDRESS_NOEXCEPT
    -> collapsed_addresses_vector<typename std::iterator_traits<It>::value_type, N> {
    using network_type = typename std::iterator_traits<It>::value_type;
    using address_type = typename network_type::ip_address_type;

    const auto version = first->version();
    const auto max_prefixlen = version == ip_version::V4
        ? ipv4_network::base_max_prefixlen
        : ipv6_network::base_max_prefixlen;

#if IPADDRESS_CPP_VERSION >= 20
    std::vector<network_type> nets;
    std::vector<address_type> ips;
    std::vector<std::pair<network_type, network_type>> subnets;
#else
    collapsed_addresses_vector<network_type, N> nets;
    collapsed_addresses_vector<address_type, N> ips;
    collapsed_addresses_vector<std::pair<network_type, network_type>, N> subnets;
#endif

    for (auto it = first; it != last; ++it) {
        const auto& net = *it;
        if (net.prefixlen() != max_prefixlen) {
            if (!nets.empty() && net.version() != version) {
                code = error_code::invalid_version;
                return {};
            }
            nets.emplace_back(net);
        } else {
            if (!ips.empty() && net.version() != version) {
                code = error_code::invalid_version;
                return {};
            }
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
        auto it = find_lower_bound(subnets.begin(), subnets.end(), supernet, [](const auto& item, const auto& key) {
            return item.first < key;
        });
        if (it != subnets.end() && it->first == supernet) {
            if (it->second != net) {
                subnets.erase(it);
                nets.emplace_back(supernet);
            }
        } else {
            subnets.emplace(it, supernet, net);
        }
    }
    collapsed_addresses_vector<network_type, N> result;
    if (!subnets.empty()) {
        auto it = subnets.begin();
        auto last = it->second;
        result.emplace_back(last);
        ++it;
        for (; it != subnets.end(); ++it) {
            if (last.broadcast_address() >= it->second.broadcast_address()) {
                continue;
            }
            last = it->second;
            result.emplace_back(last);
        }
    }
    return result;
}

template <typename Result, typename It>
IPADDRESS_NODISCARD IPADDRESS_CONSTEVAL IPADDRESS_FORCE_INLINE Result collapse_addresses(It first, It last, error_code& code) IPADDRESS_NOEXCEPT {
    using network_type = typename std::iterator_traits<It>::value_type;
    using address_type = typename network_type::ip_address_type;

    const auto version = first->version();
    const auto max_prefixlen = version == ip_version::V4
        ? ipv4_network::base_max_prefixlen
        : ipv6_network::base_max_prefixlen;

    std::vector<network_type> nets;
    std::set<address_type> ips;
    std::map<network_type, network_type> subnets;

    for (auto it = first; it != last; ++it) {
        const auto& net = *it;
        if (net.prefixlen() != max_prefixlen) {
            if (!nets.empty() && net.version() != version) {
                code = error_code::invalid_version;
                return {};
            }
            nets.emplace_back(net);
        } else {
            if (!ips.empty() && net.version() != version) {
                code = error_code::invalid_version;
                return {};
            }
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
    Result result;
    if (!subnets.empty()) {
        auto it = subnets.begin();
        auto last = it->second;
        result.emplace_back(last);
        ++it;
        for (; it != subnets.end(); ++it) {
            if (last.broadcast_address() >= it->second.broadcast_address()) {
                continue;
            }
            last = it->second;
            result.emplace_back(last);
        }
    }
    return result;
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

template <typename Net, size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(const std::array<Net, N>& nets, error_code& code) IPADDRESS_NOEXCEPT
    -> decltype(internal::consteval_collapse_addresses<N>(&nets[0], &nets[0] + nets.size(), code)) {
    using result_type = decltype(internal::consteval_collapse_addresses<N>(&nets[0], &nets[0] + nets.size(), code));
    return IPADDRESS_IS_CONST_EVALUATED(nets)
        ? internal::consteval_collapse_addresses<N>(&nets[0], &nets[0] + nets.size(), code)
        : internal::collapse_addresses<result_type>(&nets[0], &nets[0] + nets.size(), code);
}

template <typename Net, size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(const Net (&nets)[N], error_code& code) IPADDRESS_NOEXCEPT
    -> decltype(internal::consteval_collapse_addresses<N>(&nets[0], &nets[0] + N, code)) {
    using result_type = decltype(internal::consteval_collapse_addresses<N>(&nets[0], &nets[0] + N, code));
    return IPADDRESS_IS_CONST_EVALUATED(nets)
        ? internal::consteval_collapse_addresses<N>(&nets[0], &nets[0] + N, code)
        : internal::collapse_addresses<result_type>(&nets[0], &nets[0] + N, code);
}

template <typename... Nets, typename std::enable_if<internal::is_ip_network_types<Nets...>::value, bool>::type = true>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(error_code& code, const Nets&... nets) IPADDRESS_NOEXCEPT
    -> collapsed_addresses_vector<decltype(internal::ip_network_type_extract(nets...)), sizeof...(Nets)> {
    using Net = decltype(internal::ip_network_type_extract(nets...));
    const Net net_array[] = { Net(nets)... };
    return collapse_addresses(net_array, code);
}

template <typename It>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(It first, It last, error_code& code) IPADDRESS_NOEXCEPT
    -> std::vector<typename std::iterator_traits<It>::value_type> {
    return internal::collapse_addresses<std::vector<typename std::iterator_traits<It>::value_type>>(first, last, code);;
}

template <typename Net, size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(const std::array<Net, N>& nets) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS
    -> decltype(collapse_addresses(nets, *std::declval<error_code*>())) {
    error_code code = error_code::no_error;
    const auto result = collapse_addresses(nets, code);
    if (code != error_code::no_error) {
        raise_error(code, 0, "", 0);
    }
    return result;
}

template <typename Net, size_t N>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(const Net (&nets)[N]) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS
    -> decltype(collapse_addresses(nets, *std::declval<error_code*>())) {
    error_code code = error_code::no_error;
    const auto result = collapse_addresses(nets, code);
    if (code != error_code::no_error) {
        raise_error(code, 0, "", 0);
    }
    return result;
}

template <typename... Nets, typename std::enable_if<internal::is_ip_network_types<Nets...>::value, bool>::type = true>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(const Nets&... nets) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS
    -> collapsed_addresses_vector<decltype(internal::ip_network_type_extract(nets...)), sizeof...(Nets)> {
    error_code code = error_code::no_error;
    const auto result = collapse_addresses(code, nets...);
    if (code != error_code::no_error) {
        raise_error(code, 0, "", 0);
    }
    return result;
}

template <typename It>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE auto collapse_addresses(It first, It last) IPADDRESS_NOEXCEPT
    -> std::vector<typename std::iterator_traits<It>::value_type> {
    error_code code = error_code::no_error;
    const auto result = collapse_addresses(first, last, code);
    if (code != error_code::no_error) {
        raise_error(code, 0, "", 0);
    }
    return result;
}

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_FUNCTIONS_HPP
