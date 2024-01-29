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

using NetworkParserIpv4NotStrictParams = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv4NotStrictParams, parse_not_strict) {
    auto excepted_address = ipv4_address::parse(get<1>(GetParam()));
    auto excepted_netmask = ipv4_address::parse(get<2>(GetParam()));
    auto excepted_hostmask = ipv4_address::parse(get<3>(GetParam()));
    auto excepted_prefixlen = get<4>(GetParam());

    auto net = ipv4_network::parse(get<0>(GetParam()), false);
   
    EXPECT_EQ(net.address(), excepted_address);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv4_network net_from_stream;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> not_strict >> net_from_stream >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    EXPECT_EQ(net_from_stream.address(), excepted_address);
    EXPECT_EQ(net_from_stream.netmask(), excepted_netmask);
    EXPECT_EQ(net_from_stream.hostmask(), excepted_hostmask);
    EXPECT_EQ(net_from_stream.prefixlen(), excepted_prefixlen);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, NetworkParserIpv4NotStrictParams,
    testing::Values(
        std::make_tuple("1.2.3.4/24", "1.2.3.0", "255.255.255.0", "0.0.0.255", 24),
        std::make_tuple("192.0.2.0/255.255.0.0", "192.0.0.0", "255.255.0.0", "0.0.255.255", 16)
    ));

using InvalidNetworkIpv4Params = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidNetworkIpv4Params, parse) {
    auto expected_network = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err;
    ipv4_network::parse(expected_network, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_network = ipv4_network::parse(expected_network);

    EXPECT_EQ(error_network.address().to_uint32(), 0x00000000);
    ASSERT_EQ(error_network.netmask().to_uint32(), 0xFFFFFFFF);
    ASSERT_EQ(error_network.hostmask().to_uint32(), 0x00000000);
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
    testing::Values(
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
    // TODO
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
    testing::Values(
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
    // TODO
}

TEST(ipv4_network, Swap) {
    auto net1 = ipv4_network::parse("127.0.0.1");
    auto net2 = ipv4_network::parse("127.0.0.0/24");
    
    std::swap(net1, net2);

    ASSERT_EQ(net1, ipv4_network::parse("127.0.0.0/255.255.255.0"));
    ASSERT_EQ(net2, ipv4_network::parse("127.0.0.1"));
}
