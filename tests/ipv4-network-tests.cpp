#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ipv4_network, DefaultCtor) {
    ipv4_network net;
    
    EXPECT_EQ(net.address(), ipv4_address::from_uint32<0>());
    EXPECT_EQ(net.netmask(), ipv4_address::from_uint32<0xFFFFFFFF>());
    EXPECT_EQ(net.hostmask(), ipv4_address::from_uint32<0>());
    EXPECT_EQ(net.prefixlen(), 32);
    EXPECT_EQ(net.version, version::V4);
}

TEST(ipv4_network, CopyCtor) {
    auto net = ipv4_network::parse("127.0.0.0/8");
    auto net_copy = net;
    
    EXPECT_EQ(net_copy.address(), ipv4_address::from_uint32<0x7F000000>());
    EXPECT_EQ(net_copy.netmask(), ipv4_address::from_uint32<0xFF000000>());
    EXPECT_EQ(net_copy.hostmask(), ipv4_address::from_uint32<0x00FFFFFF>());
    EXPECT_EQ(net_copy.prefixlen(), 8);
}

TEST(ipv4_network, CopyOperator) {
    auto net = ipv4_network::parse("127.0.0.0/8");
    ipv4_network net_copy;
    
    EXPECT_EQ(net_copy.address(), ipv4_address::from_uint32<0>());
    EXPECT_EQ(net_copy.netmask(), ipv4_address::from_uint32<0xFFFFFFFF>());
    EXPECT_EQ(net_copy.hostmask(), ipv4_address::from_uint32<0>());
    EXPECT_EQ(net_copy.prefixlen(), 32);
    net_copy = net;
    
    EXPECT_EQ(net_copy.address(), ipv4_address::from_uint32<0x7F000000>());
    EXPECT_EQ(net_copy.netmask(), ipv4_address::from_uint32<0xFF000000>());
    EXPECT_EQ(net_copy.hostmask(), ipv4_address::from_uint32<0x00FFFFFF>());
    EXPECT_EQ(net_copy.prefixlen(), 8);
}

using NetworkParserIpv4Params = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv4Params, parse) {
    auto excepted_address = ipv4_address::parse(get<1>(GetParam()));
    auto excepted_netmask = ipv4_address::parse(get<2>(GetParam()));
    auto excepted_hostmask = ipv4_address::parse(get<3>(GetParam()));
    auto excepted_prefixlen = get<4>(GetParam());

    auto net = ipv4_network::parse(get<0>(GetParam()));
   
    EXPECT_EQ(net.address(), excepted_address);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv4_network net_from_stream;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> net_from_stream >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    EXPECT_EQ(net_from_stream.address(), excepted_address);
    EXPECT_EQ(net_from_stream.netmask(), excepted_netmask);
    EXPECT_EQ(net_from_stream.hostmask(), excepted_hostmask);
    EXPECT_EQ(net_from_stream.prefixlen(), excepted_prefixlen);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, NetworkParserIpv4Params,
    testing::Values(
        std::make_tuple("1.2.3.4", "1.2.3.4", "255.255.255.255", "0.0.0.0", 32),
        std::make_tuple("1.2.3.4/32", "1.2.3.4", "255.255.255.255", "0.0.0.0", 32),
        std::make_tuple("1.2.3.4/255.255.255.255", "1.2.3.4", "255.255.255.255", "0.0.0.0", 32),
        std::make_tuple("192.0.2.0/24", "192.0.2.0", "255.255.255.0", "0.0.0.255", 24),
        std::make_tuple("192.0.2.0/255.255.255.0", "192.0.2.0", "255.255.255.0", "0.0.0.255", 24),
        std::make_tuple("192.0.2.0/0.0.0.255", "192.0.2.0", "255.255.255.0", "0.0.0.255", 24),
        std::make_tuple("192.0.2.0/27", "192.0.2.0", "255.255.255.224", "0.0.0.31", 27),
        std::make_tuple("192.0.2.0/255.255.255.224", "192.0.2.0", "255.255.255.224", "0.0.0.31", 27)
    ));
