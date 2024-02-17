#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ip_address, DefaultCtor) {
    constexpr ip_address ip;

    constexpr auto actual = ip.v4();

    EXPECT_EQ(ip.version(), ip_version::V4);
    EXPECT_EQ(ip.size(), 4);
    EXPECT_TRUE(ip.is_v4());
    EXPECT_FALSE(ip.is_v6());
    EXPECT_TRUE(actual.has_value());

    constexpr auto value = actual.value();
    EXPECT_EQ(value.to_uint(), 0);
}

TEST(ip_address, CopyCtor) {
    constexpr auto ip = ip_address::parse("127.0.0.1");
    constexpr auto ip_copy = ip;

    constexpr auto actual_ip = ip.v4();
    constexpr auto actual_ip_copy = ip_copy.v4();

    EXPECT_EQ(ip.version(), ip_version::V4);
    EXPECT_EQ(ip.size(), 4);
    EXPECT_TRUE(ip.is_v4());
    EXPECT_FALSE(ip.is_v6());
    EXPECT_TRUE(actual_ip.has_value());

    EXPECT_EQ(ip_copy.version(), ip_version::V4);
    EXPECT_EQ(ip_copy.size(), 4);
    EXPECT_TRUE(ip_copy.is_v4());
    EXPECT_FALSE(ip_copy.is_v6());
    EXPECT_TRUE(actual_ip_copy.has_value());

    constexpr auto value_ip = actual_ip.value();
    constexpr auto value_ip_copy = actual_ip_copy.value();
    EXPECT_EQ(value_ip.to_uint(), 0x7F000001);
    EXPECT_EQ(value_ip_copy.to_uint(), 0x7F000001);
}

TEST(ip_address, CopyOperator) {
    constexpr auto ip = ip_address::parse("::ffff:0:0");
    ip_address ip_copy = ip_address::parse("127.0.0.1");

    auto actual_ip4_copy = ip_copy.v4();
    auto actual_ip6_copy = ip_copy.v6();
    const auto value_ip4_copy = actual_ip4_copy.value();
    EXPECT_EQ(ip_copy.version(), ip_version::V4);
    EXPECT_EQ(ip_copy.size(), 4);
    EXPECT_TRUE(ip_copy.is_v4());
    EXPECT_FALSE(ip_copy.is_v6());
    EXPECT_TRUE(actual_ip4_copy.has_value());
    EXPECT_FALSE(actual_ip6_copy.has_value());
    EXPECT_EQ(value_ip4_copy.to_uint(), 0x7F000001);
    
    ip_copy = ip;
    actual_ip4_copy = ip_copy.v4();
    actual_ip6_copy = ip_copy.v6();
    const auto value_ip6_copy = actual_ip6_copy.value();
    EXPECT_EQ(ip_copy.version(), ip_version::V6);
    EXPECT_EQ(ip_copy.size(), 16);
    EXPECT_FALSE(ip_copy.is_v4());
    EXPECT_TRUE(ip_copy.is_v6());
    EXPECT_FALSE(actual_ip4_copy.has_value());
    EXPECT_TRUE(actual_ip6_copy.has_value());
    EXPECT_EQ(value_ip6_copy.to_uint(), uint128_t::from_string("281470681743360").value());
}

TEST(ip_address, Ctors) {
    constexpr auto ipv4 = ipv4_address::parse("192.168.0.1");
    constexpr auto ipv6 = ipv6_address::parse("2002:ac1d:2d64::1");
    constexpr ipv4_address::base_type ipv4_bytes {0xC0, 0xA8, 0x00, 0x01};
    constexpr ipv6_address::base_type ipv6_bytes { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 255, 255,
        0, 0, 0, 0
    };

    constexpr ip_address actual1(ipv4);
    constexpr ip_address actual2(ipv6);
    constexpr ip_address actual3(ipv4_bytes);
    constexpr ip_address actual4(ipv6_bytes);
    
    auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().to_uint(), 0xC0A80001);

    auto actual2_ip = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_ip.has_value());
    EXPECT_EQ(actual2_ip.value().to_uint(), uint128_t::from_string("42549171344950636613079587071710986241").value());
    
    auto actual3_ip = actual3.v4();
    EXPECT_EQ(actual3.version(), ip_version::V4);
    EXPECT_EQ(actual3.size(), 4);
    EXPECT_TRUE(actual3.is_v4());
    EXPECT_FALSE(actual3.is_v6());
    EXPECT_TRUE(actual3_ip.has_value());
    EXPECT_EQ(actual3_ip.value().to_uint(), 0xC0A80001);

    auto actual4_ip = actual4.v6();
    EXPECT_EQ(actual4.version(), ip_version::V6);
    EXPECT_EQ(actual4.size(), 16);
    EXPECT_FALSE(actual4.is_v4());
    EXPECT_TRUE(actual4.is_v6());
    EXPECT_TRUE(actual4_ip.has_value());
    EXPECT_EQ(actual4_ip.value().to_uint(), 281470681743360ULL);
}

TEST(ip_address, from_bytes) {
    constexpr ipv4_address::base_type ipv4_bytes {0xC0, 0xA8, 0x00, 0x01};
    constexpr ipv6_address::base_type ipv6_bytes { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 255, 255,
        0, 0, 0, 0
    };

    constexpr auto actual1 = ip_address::from_bytes(ipv4_bytes);
    constexpr auto actual2 = ip_address::from_bytes(ipv6_bytes);
    constexpr auto actual3 = ip_address::from_bytes(ipv4_bytes.data(), 3, ip_version::V4);
    constexpr auto actual4 = ip_address::from_bytes(ipv6_bytes.data(), 11, ip_version::V6);
    
    auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().to_uint(), 0xC0A80001);

    auto actual2_ip = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_ip.has_value());
    EXPECT_EQ(actual2_ip.value().to_uint(), 281470681743360ULL);
    
    auto actual3_ip = actual3.v4();
    EXPECT_EQ(actual3.version(), ip_version::V4);
    EXPECT_EQ(actual3.size(), 4);
    EXPECT_TRUE(actual3.is_v4());
    EXPECT_FALSE(actual3.is_v6());
    EXPECT_TRUE(actual3_ip.has_value());
    EXPECT_EQ(actual3_ip.value().to_uint(), 0xC0A80000);

    auto actual4_ip = actual4.v6();
    EXPECT_EQ(actual4.version(), ip_version::V6);
    EXPECT_EQ(actual4.size(), 16);
    EXPECT_FALSE(actual4.is_v4());
    EXPECT_TRUE(actual4.is_v6());
    EXPECT_TRUE(actual4_ip.has_value());
    EXPECT_EQ(actual4_ip.value().to_uint(), 280375465082880ULL);
}

TEST(ip_address, from_uint) {
    constexpr ipv4_address::uint_type ip1 = 0xC0A80001;
    constexpr ipv6_address::uint_type ip2 = 281470681743360ULL;

    constexpr auto actual1 = ip_address::from_uint(ip1);
    constexpr auto actual2 = ip_address::from_uint(ip2);
    
    auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().to_uint(), 0xC0A80001);

    auto actual2_ip = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_ip.has_value());
    EXPECT_EQ(actual2_ip.value().to_uint(), 281470681743360ULL);
}
