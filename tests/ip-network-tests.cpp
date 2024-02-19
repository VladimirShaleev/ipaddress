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

    EXPECT_EQ(net.version(), ip_version::V4);
    EXPECT_EQ(net.size(), 4);
    EXPECT_TRUE(net.is_v4());
    EXPECT_FALSE(net.is_v6());
    EXPECT_TRUE(actual.has_value());

    constexpr auto value = actual.value();
    EXPECT_EQ(value.network_address().to_uint(), 0);
    EXPECT_EQ(value.broadcast_address().to_uint(), 0);
    EXPECT_EQ(value.netmask().to_uint(), 0xFFFFFFFF);
    EXPECT_EQ(value.hostmask().to_uint(), 0);
    EXPECT_EQ(value.prefixlen(), 32);
    EXPECT_EQ(value.version(), ip_version::V4);
}

// TEST(ip_network, CopyCtor) {
//     constexpr auto net = ip_network::parse("127.0.0.0/24");
//     constexpr auto net_copy = net;
// 
//     constexpr auto actual_ip = ip.v4();
//     constexpr auto actual_ip_copy = ip_copy.v4();
// 
//     EXPECT_EQ(ip.version(), ip_version::V4);
//     EXPECT_EQ(ip.size(), 4);
//     EXPECT_TRUE(ip.is_v4());
//     EXPECT_FALSE(ip.is_v6());
//     EXPECT_TRUE(actual_ip.has_value());
// 
//     EXPECT_EQ(ip_copy.version(), ip_version::V4);
//     EXPECT_EQ(ip_copy.size(), 4);
//     EXPECT_TRUE(ip_copy.is_v4());
//     EXPECT_FALSE(ip_copy.is_v6());
//     EXPECT_TRUE(actual_ip_copy.has_value());
// 
//     constexpr auto value_ip = actual_ip.value();
//     constexpr auto value_ip_copy = actual_ip_copy.value();
//     EXPECT_EQ(value_ip.to_uint(), 0x7F000001);
//     EXPECT_EQ(value_ip_copy.to_uint(), 0x7F000001);
// }
