#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

// #if IPADDRESS_CPP_VERSION >= 20
// #  include <ranges>
// #endif

using namespace testing;
using namespace ipaddress;

template <size_t N1, size_t N2>
static constexpr ipv4_network test_swap(const char(&str1)[N1], const char(&str2)[N2]) {
    auto net1 = ipv4_network::parse(str1);
    auto net2 = ipv4_network::parse(str2);
    net1.swap(net2);
    return net1;
}

template <size_t N>
static constexpr error_code test_error(const char(&str)[N]) noexcept {
    error_code err = error_code::NO_ERROR;
    ipv4_network::parse(str, err);
    return err;
}

TEST(ipv4_network, CompileTime) {
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    constexpr auto net1 = ipv4_network::parse<"127.0.0.0/8">();
    ASSERT_EQ(net1.network_address().to_uint(), 0x7F000000);
    ASSERT_EQ(net1.broadcast_address().to_uint(), 0x7FFFFFFF);
    ASSERT_EQ(net1.netmask().to_uint(), 0xFF000000);
    ASSERT_EQ(net1.hostmask().to_uint(), 0x00FFFFFF);
    ASSERT_EQ(net1.prefixlen(), 8);

    constexpr auto net2 = ipv4_network::parse<"127.0.0.1">();
    constexpr auto net2_address = net2.network_address().to_uint();
    constexpr auto net2_broadcast = net2.broadcast_address().to_uint();
    constexpr auto net2_netmask = net2.netmask().to_uint();
    constexpr auto net2_hostmask = net2.hostmask().to_uint();
    constexpr auto net2_prefixlen = net2.prefixlen();
    ASSERT_EQ(net2_address, 0x7F000001);
    ASSERT_EQ(net2_broadcast, 0x7F000001);
    ASSERT_EQ(net2_netmask, 0xFFFFFFFF);
    ASSERT_EQ(net2_hostmask, 0x00000000);
    ASSERT_EQ(net2_prefixlen, 32);

    constexpr auto net_wchar = ipv4_network::parse<L"127.0.0.1">();
    ASSERT_EQ(net_wchar.network_address().to_uint(), 0x7F000001);

    constexpr auto net_char16 = ipv4_network::parse<u"127.0.0.1">();
    ASSERT_EQ(net_char16.network_address().to_uint(), 0x7F000001);

    constexpr auto net_char32 = ipv4_network::parse<U"127.0.0.1">();
    ASSERT_EQ(net_char32.network_address().to_uint(), 0x7F000001);

#if __cpp_char8_t >= 201811L
    constexpr auto net_char8 = ipv4_network::parse<u8"127.0.0.1">();
    ASSERT_EQ(net_char8.network_address().to_uint(), 0x7F000001);
#endif // __cpp_char8_t >= 201811L
#endif

    constexpr auto net3 = ipv4_network::parse("127.0.0.0/8");
    constexpr auto net3_address = net3.network_address().to_uint();
    constexpr auto net3_broadcast = net3.broadcast_address().to_uint();
    constexpr auto net3_netmask = net3.netmask().to_uint();
    constexpr auto net3_hostmask = net3.hostmask().to_uint();
    constexpr auto net3_prefixlen = net3.prefixlen();
    constexpr auto net3_hash = net3.hash();
    ASSERT_EQ(net3_address, 0x7F000000);
    ASSERT_EQ(net3_broadcast, 0x7FFFFFFF);
    ASSERT_EQ(net3_netmask, 0xFF000000);
    ASSERT_EQ(net3_hostmask, 0x00FFFFFF);
    ASSERT_EQ(net3_prefixlen, 8);
    ASSERT_GT(net3_hash, size_t(0));

    constexpr auto net4 = test_swap("0.0.0.0/8", "127.0.0.0/16");
    constexpr auto net4_address = net4.network_address().to_uint();
    constexpr auto net4_broadcast = net4.broadcast_address().to_uint();
    constexpr auto net4_netmask = net4.netmask().to_uint();
    constexpr auto net4_hostmask = net4.hostmask().to_uint();
    constexpr auto net4_prefixlen = net4.prefixlen();
    constexpr auto net4_hash = net4.hash();
    ASSERT_EQ(net4_address, 0x7F000000);
    ASSERT_EQ(net4_broadcast, 0x7F00FFFF);
    ASSERT_EQ(net4_netmask, 0xFFFF0000);
    ASSERT_EQ(net4_hostmask, 0x0000FFFF);
    ASSERT_EQ(net4_prefixlen, 16);
    ASSERT_GT(net4_hash, size_t(0));

    constexpr auto net5_error = test_error("127.0.0.1/24");
    ASSERT_EQ(net5_error, error_code::HAS_HOST_BITS_SET);

    constexpr auto net_prefix = ipv4_network::parse("1.2.3.4/255.255.255.255");
    ASSERT_EQ(net_prefix, ipv4_network::parse("1.2.3.4/32"));
    
    constexpr auto b1 = net3 < net4;
    constexpr auto b2 = net3 > net4;
    constexpr auto b3 = net3 <= net4;
    constexpr auto b4 = net3 >= net4;
    constexpr auto b5 = net3 == net4;
    constexpr auto b6 = net3 != net4;
    ASSERT_TRUE(b1);
    ASSERT_FALSE(b2);
    ASSERT_TRUE(b3);
    ASSERT_FALSE(b4);
    ASSERT_FALSE(b5);
    ASSERT_TRUE(b6);

    constexpr auto net6 = "127.0.0.0/16"_ipv4_net;
    constexpr auto net7 = "127.128.128.255"_ipv4_net;
    ASSERT_EQ(net6, ipv4_network::parse("127.0.0.0/16"));
    ASSERT_EQ(net7, ipv4_network::parse("127.128.128.255/32"));

    constexpr auto net8 = ipv4_network::from_address(ipv4_address::parse("127.0.0.0"), 16);
    constexpr auto net9 = ipv4_network::from_address(ipv4_address::parse("127.0.0.0"), 16, false);
    ASSERT_EQ(net8, ipv4_network::parse("127.0.0.0/16"));
    ASSERT_EQ(net9, ipv4_network::parse("127.0.0.0/16"));

    constexpr auto net10 = ipv4_network::parse("224.1.1.0/31").is_multicast();
    constexpr auto net11 = ipv4_network::parse("240.0.0.0").is_multicast();
    ASSERT_TRUE(net10);
    ASSERT_FALSE(net11);

    constexpr auto net12 = ipv4_network::parse("0.0.0.0/8").is_private();
    constexpr auto net13 = ipv4_network::parse("0.0.0.0/0").is_private();
    ASSERT_TRUE(net12);
    ASSERT_FALSE(net13);

    constexpr auto net14 = ipv4_network::parse("192.0.3.0/24").is_global();
    constexpr auto net15 = ipv4_network::parse("100.64.0.0/10").is_global();
    ASSERT_TRUE(net14);
    ASSERT_FALSE(net15);

    constexpr auto net16 = ipv4_network::parse("240.0.0.1").is_reserved();
    constexpr auto net17 = ipv4_network::parse("239.255.255.255").is_reserved();
    ASSERT_TRUE(net16);
    ASSERT_FALSE(net17);

    constexpr auto net18 = ipv4_network::parse("127.42.0.0/24").is_loopback();
    constexpr auto net19 = ipv4_network::parse("128.0.0.0/8").is_loopback();
    ASSERT_TRUE(net18);
    ASSERT_FALSE(net19);

    constexpr auto net20 = ipv4_network::parse("169.254.1.0/24").is_link_local();
    constexpr auto net21 = ipv4_network::parse("169.255.100.200").is_link_local();
    ASSERT_TRUE(net20);
    ASSERT_FALSE(net21);

    constexpr auto net22 = ipv4_network::parse("0.0.0.0/32").is_unspecified();
    constexpr auto net23 = ipv4_network::parse("0.0.0.0/8").is_unspecified();
    ASSERT_TRUE(net22);
    ASSERT_FALSE(net23);

    constexpr auto contains = ipv4_network::parse("192.0.2.0/28").contains(ipv4_address::parse("192.0.2.6"));
    ASSERT_TRUE(contains);

    constexpr auto overlaps = ipv4_network::parse("1.2.3.0/24").overlaps(ipv4_network::parse("1.2.3.0/30"));
    ASSERT_TRUE(overlaps);

    constexpr auto subnet_of = ipv4_network::parse("10.0.0.0/30").subnet_of(ipv4_network::parse("10.0.0.0/24"));
    ASSERT_TRUE(subnet_of);

    constexpr auto supernet_of = ipv4_network::parse("192.168.1.0/24").supernet_of(ipv4_network::parse("192.168.1.128/30"));
    ASSERT_TRUE(supernet_of);

    constexpr auto addresses_count = ipv4_network::parse("10.0.0.0/30").addresses_count();
    ASSERT_EQ(addresses_count, 4);

    constexpr auto supernet = ipv4_network::parse("192.0.2.0/24").supernet();
    ASSERT_EQ(supernet, ipv4_network::parse("192.0.2.0/23"));

    constexpr auto hosts_sequence = ipv4_network::parse("192.0.2.0/29").hosts();
    constexpr auto hosts_empty = hosts_sequence.empty();
    constexpr auto hosts_size = hosts_sequence.size();
    constexpr auto hosts_front = hosts_sequence.front();
    constexpr auto hosts_back = hosts_sequence.back();
    constexpr auto hosts_at_0 = hosts_sequence.at(0);
    constexpr auto hosts_at_1 = hosts_sequence[1];
    constexpr auto hosts_at_2 = hosts_sequence.at(2);
    constexpr auto hosts_at_3 = hosts_sequence[3];
    constexpr auto hosts_begin = hosts_sequence.begin();
    constexpr auto hosts_end = hosts_sequence.end();
    constexpr auto hosts_rbegin = hosts_sequence.rbegin();
    constexpr auto hosts_rend = hosts_sequence.rend();
    constexpr auto hosts_diff = hosts_end.uint_diff(hosts_begin);
    constexpr auto hosts_begin_0 = *hosts_begin;
    constexpr auto hosts_begin_0_uint = hosts_begin->to_uint();
    constexpr auto hosts_begin_at_0 = hosts_begin[0U];
    constexpr auto hosts_begin_at_1 = hosts_begin[1U];
    constexpr auto hosts_begin_at_2 = hosts_begin[2U];
    constexpr auto hosts_begin_at_3 = hosts_begin[3U];
    constexpr auto hosts_it = *(--(++hosts_sequence.begin()++ + 2U) -= 1U);
    constexpr auto hosts_it_eq = hosts_begin == hosts_end;
    constexpr auto hosts_it_ne = hosts_begin != hosts_end;
    constexpr auto hosts_it_ls = hosts_begin < hosts_end;
    constexpr auto hosts_it_le = hosts_begin <= hosts_end;
    constexpr auto hosts_it_gt = hosts_begin > hosts_end;
    constexpr auto hosts_it_ge = hosts_begin >= hosts_end;
    constexpr auto hosts_rdiff = hosts_rend.uint_diff(hosts_rbegin);
    constexpr auto hosts_rbegin_0 = *hosts_rbegin;
    constexpr auto hosts_rbegin_0_uint = hosts_rbegin->to_uint();
    constexpr auto hosts_rbegin_at_0 = hosts_rbegin[0U];
    constexpr auto hosts_rbegin_at_1 = hosts_rbegin[1U];
    constexpr auto hosts_rbegin_at_2 = hosts_rbegin[2U];
    constexpr auto hosts_rbegin_at_3 = hosts_rbegin[3U];
    constexpr auto hosts_rit = *(--(++hosts_sequence.rbegin()++ + 2U) -= 1U);
    constexpr auto hosts_rit_eq = hosts_rbegin == hosts_rend;
    constexpr auto hosts_rit_ne = hosts_rbegin != hosts_rend;
    constexpr auto hosts_rit_ls = hosts_rbegin < hosts_rend;
    constexpr auto hosts_rit_le = hosts_rbegin <= hosts_rend;
    constexpr auto hosts_rit_gt = hosts_rbegin > hosts_rend;
    constexpr auto hosts_rit_ge = hosts_rbegin >= hosts_rend;
    ASSERT_FALSE(hosts_empty);
    ASSERT_EQ(hosts_size, 6);
    ASSERT_EQ(hosts_front, ipv4_address::parse("192.0.2.1"));
    ASSERT_EQ(hosts_back, ipv4_address::parse("192.0.2.6"));
    ASSERT_EQ(hosts_at_0, ipv4_address::parse("192.0.2.1"));
    ASSERT_EQ(hosts_at_1, ipv4_address::parse("192.0.2.2"));
    ASSERT_EQ(hosts_at_2, ipv4_address::parse("192.0.2.3"));
    ASSERT_EQ(hosts_at_3, ipv4_address::parse("192.0.2.4"));
    ASSERT_EQ(hosts_diff, 6);
    ASSERT_EQ(hosts_begin_0, ipv4_address::parse("192.0.2.1"));
    ASSERT_EQ(hosts_begin_0_uint, 0xC0000201);
    ASSERT_EQ(hosts_begin_at_0, ipv4_address::parse("192.0.2.1"));
    ASSERT_EQ(hosts_begin_at_1, ipv4_address::parse("192.0.2.2"));
    ASSERT_EQ(hosts_begin_at_2, ipv4_address::parse("192.0.2.3"));
    ASSERT_EQ(hosts_begin_at_3, ipv4_address::parse("192.0.2.4"));
    ASSERT_EQ(hosts_it, ipv4_address::parse("192.0.2.2"));
    ASSERT_FALSE(hosts_it_eq);
    ASSERT_TRUE(hosts_it_ne);
    ASSERT_TRUE(hosts_it_ls);
    ASSERT_TRUE(hosts_it_le);
    ASSERT_FALSE(hosts_it_gt);
    ASSERT_FALSE(hosts_it_ge);
    ASSERT_EQ(hosts_rdiff, 6);
    ASSERT_EQ(hosts_rbegin_0, ipv4_address::parse("192.0.2.6"));
    ASSERT_EQ(hosts_rbegin_0_uint, 0xC0000206);
    ASSERT_EQ(hosts_rbegin_at_0, ipv4_address::parse("192.0.2.6"));
    ASSERT_EQ(hosts_rbegin_at_1, ipv4_address::parse("192.0.2.5"));
    ASSERT_EQ(hosts_rbegin_at_2, ipv4_address::parse("192.0.2.4"));
    ASSERT_EQ(hosts_rbegin_at_3, ipv4_address::parse("192.0.2.3"));
    ASSERT_EQ(hosts_rit, ipv4_address::parse("192.0.2.5"));
    ASSERT_FALSE(hosts_rit_eq);
    ASSERT_TRUE(hosts_rit_ne);
    ASSERT_TRUE(hosts_rit_ls);
    ASSERT_TRUE(hosts_rit_le);
    ASSERT_FALSE(hosts_rit_gt);
    ASSERT_FALSE(hosts_rit_ge);

// #if IPADDRESS_CPP_VERSION >= 20
//     constexpr auto c_hosts_sequence = ipv4_network::parse("192.0.2.0/29").hosts();
//     constexpr auto host_range = *++((c_hosts_sequence
//         | std::views::filter([](const auto& a) { return a.to_uint() % 2 == 0; })
//         | std::views::take(2)).begin());
//     ASSERT_EQ(host_range, ipv4_address::parse("192.0.2.4"));
// #endif

    constexpr auto subnets_sequence = ipv4_network::parse("192.0.2.0/24").subnets(2);
    constexpr auto subnets_empty = subnets_sequence.empty();
    constexpr auto subnets_size = subnets_sequence.size();
    constexpr auto subnets_front = subnets_sequence.front();
    constexpr auto subnets_back = subnets_sequence.back();
    constexpr auto subnets_at_0 = subnets_sequence.at(0);
    constexpr auto subnets_at_1 = subnets_sequence[1];
    constexpr auto subnets_at_2 = subnets_sequence.at(2);
    constexpr auto subnets_at_3 = subnets_sequence[3];
    constexpr auto subnets_begin = subnets_sequence.begin();
    constexpr auto subnets_end = subnets_sequence.end();
    constexpr auto subnets_rbegin = subnets_sequence.rbegin();
    constexpr auto subnets_rend = subnets_sequence.rend();
    constexpr auto subnets_diff = subnets_end.uint_diff(subnets_begin);
    constexpr auto subnets_begin_0 = *subnets_begin;
    constexpr auto subnets_begin_0_uint = subnets_begin->network_address().to_uint();
    constexpr auto subnets_begin_at_0 = subnets_begin[0U];
    constexpr auto subnets_begin_at_1 = subnets_begin[1U];
    constexpr auto subnets_begin_at_2 = subnets_begin[2U];
    constexpr auto subnets_begin_at_3 = subnets_begin[3U];
    constexpr auto subnets_it = *(--(++subnets_sequence.begin()++ + 2U) -= 1U);
    constexpr auto subnets_it_eq = subnets_begin == subnets_end;
    constexpr auto subnets_it_ne = subnets_begin != subnets_end;
    constexpr auto subnets_it_ls = subnets_begin < subnets_end;
    constexpr auto subnets_it_le = subnets_begin <= subnets_end;
    constexpr auto subnets_it_gt = subnets_begin > subnets_end;
    constexpr auto subnets_it_ge = subnets_begin >= subnets_end;
    constexpr auto subnets_rdiff = subnets_rend.uint_diff(subnets_rbegin);
    constexpr auto subnets_rbegin_0 = *subnets_rbegin;
    constexpr auto subnets_rbegin_0_uint = subnets_rbegin->network_address().to_uint();
    constexpr auto subnets_rbegin_at_0 = subnets_rbegin[0U];
    constexpr auto subnets_rbegin_at_1 = subnets_rbegin[1U];
    constexpr auto subnets_rbegin_at_2 = subnets_rbegin[2U];
    constexpr auto subnets_rbegin_at_3 = subnets_rbegin[3U];
    constexpr auto subnets_rit = *(--(++subnets_sequence.rbegin()++ + 2U) -= 1U);
    constexpr auto subnets_rit_eq = subnets_rbegin == subnets_rend;
    constexpr auto subnets_rit_ne = subnets_rbegin != subnets_rend;
    constexpr auto subnets_rit_ls = subnets_rbegin < subnets_rend;
    constexpr auto subnets_rit_le = subnets_rbegin <= subnets_rend;
    constexpr auto subnets_rit_gt = subnets_rbegin > subnets_rend;
    constexpr auto subnets_rit_ge = subnets_rbegin >= subnets_rend;
    ASSERT_FALSE(subnets_empty);
    ASSERT_EQ(subnets_size, 4);
    ASSERT_EQ(subnets_front, ipv4_network::parse("192.0.2.0/26"));
    ASSERT_EQ(subnets_back, ipv4_network::parse("192.0.2.192/26"));
    ASSERT_EQ(subnets_at_0, ipv4_network::parse("192.0.2.0/26"));
    ASSERT_EQ(subnets_at_1, ipv4_network::parse("192.0.2.64/26"));
    ASSERT_EQ(subnets_at_2, ipv4_network::parse("192.0.2.128/26"));
    ASSERT_EQ(subnets_at_3, ipv4_network::parse("192.0.2.192/26"));
    ASSERT_EQ(subnets_diff, 4);
    ASSERT_EQ(subnets_begin_0, ipv4_network::parse("192.0.2.0/26"));
    ASSERT_EQ(subnets_begin_0_uint, 0xC0000200);
    ASSERT_EQ(subnets_begin_at_0, ipv4_network::parse("192.0.2.0/26"));
    ASSERT_EQ(subnets_begin_at_1, ipv4_network::parse("192.0.2.64/26"));
    ASSERT_EQ(subnets_begin_at_2, ipv4_network::parse("192.0.2.128/26"));
    ASSERT_EQ(subnets_begin_at_3, ipv4_network::parse("192.0.2.192/26"));
    ASSERT_EQ(subnets_it, ipv4_network::parse("192.0.2.64/26"));
    ASSERT_FALSE(subnets_it_eq);
    ASSERT_TRUE(subnets_it_ne);
    ASSERT_TRUE(subnets_it_ls);
    ASSERT_TRUE(subnets_it_le);
    ASSERT_FALSE(subnets_it_gt);
    ASSERT_FALSE(subnets_it_ge);
    ASSERT_EQ(subnets_rdiff, 4);
    ASSERT_EQ(subnets_rbegin_0, ipv4_network::parse("192.0.2.192/26"));
    ASSERT_EQ(subnets_rbegin_0_uint, 0xC00002C0);
    ASSERT_EQ(subnets_rbegin_at_0, ipv4_network::parse("192.0.2.192/26"));
    ASSERT_EQ(subnets_rbegin_at_1, ipv4_network::parse("192.0.2.128/26"));
    ASSERT_EQ(subnets_rbegin_at_2, ipv4_network::parse("192.0.2.64/26"));
    ASSERT_EQ(subnets_rbegin_at_3, ipv4_network::parse("192.0.2.0/26"));
    ASSERT_EQ(subnets_rit, ipv4_network::parse("192.0.2.128/26"));
    ASSERT_FALSE(subnets_rit_eq);
    ASSERT_TRUE(subnets_rit_ne);
    ASSERT_TRUE(subnets_rit_ls);
    ASSERT_TRUE(subnets_rit_le);
    ASSERT_FALSE(subnets_rit_gt);
    ASSERT_FALSE(subnets_rit_ge);
    
// #if IPADDRESS_CPP_VERSION >= 20
//     constexpr auto c_subnets_sequence = ipv4_network::parse("192.0.2.0/24").subnets(2);
//     constexpr auto subnets_range = *++((c_subnets_sequence
//         | std::views::filter([](const auto& n) { return n.network_address().to_uint() % 2 == 0; })
//         | std::views::take(2)).begin());
//     ASSERT_EQ(subnets_range, ipv4_network::parse("192.0.2.64/26"));
// #endif

    constexpr auto exclude_sequence = ipv4_network::parse("192.0.2.0/28").address_exclude(ipv4_network::parse("192.0.2.1/32"));
    constexpr auto exclude_empty = exclude_sequence.empty();
    constexpr auto exclude_begin = exclude_sequence.begin();
    constexpr auto exclude_end = exclude_sequence.end();
    constexpr auto exclude_begin_0 = *exclude_begin;
    constexpr auto exclude_begin_0_uint = exclude_begin->network_address().to_uint();
    constexpr auto exclude_begin_1 = *++exclude_sequence.begin();
    ASSERT_FALSE(exclude_empty);
    ASSERT_EQ(exclude_begin_0, ipv4_network::parse("192.0.2.8/29"));
    ASSERT_EQ(exclude_begin_0_uint, 0xC0000208);
    ASSERT_EQ(exclude_begin_1, ipv4_network::parse("192.0.2.4/30"));
    constexpr auto exclude_it_eq = exclude_begin == exclude_end;
    constexpr auto exclude_it_ne = exclude_begin != exclude_end;
    constexpr auto exclude_it_ls = exclude_begin < exclude_end;
    constexpr auto exclude_it_le = exclude_begin <= exclude_end;
    constexpr auto exclude_it_gt = exclude_begin > exclude_end;
    constexpr auto exclude_it_ge = exclude_begin >= exclude_end;
    ASSERT_FALSE(exclude_it_eq);
    ASSERT_TRUE(exclude_it_ne);
    ASSERT_TRUE(exclude_it_ls);
    ASSERT_TRUE(exclude_it_le);
    ASSERT_FALSE(exclude_it_gt);
    ASSERT_FALSE(exclude_it_ge);

// #if IPADDRESS_CPP_VERSION >= 20
//     constexpr auto c_exclude_sequence = ipv4_network::parse("192.0.2.0/28").address_exclude(ipv4_network::parse("192.0.2.1/32"));
//     constexpr auto exclude_range = *++((c_exclude_sequence
//         | std::views::filter([](const auto& n) { return n.network_address().to_uint() % 2 == 0; })
//         | std::views::take(2)).begin());
//     ASSERT_EQ(exclude_range, ipv4_network::parse("192.0.2.4/30"));
// #endif

    constexpr auto net_wchar_2 = ipv4_network::parse(L"127.0.0.1");
    ASSERT_EQ(net_wchar_2.network_address().to_uint(), 0x7F000001);

    constexpr auto net_char16_2 = ipv4_network::parse(u"127.0.0.1");
    ASSERT_EQ(net_char16_2.network_address().to_uint(), 0x7F000001);

    constexpr auto net_char32_2 = ipv4_network::parse(U"127.0.0.1");
    ASSERT_EQ(net_char32_2.network_address().to_uint(), 0x7F000001);

#if __cpp_char8_t >= 201811L
    constexpr auto net_char8_2 = ipv4_network::parse(u8"127.0.0.1");
    ASSERT_EQ(net_char8_2.network_address().to_uint(), 0x7F000001);
#endif // __cpp_char8_t >= 201811L
}

TEST(ipv4_network, DefaultCtor) {
    ipv4_network net;
    
    EXPECT_EQ(net.network_address(), ipv4_address::from_uint<0>());
    ASSERT_EQ(net.broadcast_address(), ipv4_address::from_uint<0>());
    EXPECT_EQ(net.netmask(), ipv4_address::from_uint<0xFFFFFFFF>());
    EXPECT_EQ(net.hostmask(), ipv4_address::from_uint<0>());
    EXPECT_EQ(net.prefixlen(), 32);
    EXPECT_EQ(net.version(), ip_version::V4);
}

TEST(ipv4_network, CopyCtor) {
    auto net = ipv4_network::parse("127.0.0.0/8");
    auto net_copy = net;
    
    EXPECT_EQ(net_copy.network_address(), ipv4_address::from_uint<0x7F000000>());
    ASSERT_EQ(net_copy.broadcast_address(), ipv4_address::from_uint<0x7FFFFFFF>());
    EXPECT_EQ(net_copy.netmask(), ipv4_address::from_uint<0xFF000000>());
    EXPECT_EQ(net_copy.hostmask(), ipv4_address::from_uint<0x00FFFFFF>());
    EXPECT_EQ(net_copy.prefixlen(), 8);
}

TEST(ipv4_network, CopyOperator) {
    auto net = ipv4_network::parse("127.0.0.0/8");
    ipv4_network net_copy;
    
    EXPECT_EQ(net_copy.network_address(), ipv4_address::from_uint<0>());
    ASSERT_EQ(net_copy.broadcast_address(), ipv4_address::from_uint<0>());
    EXPECT_EQ(net_copy.netmask(), ipv4_address::from_uint<0xFFFFFFFF>());
    EXPECT_EQ(net_copy.hostmask(), ipv4_address::from_uint<0>());
    EXPECT_EQ(net_copy.prefixlen(), 32);
    net_copy = net;
    
    EXPECT_EQ(net_copy.network_address(), ipv4_address::from_uint<0x7F000000>());
    ASSERT_EQ(net_copy.broadcast_address(), ipv4_address::from_uint<0x7FFFFFFF>());
    EXPECT_EQ(net_copy.netmask(), ipv4_address::from_uint<0xFF000000>());
    EXPECT_EQ(net_copy.hostmask(), ipv4_address::from_uint<0x00FFFFFF>());
    EXPECT_EQ(net_copy.prefixlen(), 8);
}

TEST(ipv4_network, parse_utf) {
    auto str1 = L"127.0.0.0/24";
    auto net_wchar = ipv4_network::parse(str1);
    ASSERT_EQ(net_wchar.network_address().to_uint(), 0x7F000000);
    ASSERT_EQ(net_wchar.prefixlen(), 24);

    auto str2 = u"127.0.0.0/24";
    auto net_char16 = ipv4_network::parse(str2);
    ASSERT_EQ(net_char16.network_address().to_uint(), 0x7F000000);
    ASSERT_EQ(net_char16.prefixlen(), 24);

    auto str3 = U"127.0.0.0/24";
    auto net_char32 = ipv4_network::parse(str3);
    ASSERT_EQ(net_char32.network_address().to_uint(), 0x7F000000);
    ASSERT_EQ(net_char32.prefixlen(), 24);

#if __cpp_char8_t >= 201811L
    auto str4 = u8"127.0.0.0/24";
    auto net_char8 = ipv4_network::parse(str4);
    ASSERT_EQ(net_char8.network_address().to_uint(), 0x7F000000);
    ASSERT_EQ(net_char8.prefixlen(), 24);
#endif // __cpp_char8_t >= 201811L
}

using NetworkParserIpv4Params = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv4Params, parse) {
    auto excepted_address = ipv4_address::parse(get<1>(GetParam()));
    auto excepted_broadcast = ipv4_address::parse(get<2>(GetParam()));
    auto excepted_netmask = ipv4_address::parse(get<3>(GetParam()));
    auto excepted_hostmask = ipv4_address::parse(get<4>(GetParam()));
    auto excepted_prefixlen = get<5>(GetParam());

    auto net = ipv4_network::parse(get<0>(GetParam()));
   
    EXPECT_EQ(net.network_address(), excepted_address);
    ASSERT_EQ(net.broadcast_address(), excepted_broadcast);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv4_network net_from_stream;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> net_from_stream >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    EXPECT_EQ(net_from_stream.network_address(), excepted_address);
    ASSERT_EQ(net_from_stream.broadcast_address(), excepted_broadcast);
    EXPECT_EQ(net_from_stream.netmask(), excepted_netmask);
    EXPECT_EQ(net_from_stream.hostmask(), excepted_hostmask);
    EXPECT_EQ(net_from_stream.prefixlen(), excepted_prefixlen);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, NetworkParserIpv4Params,
    Values(
        std::make_tuple("1.2.3.4", "1.2.3.4", "1.2.3.4", "255.255.255.255", "0.0.0.0", 32),
        std::make_tuple("1.2.3.4/32", "1.2.3.4", "1.2.3.4", "255.255.255.255", "0.0.0.0", 32),
        std::make_tuple("1.2.3.4/255.255.255.255", "1.2.3.4", "1.2.3.4", "255.255.255.255", "0.0.0.0", 32),
        std::make_tuple("192.0.2.0/24", "192.0.2.0", "192.0.2.255", "255.255.255.0", "0.0.0.255", 24),
        std::make_tuple("192.0.2.0/255.255.255.0", "192.0.2.0", "192.0.2.255", "255.255.255.0", "0.0.0.255", 24),
        std::make_tuple("192.0.2.0/0.0.0.255", "192.0.2.0", "192.0.2.255", "255.255.255.0", "0.0.0.255", 24),
        std::make_tuple("192.0.2.0/27", "192.0.2.0", "192.0.2.31", "255.255.255.224", "0.0.0.31", 27),
        std::make_tuple("192.0.2.0/255.255.255.224", "192.0.2.0", "192.0.2.31", "255.255.255.224", "0.0.0.31", 27)
    ));

using NetworkParserIpv4NotStrictParams = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv4NotStrictParams, parse_non_strict) {
    auto excepted_address = ipv4_address::parse(get<1>(GetParam()));
    auto excepted_broadcast = ipv4_address::parse(get<2>(GetParam()));
    auto excepted_netmask = ipv4_address::parse(get<3>(GetParam()));
    auto excepted_hostmask = ipv4_address::parse(get<4>(GetParam()));
    auto excepted_prefixlen = get<5>(GetParam());

    auto net = ipv4_network::parse(get<0>(GetParam()), false);
   
    EXPECT_EQ(net.network_address(), excepted_address);
    ASSERT_EQ(net.broadcast_address(), excepted_broadcast);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv4_network net_from_stream;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> non_strict >> net_from_stream >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    EXPECT_EQ(net_from_stream.network_address(), excepted_address);
    ASSERT_EQ(net_from_stream.broadcast_address(), excepted_broadcast);
    EXPECT_EQ(net_from_stream.netmask(), excepted_netmask);
    EXPECT_EQ(net_from_stream.hostmask(), excepted_hostmask);
    EXPECT_EQ(net_from_stream.prefixlen(), excepted_prefixlen);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, NetworkParserIpv4NotStrictParams,
    Values(
        std::make_tuple("1.2.3.4/24", "1.2.3.0", "1.2.3.255", "255.255.255.0", "0.0.0.255", 24),
        std::make_tuple("192.0.2.0/255.255.0.0", "192.0.0.0", "192.0.255.255", "255.255.0.0", "0.0.255.255", 16)
    ));

using NetworkFromAddressIpv4Params = TestWithParam<std::tuple<const char*, const char*, size_t, bool>>;
TEST_P(NetworkFromAddressIpv4Params, from_address) {
    auto address = ipv4_address::parse(get<1>(GetParam()));
    auto prefixlen = get<2>(GetParam());
    auto strict = get<3>(GetParam());

    auto expected = ipv4_network::parse(get<0>(GetParam()), strict);

    error_code err = error_code::NO_ERROR;
    auto actual = ipv4_network::from_address(address, err, prefixlen, strict);
    ASSERT_EQ(actual, expected);
    ASSERT_EQ(actual.network_address(), expected.network_address());
    ASSERT_EQ(actual.broadcast_address(), expected.broadcast_address());
    ASSERT_EQ(actual.netmask(), expected.netmask());
    ASSERT_EQ(actual.hostmask(), expected.hostmask());
    ASSERT_EQ(actual.prefixlen(), expected.prefixlen());

    auto actual2 = ipv4_network::from_address(address, prefixlen, strict);
    ASSERT_EQ(actual2, expected);
    ASSERT_EQ(actual2.network_address(), expected.network_address());
    ASSERT_EQ(actual2.broadcast_address(), expected.broadcast_address());
    ASSERT_EQ(actual2.netmask(), expected.netmask());
    ASSERT_EQ(actual2.hostmask(), expected.hostmask());
    ASSERT_EQ(actual2.prefixlen(), expected.prefixlen());
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, NetworkFromAddressIpv4Params,
    Values(
        std::make_tuple("1.2.3.4/24", "1.2.3.4", 24, false),
        std::make_tuple("192.0.2.0/255.255.255.0", "192.0.2.0", 24, true)
    ));

TEST(ipv4_network, from_address_error) {
    error_code err = error_code::NO_ERROR;
    auto actual = ipv4_network::from_address(ipv4_address::parse("127.0.0.1"), err, 24);
    ASSERT_EQ(err, error_code::HAS_HOST_BITS_SET);
    ASSERT_EQ(actual.network_address(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(actual.broadcast_address(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(actual.netmask(), ipv4_address::parse("255.255.255.255"));
    ASSERT_EQ(actual.hostmask(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(actual.prefixlen(), 32);
    
#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_network = ipv4_network::from_address(ipv4_address::parse("127.0.0.1"), 24);
    ASSERT_EQ(error_network.network_address(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(error_network.netmask(), ipv4_address::parse("255.255.255.255"));
    ASSERT_EQ(error_network.hostmask(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(error_network.prefixlen(), 32);
#else
    EXPECT_THAT(
        []() { ipv4_network::from_address(ipv4_address::parse("127.0.0.1"), 24); },
        ThrowsMessage<parse_error>(StrEq("has host bits set in address 127.0.0.1")));
#endif
}

using InvalidNetworkIpv4Params = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidNetworkIpv4Params, parse) {
    auto expected_network = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err;
    ipv4_network::parse(expected_network, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_network = ipv4_network::parse(expected_network);

    EXPECT_EQ(error_network.network_address().to_uint(), 0x00000000);
    ASSERT_EQ(error_network.netmask().to_uint(), 0xFFFFFFFF);
    ASSERT_EQ(error_network.hostmask().to_uint(), 0x00000000);
    ASSERT_EQ(error_network.prefixlen(), 32);
#else
    EXPECT_THAT(
        [network=expected_network]() { ipv4_network::parse(network); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
    EXPECT_THAT(
        [network=expected_network]() { ipv4_network::parse(network); },
        Throws<parse_error>(Property(&parse_error::code, Eq(expected_error_code))));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, InvalidNetworkIpv4Params,
    Values(
        std::make_tuple("1.2.3.4/", error_code::EMPTY_NETMASK, "empty mask in address 1.2.3.4/"),
        std::make_tuple("1.2.3.4/33", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/33"),
        std::make_tuple("1.2.3.4/255.0.0.256", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/255.0.0.256"),
        std::make_tuple("1.2.3.4/-1", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/-1"),
        std::make_tuple("1.2.3.4/+1", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/+1"),
        std::make_tuple("1.2.3.4/ 1 ", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/ 1 "),
        std::make_tuple("1.2.3.4/0x1", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/0x1"),
        std::make_tuple("1.2.3.4/254.254.255.256", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/254.254.255.256"),
        std::make_tuple("1.2.3.4/1.a.2.3", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/1.a.2.3"),
        std::make_tuple("1.1.1.1/254.xyz.2.3", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.1.1.1/254.xyz.2.3"),
        std::make_tuple("1.1.1.1/pudding", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.1.1.1/pudding"),
        std::make_tuple("1.1.1.1/::", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.1.1.1/::"),

        std::make_tuple("1.2.3.4/255.255.255.127", error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES, "netmask pattern mixes zeroes & ones in address 1.2.3.4/255.255.255.127"),
        std::make_tuple("1.2.3.4/255.255.255.251", error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES, "netmask pattern mixes zeroes & ones in address 1.2.3.4/255.255.255.251"),
        std::make_tuple("1.1.1.1/240.255.0.0", error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES, "netmask pattern mixes zeroes & ones in address 1.1.1.1/240.255.0.0"),
        std::make_tuple("1.1.1.1/255.254.128.0", error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES, "netmask pattern mixes zeroes & ones in address 1.1.1.1/255.254.128.0"),
        std::make_tuple("1.1.1.1/0.1.127.255", error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES, "netmask pattern mixes zeroes & ones in address 1.1.1.1/0.1.127.255"),

        std::make_tuple("1.2.3.4/24", error_code::HAS_HOST_BITS_SET, "has host bits set in address 1.2.3.4/24"),
        std::make_tuple("1.2.3.4//", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 1.2.3.4//"),
        std::make_tuple("1.2.3.4//32", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 1.2.3.4//32"),
        std::make_tuple("1.2.3.4/32/24", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 1.2.3.4/32/24"),

        std::make_tuple("/", error_code::EMPTY_NETMASK, "empty mask in address /"),
        std::make_tuple("/8", error_code::EMPTY_ADDRESS, "address cannot be empty"),
        std::make_tuple("bogus", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address bogus has invalid symbol"),
        std::make_tuple("example.com", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address example.com has invalid symbol"),
        std::make_tuple("10/8", error_code::EXPECTED_4_OCTETS, "expected 4 octets in 10/8"),
        std::make_tuple("::1.2.3.4", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address ::1.2.3.4 has invalid symbol"),
        std::make_tuple("1.2.3.256", error_code::OCTET_EXCEEDED_255, "octet 0 of address 1.2.3.256 exceeded 255")
    ));

TEST(ipv4_network, Comparison) {
    auto net1 = ipv4_network::parse("127.240.1.0/24");
    auto net2 = ipv4_network::parse("127.240.1.0");
    auto net3 = ipv4_network::parse("127.240.1.0/32");
    
    ASSERT_TRUE(net1 < net2);
    ASSERT_TRUE(net1 <= net2);
    ASSERT_FALSE(net1 > net2);
    ASSERT_FALSE(net1 >= net2);
    ASSERT_FALSE(net1 == net2);
    ASSERT_TRUE(net1 != net2);
    
    ASSERT_FALSE(net2 < net1);
    ASSERT_FALSE(net2 <= net1);
    ASSERT_TRUE(net2 > net1);
    ASSERT_TRUE(net2 >= net1);
    ASSERT_FALSE(net2 == net1);
    ASSERT_TRUE(net2 != net1);
    
    ASSERT_FALSE(net2 < net3);
    ASSERT_TRUE(net2 <= net3);
    ASSERT_FALSE(net2 > net3);
    ASSERT_TRUE(net2 >= net3);
    ASSERT_TRUE(net2 == net3);
    ASSERT_FALSE(net2 != net3);
    
    ASSERT_FALSE(net3 < net2);
    ASSERT_TRUE(net3 <= net2);
    ASSERT_FALSE(net3 > net2);
    ASSERT_TRUE(net3 >= net2);
    ASSERT_TRUE(net3 == net2);
    ASSERT_FALSE(net3 != net2);
}

using ToStringNetworkIpv4Params = TestWithParam<std::tuple<const char*, const char*>>;
TEST_P(ToStringNetworkIpv4Params, to_string) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_network::parse(std::get<0>(GetParam()));

    std::ostringstream ss;
    ss << actual;

    ASSERT_EQ(actual.to_string(), std::string(expected));
    ASSERT_EQ(std::string(actual), std::string(expected));
    ASSERT_EQ(std::to_string(actual), std::string(expected));
    ASSERT_EQ(ss.str(), std::string(expected));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, ToStringNetworkIpv4Params,
    Values(
        std::make_tuple("1.2.3.4", "1.2.3.4/32"),
        std::make_tuple("1.2.3.4/32", "1.2.3.4/32"),
        std::make_tuple("1.2.3.4/255.255.255.255", "1.2.3.4/32")
    ));

TEST(ipv4_network, Hash) {
    auto net1 = ipv4_network::parse("127.0.0.1");
    auto net2 = ipv4_network::parse("127.0.0.1/32");
    auto net3 = ipv4_network::parse("127.0.0.0/24");
    auto net4 = ipv4_network::parse("127.0.0.0/16");
    auto hash_functor = std::hash<ipv4_network>{};

    ASSERT_EQ(net1.hash(), sizeof(size_t) == 8 ? 605929542755200525ULL : 2256296057U);
    ASSERT_EQ(net2.hash(), sizeof(size_t) == 8 ? 605929542755200525ULL : 2256296057U);
    ASSERT_EQ(net3.hash(), sizeof(size_t) == 8 ? 7623195473821216247ULL : 546525844U);
    ASSERT_EQ(net4.hash(), sizeof(size_t) == 8 ? 5503161050249481481ULL : 2858631644U);

    ASSERT_EQ(hash_functor(net1), sizeof(size_t) == 8 ? 605929542755200525ULL : 2256296057U);
    ASSERT_EQ(hash_functor(net2), sizeof(size_t) == 8 ? 605929542755200525ULL : 2256296057U);
    ASSERT_EQ(hash_functor(net3), sizeof(size_t) == 8 ? 7623195473821216247ULL : 546525844U);
    ASSERT_EQ(hash_functor(net4), sizeof(size_t) == 8 ? 5503161050249481481ULL : 2858631644U);
}

TEST(ipv4_network, Containers) {
    auto net1 = ipv4_network::parse("127.0.0.0");
    auto net2 = ipv4_network::parse("127.0.0.0/24");
    auto net3 = ipv4_network::parse("127.0.0.0/16");

    std::vector<ipv4_network> vec;
    vec.push_back(net1);
    vec.push_back(net2);
    vec.emplace_back(net3);
    ASSERT_EQ(vec[0], net1);
    ASSERT_EQ(vec[1], net2);
    ASSERT_EQ(vec[2], net3);

    std::map<ipv4_network, int> map;
    map[net2] = 2;
    map[net1] = 1;
    map[net3] = 3;
    auto it = map.begin();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(it++->first, net3);
    ASSERT_EQ(it++->first, net2);
    ASSERT_EQ(it++->first, net1);
    
    std::unordered_map<ipv4_network, int> unordered_map;
    unordered_map[net2] = 2;
    unordered_map[net1] = 1;
    unordered_map[net3] = 3;
    unordered_map[net3] = 4;
    ASSERT_EQ(unordered_map.size(), 3);
    ASSERT_EQ(unordered_map[net1], 1);
    ASSERT_EQ(unordered_map[net2], 2);
    ASSERT_EQ(unordered_map[net3], 4);
}

TEST(ipv4_network, Swap) {
    auto net1 = ipv4_network::parse("127.0.0.1");
    auto net2 = ipv4_network::parse("127.0.0.0/24");
    
    std::swap(net1, net2);

    ASSERT_EQ(net1, ipv4_network::parse("127.0.0.0/255.255.255.0"));
    ASSERT_EQ(net2, ipv4_network::parse("127.0.0.1"));
}

TEST(ipv4_network, literals) {
    constexpr auto net1 = "127.0.0.0/16"_ipv4_net;
    constexpr auto net2 = "127.128.128.255"_ipv4_net;
    
    ASSERT_EQ(net1, ipv4_network::parse("127.0.0.0/16"));
    ASSERT_EQ(net2, ipv4_network::parse("127.128.128.255/32"));
}

using IsMulticastIpv4NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsMulticastIpv4NetworkParams, is_multicast) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_network::parse(std::get<0>(GetParam())).is_multicast();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, IsMulticastIpv4NetworkParams,
    Values(
        std::make_tuple("224.1.1.0/31", true),
        std::make_tuple("240.0.0.0", false)
    ));

using IsPrivateIpv4NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsPrivateIpv4NetworkParams, is_private) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_network::parse(std::get<0>(GetParam())).is_private();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, IsPrivateIpv4NetworkParams,
    Values(
        std::make_tuple("1.0.0.0/8", false),
        std::make_tuple("100.64.0.0/10", false),
        std::make_tuple("0.0.0.0/0", false),
        std::make_tuple("0.0.0.0/8", true),
        std::make_tuple("10.0.0.0/8", true),
        std::make_tuple("127.0.0.0/8", true),
        std::make_tuple("169.254.0.0/16", true),
        std::make_tuple("172.16.0.0/12", true),
        std::make_tuple("192.0.0.0/29", true),
        std::make_tuple("192.0.0.170/31", true),
        std::make_tuple("192.0.2.0/24", true),
        std::make_tuple("192.168.0.0/16", true),
        std::make_tuple("198.18.0.0/15", true),
        std::make_tuple("198.51.100.0/24", true),
        std::make_tuple("203.0.113.0/24", true),
        std::make_tuple("240.0.0.0/4", true),
        std::make_tuple("255.255.255.255/32", true)
    ));

using IsGlobalIpv4NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsGlobalIpv4NetworkParams, is_global) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_network::parse(std::get<0>(GetParam())).is_global();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, IsGlobalIpv4NetworkParams,
    Values(
        std::make_tuple("100.64.0.0/10", false),
        std::make_tuple("192.0.3.0/24", true)
    ));

using IsReservedIpv4NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsReservedIpv4NetworkParams, is_reserved) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_network::parse(std::get<0>(GetParam())).is_reserved();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, IsReservedIpv4NetworkParams,
    Values(
        std::make_tuple("240.0.0.1", true),
        std::make_tuple("239.255.255.255", false)
    ));

using IsLoopbackIpv4NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsLoopbackIpv4NetworkParams, is_loopback) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_network::parse(std::get<0>(GetParam())).is_loopback();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, IsLoopbackIpv4NetworkParams,
    Values(
        std::make_tuple("127.100.200.254", true),
        std::make_tuple("127.42.0.0/24", true),
        std::make_tuple("128.0.0.0/8", false)
    ));

using IsLinkLocalIpv4NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsLinkLocalIpv4NetworkParams, is_link_local) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_network::parse(std::get<0>(GetParam())).is_link_local();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, IsLinkLocalIpv4NetworkParams,
    Values(
        std::make_tuple("169.254.1.0/24", true),
        std::make_tuple("169.255.100.200", false)
    ));

using IsUnspecifiedIpv4NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsUnspecifiedIpv4NetworkParams, is_unspecified) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_network::parse(std::get<0>(GetParam())).is_unspecified();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, IsUnspecifiedIpv4NetworkParams,
    Values(
        std::make_tuple("0.0.0.0/32", true),
        std::make_tuple("0.0.0.0/8", false)
    ));

using ContainsIpv4NetworkParams = TestWithParam<std::tuple<const char*, const char*, bool>>;
TEST_P(ContainsIpv4NetworkParams, contains) {
    const auto expected = std::get<2>(GetParam());

    const auto network = ipv4_network::parse(std::get<0>(GetParam()));
    const auto address = ipv4_address::parse(std::get<1>(GetParam()));

    const auto actual = network.contains(address);

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, ContainsIpv4NetworkParams,
    Values(
        std::make_tuple("192.0.2.0/28", "192.0.2.6", true),
        std::make_tuple("192.0.2.0/28", "192.0.3.6", false)
    ));

using OverlapsIpv4NetworkParams = TestWithParam<std::tuple<const char*, const char*, bool>>;
TEST_P(OverlapsIpv4NetworkParams, overlaps) {
    const auto expected = std::get<2>(GetParam());

    const auto net1 = ipv4_network::parse(std::get<0>(GetParam()));
    const auto net2 = ipv4_network::parse(std::get<1>(GetParam()));

    const auto actual = net1.overlaps(net2);

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, OverlapsIpv4NetworkParams,
    Values(
        std::make_tuple("1.2.3.0/24", "1.2.3.0/30", true),
        std::make_tuple("1.2.3.0/24", "1.2.2.0/24", false),
        std::make_tuple("1.2.2.0/24", "1.2.2.64/26", true)
    ));

using SubnetOfIpv4NetworkParams = TestWithParam<std::tuple<const char*, const char*, bool>>;
TEST_P(SubnetOfIpv4NetworkParams, subnet_of) {
    const auto expected = std::get<2>(GetParam());

    const auto net1 = ipv4_network::parse(std::get<0>(GetParam()));
    const auto net2 = ipv4_network::parse(std::get<1>(GetParam()));

    const auto actual = net1.subnet_of(net2);

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, SubnetOfIpv4NetworkParams,
    Values(
        std::make_tuple("192.168.1.0/24", "192.168.1.128/30", false),
        std::make_tuple("10.0.0.0/30", "10.0.1.0/24", false),
        std::make_tuple("10.0.0.0/30", "10.0.0.0/24", true),
        std::make_tuple("10.0.0.0/30", "10.0.1.0/24", false),
        std::make_tuple("10.0.1.0/24", "10.0.0.0/30", false)
    ));

using SupernetOfIpv4NetworkParams = TestWithParam<std::tuple<const char*, const char*, bool>>;
TEST_P(SupernetOfIpv4NetworkParams, supernet_of) {
    const auto expected = std::get<2>(GetParam());

    const auto net1 = ipv4_network::parse(std::get<0>(GetParam()));
    const auto net2 = ipv4_network::parse(std::get<1>(GetParam()));

    const auto actual = net1.supernet_of(net2);

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, SupernetOfIpv4NetworkParams,
    Values(
        std::make_tuple("192.168.1.0/24", "192.168.1.128/30", true),
        std::make_tuple("10.0.0.0/30", "10.0.1.0/24", false),
        std::make_tuple("10.0.0.0/30", "10.0.0.0/24", false),
        std::make_tuple("10.0.0.0/24", "10.0.0.0/30", true)
    ));

using AddressesCountIpv4NetworkParams = TestWithParam<std::tuple<const char*, uint32_t>>;
TEST_P(AddressesCountIpv4NetworkParams, addresses_count) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_network::parse(std::get<0>(GetParam())).addresses_count();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, AddressesCountIpv4NetworkParams,
    Values(
        std::make_tuple("192.168.1.0/24", (uint32_t) 256),
        std::make_tuple("192.168.1.0/32", (uint32_t) 1),
        std::make_tuple("1.2.3.0/24", (uint32_t) 256),
        std::make_tuple("10.0.0.0/30", (uint32_t) 4),
        std::make_tuple("0.0.0.0/0", (uint32_t) 4294967296),
        std::make_tuple("0.0.0.0/1", (uint32_t) 2147483648),
        std::make_tuple("0.0.0.0/32", (uint32_t) 1),
        std::make_tuple("0.0.0.0/31", (uint32_t) 2)
    ));

using HostsIpv4NetworkParams = TestWithParam<std::tuple<const char*, std::vector<const char*>>>;
TEST_P(HostsIpv4NetworkParams, hosts) {
    std::vector<ipv4_address> expected;
    for (const auto& addr : std::get<1>(GetParam())) {
        expected.push_back(ipv4_address::parse(addr));
    }
    const auto actual = ipv4_network::parse(std::get<0>(GetParam())).hosts();

    ASSERT_FALSE(actual.empty());
    ASSERT_EQ(actual.size(), expected.size());
    ASSERT_EQ(actual.front(), expected.front());
    ASSERT_EQ(actual.back(), expected.back());

    auto expected_it = expected.begin();
    for (const auto& address : actual) {
        ASSERT_EQ(address, *expected_it++);
    }

    auto expected_const_it = expected.cbegin();
    for (auto it = actual.cbegin(); it != actual.cend(); ++it) {
        ASSERT_EQ(*it, *expected_const_it++);
    }

    auto expected_reverse_it = expected.rbegin();
    for (auto it = actual.rbegin(); it != actual.rend(); ++it) {
        ASSERT_EQ(*it, *expected_reverse_it++);
    }

    auto expected_const_reverse_it = expected.crbegin();
    auto actual_const_reverse_it = actual.crbegin();
    for (; actual_const_reverse_it != actual.crend(); ++actual_const_reverse_it) {
        ASSERT_EQ(*actual_const_reverse_it, *expected_const_reverse_it++);
    }

    expected_const_it = expected.cbegin();
    for (auto it = actual_const_reverse_it.base(); it != actual.cend(); ++it) {
        ASSERT_EQ(*it, *expected_const_it++);
    }

    for (size_t i = 0; i < actual.size(); ++i) {
        ASSERT_EQ(actual[(uint32_t) i], expected[i]);
        ASSERT_EQ(actual.at((uint32_t) i), expected[i]);
    }
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, HostsIpv4NetworkParams,
    Values(
        std::make_tuple("192.0.2.0/29", std::vector<const char*>{"192.0.2.1", "192.0.2.2", "192.0.2.3", "192.0.2.4", "192.0.2.5", "192.0.2.6" }),
        std::make_tuple("192.0.2.0/31", std::vector<const char*>{"192.0.2.0", "192.0.2.1"}),
        std::make_tuple("192.0.2.1/32", std::vector<const char*>{"192.0.2.1"})
    ));

using SupernetIpv4NetworkParams = TestWithParam<std::tuple<const char*, size_t, optional<size_t>, const char*>>;
TEST_P(SupernetIpv4NetworkParams, supernet) {
    const auto expected = ipv4_network::parse(std::get<3>(GetParam()));

    const auto network = ipv4_network::parse(std::get<0>(GetParam()));
    const auto prefixlen_diff = std::get<1>(GetParam());
    const auto new_prefix = std::get<2>(GetParam());

    const auto actual = network.supernet(prefixlen_diff, new_prefix);

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, SupernetIpv4NetworkParams,
    Values(
        std::make_tuple("192.0.2.0/24", 1, nullptr, "192.0.2.0/23"),
        std::make_tuple("192.0.2.0/24", 2, nullptr, "192.0.0.0/22"),
        std::make_tuple("192.0.2.0/24", 1, 20, "192.0.0.0/20"),
        std::make_tuple("192.0.0.0/2", 1, nullptr, "128.0.0.0/1"),
        std::make_tuple("128.0.0.0/1", 1, nullptr, "0.0.0.0/0"),
        std::make_tuple("0.0.0.0/0", 1, nullptr, "0.0.0.0/0")
    ));

using SupernetErrorIpv4NetworkParams = TestWithParam<std::tuple<const char*, size_t, optional<size_t>, error_code, const char*>>;
TEST_P(SupernetErrorIpv4NetworkParams, supernet) {
    const auto expected_error = std::get<3>(GetParam());
    const auto expected_error_str = std::get<4>(GetParam());

    const auto network = ipv4_network::parse(std::get<0>(GetParam()));
    const auto prefixlen_diff = std::get<1>(GetParam());
    const auto new_prefix = std::get<2>(GetParam());

    error_code err;
    auto actural = network.supernet(err, prefixlen_diff, new_prefix);
    ASSERT_EQ(err, expected_error);
    ASSERT_EQ(actural.network_address(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(actural.netmask(), ipv4_address::parse("255.255.255.255"));
    ASSERT_EQ(actural.hostmask(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(actural.prefixlen(), 32);

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_network = network.supernet(prefixlen_diff, new_prefix);
    ASSERT_EQ(error_network.network_address(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(error_network.netmask(), ipv4_address::parse("255.255.255.255"));
    ASSERT_EQ(error_network.hostmask(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(error_network.prefixlen(), 32);
#else
    EXPECT_THAT(
        ([&network, prefixlen_diff, new_prefix]() { network.supernet(prefixlen_diff, new_prefix); }),
        ThrowsMessage<logic_error>(StrEq(expected_error_str)));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, SupernetErrorIpv4NetworkParams,
    Values(
        std::make_tuple("192.0.2.0/24", 1, 25, error_code::NEW_PREFIX_MUST_BE_SHORTER, "new prefix must be shorter"),
        std::make_tuple("192.0.2.0/24", 2, 23, error_code::CANNOT_SET_PREFIXLEN_DIFF_AND_NEW_PREFIX, "cannot set prefixlen_diff and new_prefix"),
        std::make_tuple("192.0.2.0/24", 25, nullptr, error_code::INVALID_PREFIXLEN_DIFF, "invalid prefixlen_diff")
    ));

using SubnetsIpv4NetworkParams = TestWithParam<std::tuple<const char*, size_t, optional<size_t>, std::vector<const char*>>>;
TEST_P(SubnetsIpv4NetworkParams, subnets) {
    std::vector<ipv4_network> expected;
    for (const auto& addr : std::get<3>(GetParam())) {
        expected.push_back(ipv4_network::parse(addr));
    }

    const auto network = ipv4_network::parse(std::get<0>(GetParam()));
    const auto prefixlen_diff = std::get<1>(GetParam());
    const auto new_prefix = std::get<2>(GetParam());

    const auto actual = network.subnets(prefixlen_diff, new_prefix);

    ASSERT_FALSE(actual.empty());
    ASSERT_EQ(actual.size(), expected.size());
    ASSERT_EQ(actual.front(), expected.front());
    ASSERT_EQ(actual.back(), expected.back());
    
    auto expected_it = expected.begin();
    for (const auto& address : actual) {
        ASSERT_EQ(address, *expected_it++);
    }

    auto expected_const_it = expected.cbegin();
    for (auto it = actual.cbegin(); it != actual.cend(); ++it) {
        ASSERT_EQ(*it, *expected_const_it++);
    }

    auto expected_reverse_it = expected.rbegin();
    for (auto it = actual.rbegin(); it != actual.rend(); ++it) {
        ASSERT_EQ(*it, *expected_reverse_it++);
    }

    auto expected_const_reverse_it = expected.crbegin();
    auto actual_const_reverse_it = actual.crbegin();
    for (; actual_const_reverse_it != actual.crend(); ++actual_const_reverse_it) {
        ASSERT_EQ(*actual_const_reverse_it, *expected_const_reverse_it++);
    }

    expected_const_it = expected.cbegin();
    for (auto it = actual_const_reverse_it.base(); it != actual.cend(); ++it) {
        ASSERT_EQ(*it, *expected_const_it++);
    }

    for (size_t i = 0; i < actual.size(); ++i) {
        ASSERT_EQ(actual[(uint32_t) i], expected[i]);
        ASSERT_EQ(actual.at((uint32_t) i), expected[i]);
    }
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, SubnetsIpv4NetworkParams,
    Values(
        std::make_tuple("0.0.0.0/0", 1, nullptr, std::vector<const char*>{"0.0.0.0/1", "128.0.0.0/1"}),
        std::make_tuple("192.0.2.0/24", 1, nullptr, std::vector<const char*>{"192.0.2.0/25", "192.0.2.128/25" }),
        std::make_tuple("192.0.2.0/24", 2, nullptr, std::vector<const char*>{"192.0.2.0/26", "192.0.2.64/26", "192.0.2.128/26", "192.0.2.192/26"}),
        std::make_tuple("192.0.2.0/24", 1, 26, std::vector<const char*>{"192.0.2.0/26", "192.0.2.64/26", "192.0.2.128/26", "192.0.2.192/26"}),
        std::make_tuple("192.0.2.0/24", 1, 25, std::vector<const char*>{"192.0.2.0/25", "192.0.2.128/25"}),
        std::make_tuple("192.0.2.0/32", 1, nullptr, std::vector<const char*>{"192.0.2.0/32"}),
        std::make_tuple("255.255.255.255/32", 1, nullptr, std::vector<const char*>{"255.255.255.255/32"})
    ));

using SubnetsErrorIpv4NetworkParams = TestWithParam<std::tuple<const char*, size_t, optional<size_t>, error_code, const char*>>;
TEST_P(SubnetsErrorIpv4NetworkParams, subnets) {
    const auto expected_error = std::get<3>(GetParam());
    const auto expected_error_str = std::get<4>(GetParam());

    const auto network = ipv4_network::parse(std::get<0>(GetParam()));
    const auto prefixlen_diff = std::get<1>(GetParam());
    const auto new_prefix = std::get<2>(GetParam());

    error_code err;
    const auto actual = network.subnets(err, prefixlen_diff, new_prefix);
    ASSERT_EQ(err, expected_error);
    ASSERT_TRUE(actual.empty());

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_subnets = network.subnets(prefixlen_diff, new_prefix);
    ASSERT_TRUE(error_subnets.empty());
#else
    EXPECT_THAT(
        ([&network, prefixlen_diff, new_prefix]() { network.subnets(prefixlen_diff, new_prefix); }),
        ThrowsMessage<logic_error>(StrEq(expected_error_str)));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, SubnetsErrorIpv4NetworkParams,
    Values(
        std::make_tuple("192.0.2.0/24", 1, 23, error_code::NEW_PREFIX_MUST_BE_LONGER, "new prefix must be longer"),
        std::make_tuple("192.0.2.0/24", 2, 25, error_code::CANNOT_SET_PREFIXLEN_DIFF_AND_NEW_PREFIX, "cannot set prefixlen_diff and new_prefix"),
        std::make_tuple("192.0.2.0/24", 1, 33, error_code::INVALID_PREFIXLEN_DIFF, "invalid prefixlen_diff")
    ));

using AddressExcludeIpv4NetworkParams = TestWithParam<std::tuple<const char*, const char*, std::vector<const char*>>>;
TEST_P(AddressExcludeIpv4NetworkParams, address_exclude) {
    std::vector<ipv4_network> expected;
    for (const auto& addr : std::get<2>(GetParam())) {
        expected.push_back(ipv4_network::parse(addr));
    }
    
    const auto actual = ipv4_network::parse(std::get<0>(GetParam()))
        .address_exclude(ipv4_network::parse(std::get<1>(GetParam())));

    ASSERT_EQ(actual.empty(), expected.empty());
    
    auto expected_it = expected.begin();
    for (const auto& address : actual) {
        ASSERT_EQ(address, *expected_it++);
    }

    auto expected_const_it = expected.cbegin();
    for (auto it = actual.cbegin(); it != actual.cend(); ++it) {
        ASSERT_EQ(*it, *expected_const_it++);
    }
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, AddressExcludeIpv4NetworkParams,
    Values(
        std::make_tuple("192.0.2.0/28", "192.0.2.1/32", std::vector<const char*>{"192.0.2.8/29", "192.0.2.4/30", "192.0.2.2/31", "192.0.2.0/32" })
    ));

using AddressExcludeErrorIpv4NetworkParams = TestWithParam<std::tuple<const char*, const char*, error_code, const char*>>;
TEST_P(AddressExcludeErrorIpv4NetworkParams, address_exclude) {
    const auto expected_error = std::get<2>(GetParam());
    const auto expected_error_str = std::get<3>(GetParam());

    const auto network1 = ipv4_network::parse(std::get<0>(GetParam()));
    const auto network2 = ipv4_network::parse(std::get<1>(GetParam()));
    
    error_code err;
    const auto actual = network1.address_exclude(network2, err);
    ASSERT_EQ(err, expected_error);
    ASSERT_TRUE(actual.empty());

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_address_exclude = network1.address_exclude(network2);
    ASSERT_TRUE(error_address_exclude.empty());
#else
    EXPECT_THAT(
        ([&network1, &network2]() { network1.address_exclude(network2); }),
        ThrowsMessage<logic_error>(StrEq(expected_error_str)));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, AddressExcludeErrorIpv4NetworkParams,
    Values(
        std::make_tuple("192.168.1.128/30", "192.168.1.0/24", error_code::NOT_CONTAINED_NETWORK, "network is not a subnet of other")
    ));
