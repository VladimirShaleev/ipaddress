#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ip_network, DefaultCtor) {
    constexpr ip_network net;

    constexpr auto actual = net.v4();
    constexpr auto version = net.version();
    constexpr auto size = net.size();
    constexpr auto is_v4 = net.is_v4();
    constexpr auto is_v6 = net.is_v6();
    constexpr auto has_value = actual.has_value();

    EXPECT_EQ(version, ip_version::V4);
    EXPECT_EQ(size, 4);
    EXPECT_TRUE(is_v4);
    EXPECT_FALSE(is_v6);
    EXPECT_TRUE(has_value);

    constexpr auto value = actual.value();
    constexpr auto network_address = value.network_address().to_uint();
    constexpr auto broadcast_address = value.broadcast_address().to_uint();
    constexpr auto netmask = value.netmask().to_uint();
    constexpr auto hostmask = value.hostmask().to_uint();
    constexpr auto prefixlen = value.prefixlen();
    constexpr auto ipv4_version = value.version();
    EXPECT_EQ(network_address, 0);
    EXPECT_EQ(broadcast_address, 0);
    EXPECT_EQ(netmask, 0xFFFFFFFF);
    EXPECT_EQ(hostmask, 0);
    EXPECT_EQ(prefixlen, 32);
    EXPECT_EQ(ipv4_version, ip_version::V4);
}

TEST(ip_network, CopyCtor) {
    constexpr auto net = ip_network::parse("127.0.0.0/24");
    constexpr auto net_copy = net;

    constexpr auto actual_net = net.v4();
    constexpr auto actual_net_copy = net_copy.v4();

    EXPECT_EQ(net.version(), ip_version::V4);
    EXPECT_EQ(net.size(), 4);
    EXPECT_TRUE(net.is_v4());
    EXPECT_FALSE(net.is_v6());
    EXPECT_TRUE(actual_net.has_value());

    EXPECT_EQ(net_copy.version(), ip_version::V4);
    EXPECT_EQ(net_copy.size(), 4);
    EXPECT_TRUE(net_copy.is_v4());
    EXPECT_FALSE(net_copy.is_v6());
    EXPECT_TRUE(actual_net_copy.has_value());

    constexpr auto value_net = actual_net.value();
    constexpr auto value_net_copy = actual_net_copy.value();
    EXPECT_EQ(value_net.network_address().to_uint(), 0x7F000000);
    EXPECT_EQ(value_net.broadcast_address().to_uint(), 0x7F0000FF);
    EXPECT_EQ(value_net.netmask().to_uint(), 0xFFFFFF00);
    EXPECT_EQ(value_net.hostmask().to_uint(), 0x000000FF);
    EXPECT_EQ(value_net.prefixlen(), 24);
    EXPECT_EQ(value_net.version(), ip_version::V4);

    EXPECT_EQ(value_net_copy.network_address().to_uint(), 0x7F000000);
    EXPECT_EQ(value_net_copy.broadcast_address().to_uint(), 0x7F0000FF);
    EXPECT_EQ(value_net_copy.netmask().to_uint(), 0xFFFFFF00);
    EXPECT_EQ(value_net_copy.hostmask().to_uint(), 0x000000FF);
    EXPECT_EQ(value_net_copy.prefixlen(), 24);
    EXPECT_EQ(value_net_copy.version(), ip_version::V4);
}

TEST(ip_network, CopyOperator) {
    constexpr auto net = 
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    ip_network::parse<"::ffff:0:0">();
#else
    ip_network::parse("::ffff:0:0");
#endif
    ip_network net_copy = ip_network::parse("127.0.0.1");

    auto actual_net4_copy = net_copy.v4();
    auto actual_net6_copy = net_copy.v6();
    const auto value_net4_copy = actual_net4_copy.value();
    EXPECT_EQ(net_copy.version(), ip_version::V4);
    EXPECT_EQ(net_copy.size(), 4);
    EXPECT_TRUE(net_copy.is_v4());
    EXPECT_FALSE(net_copy.is_v6());
    EXPECT_TRUE(actual_net4_copy.has_value());
    EXPECT_FALSE(actual_net6_copy.has_value());
    EXPECT_EQ(value_net4_copy.network_address().to_uint(), 0x7F000001);
    
    net_copy = net;
    actual_net4_copy = net_copy.v4();
    actual_net6_copy = net_copy.v6();
    const auto value_net6_copy = actual_net6_copy.value();
    EXPECT_EQ(net_copy.version(), ip_version::V6);
    EXPECT_EQ(net_copy.size(), 16);
    EXPECT_FALSE(net_copy.is_v4());
    EXPECT_TRUE(net_copy.is_v6());
    EXPECT_FALSE(actual_net4_copy.has_value());
    EXPECT_TRUE(actual_net6_copy.has_value());
    EXPECT_EQ(value_net6_copy.network_address().to_uint(), uint128_t::from_string("281470681743360").value());
}

TEST(ip_network, Ctors) {
    constexpr auto net4 = ipv4_network::parse("192.168.0.1");
    constexpr auto net6 = ipv6_network::parse("2002:ac1d:2d64::1");
    constexpr auto ip4 = ipv4_address::parse("192.168.0.1");
    constexpr auto ip6 = ip_address::parse("2002:ac1d:2d64::1");

    constexpr ip_network actual1(net4);
    constexpr ip_network actual2(net6);
    constexpr ip_network actual3 = ip_network::from_address(ip4, 24, false);
    constexpr ip_network actual4 = ip_network::from_address(ip6, 128);
    
    auto actual1_net = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_net.has_value());
    EXPECT_EQ(actual1_net.value().network_address().to_uint(), 0xC0A80001);
    EXPECT_EQ(actual1_net.value().broadcast_address().to_uint(), 0xC0A80001);
    EXPECT_EQ(actual1_net.value().netmask().to_uint(), 0xFFFFFFFF);
    EXPECT_EQ(actual1_net.value().hostmask().to_uint(), 0);
    EXPECT_EQ(actual1_net.value().prefixlen(), 32);

    auto actual2_net = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_net.has_value());
    EXPECT_EQ(actual2_net.value().network_address().to_uint(), uint128_t::from_string("42549171344950636613079587071710986241").value());
    EXPECT_EQ(actual2_net.value().broadcast_address().to_uint(), uint128_t::from_string("42549171344950636613079587071710986241").value());
    EXPECT_EQ(actual2_net.value().netmask().to_uint(), uint128_t::from_string("340282366920938463463374607431768211455").value());
    EXPECT_EQ(actual2_net.value().hostmask().to_uint(), 0);
    EXPECT_EQ(actual2_net.value().prefixlen(), 128);
    
    auto actual3_net = actual3.v4();
    EXPECT_EQ(actual3.version(), ip_version::V4);
    EXPECT_EQ(actual3.size(), 4);
    EXPECT_TRUE(actual3.is_v4());
    EXPECT_FALSE(actual3.is_v6());
    EXPECT_TRUE(actual3_net.has_value());
    EXPECT_EQ(actual3_net.value().network_address().to_uint(), 0xC0A80000);
    EXPECT_EQ(actual3_net.value().broadcast_address().to_uint(), 0xC0A800FF);
    EXPECT_EQ(actual3_net.value().netmask().to_uint(), 0xFFFFFF00);
    EXPECT_EQ(actual3_net.value().hostmask().to_uint(), 0x000000FF);
    EXPECT_EQ(actual3_net.value().prefixlen(), 24);

    auto actual4_net = actual4.v6();
    EXPECT_EQ(actual4.version(), ip_version::V6);
    EXPECT_EQ(actual4.size(), 16);
    EXPECT_FALSE(actual4.is_v4());
    EXPECT_TRUE(actual4.is_v6());
    EXPECT_TRUE(actual4_net.has_value());
    EXPECT_EQ(actual4_net.value().network_address().to_uint(), uint128_t::from_string("42549171344950636613079587071710986241").value());
    EXPECT_EQ(actual4_net.value().broadcast_address().to_uint(), uint128_t::from_string("42549171344950636613079587071710986241").value());
    EXPECT_EQ(actual4_net.value().netmask().to_uint(), uint128_t::from_string("340282366920938463463374607431768211455").value());
    EXPECT_EQ(actual4_net.value().hostmask().to_uint(), 0);
    EXPECT_EQ(actual4_net.value().prefixlen(), 128);
}
