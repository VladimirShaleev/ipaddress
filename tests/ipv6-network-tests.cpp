#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ipv6_network, DefaultCtor) {
    ipv6_network net;
    
    EXPECT_EQ(net.address(), ipv6_address::parse("::"));
    EXPECT_EQ(net.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    EXPECT_EQ(net.hostmask(), ipv6_address::parse("::"));
    EXPECT_EQ(net.prefixlen(), 128);
    EXPECT_EQ(net.version, version::V6);
}

TEST(ipv6_network, CopyCtor) {
    auto net = ipv6_network::parse("2001:db8::/96");
    auto net_copy = net;
    
    EXPECT_EQ(net_copy.address(), ipv6_address::parse("2001:db8::"));
    EXPECT_EQ(net_copy.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff::"));
    EXPECT_EQ(net_copy.hostmask(), ipv6_address::parse("::ffff:ffff"));
    EXPECT_EQ(net_copy.prefixlen(), 96);
}

TEST(ipv6_network, CopyOperator) {
    auto net = ipv6_network::parse("2001:db8::/96");
    ipv6_network net_copy;
    
    EXPECT_EQ(net_copy.address(), ipv6_address::parse("::"));
    EXPECT_EQ(net_copy.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    EXPECT_EQ(net_copy.hostmask(), ipv6_address::parse("::"));
    EXPECT_EQ(net_copy.prefixlen(), 128);
    EXPECT_EQ(net_copy.version, version::V6);
    net_copy = net;

    EXPECT_EQ(net_copy.address(), ipv6_address::parse("2001:db8::"));
    EXPECT_EQ(net_copy.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff::"));
    EXPECT_EQ(net_copy.hostmask(), ipv6_address::parse("::ffff:ffff"));
    EXPECT_EQ(net_copy.prefixlen(), 96);
}

using NetworkParserIpv6Params = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv6Params, parse) {
    auto excepted_address = ipv6_address::parse(get<1>(GetParam()));
    auto excepted_netmask = ipv6_address::parse(get<2>(GetParam()));
    auto excepted_hostmask = ipv6_address::parse(get<3>(GetParam()));
    auto excepted_prefixlen = get<4>(GetParam());

    auto net = ipv6_network::parse(get<0>(GetParam()));
    
    EXPECT_EQ(net.address(), excepted_address);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv6_network net_from_stream;
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
    ipv6_network, NetworkParserIpv6Params,
    testing::Values(
        std::make_tuple("::1", "::1", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", 128),
        std::make_tuple("::1%scope", "::1%scope", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", 128),
        std::make_tuple("2001:db8::/32", "2001:db8::", "ffff:ffff::", "::ffff:ffff:ffff:ffff:ffff:ffff", 32),
        std::make_tuple("2001:db8::%scope/32", "2001:db8::%scope", "ffff:ffff::", "::ffff:ffff:ffff:ffff:ffff:ffff", 32),
        std::make_tuple("::/0", "::", "::", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", 0),
        std::make_tuple("::/128", "::", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", 128)
    ));

using NetworkParserIpv6NotStrictParams = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv6NotStrictParams, parse_not_strict) {
    auto excepted_address = ipv6_address::parse(get<1>(GetParam()));
    auto excepted_netmask = ipv6_address::parse(get<2>(GetParam()));
    auto excepted_hostmask = ipv6_address::parse(get<3>(GetParam()));
    auto excepted_prefixlen = get<4>(GetParam());

    auto net = ipv6_network::parse(get<0>(GetParam()), false);
   
    EXPECT_EQ(net.address(), excepted_address);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv6_network net_from_stream;
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
    ipv6_network, NetworkParserIpv6NotStrictParams,
    testing::Values(
        std::make_tuple("2001:db8::/16", "2001::", "ffff::", "0:ffff:ffff:ffff:ffff:ffff:ffff:ffff", 16),
        std::make_tuple("2001:db8::/24", "2001:d00::", "ffff:ff00::", "0:ff:ffff:ffff:ffff:ffff:ffff:ffff", 24)
    ));

using InvalidNetworkIpv6Params = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidNetworkIpv6Params, parse) {
    auto expected_network = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err;
    ipv6_network::parse(expected_network, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_network = ipv6_network::parse(expected_network);

    EXPECT_EQ(error_network.address(), ipv6_address::parse("::"));
    ASSERT_EQ(error_network.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(error_network.hostmask(), ipv6_address::parse("::"));
    ASSERT_EQ(error_network.prefixlen(), 128);
#else
    EXPECT_THAT(
        [network=expected_network]() { ipv6_network::parse(network); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
    EXPECT_THAT(
        [network=expected_network]() { ipv6_network::parse(network); },
        Throws<parse_error>(Property(&parse_error::code, Eq(expected_error_code))));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, InvalidNetworkIpv6Params,
    testing::Values(
        std::make_tuple("/", error_code::EMPTY_NETMASK, "empty mask in address /"),
        std::make_tuple("2001:db8::/", error_code::EMPTY_NETMASK, "empty mask in address 2001:db8::/"),
        std::make_tuple("2001:db8::/129", error_code::INVALID_NETMASK, "is not a valid netmask in address 2001:db8::/129"),
        std::make_tuple("2001:db8::/255.255.255.255", error_code::INVALID_NETMASK, "is not a valid netmask in address 2001:db8::/255.255.255.255"),
        std::make_tuple("/%scope", error_code::INVALID_NETMASK, "is not a valid netmask in address /%scope"),
        std::make_tuple("/%scope8", error_code::INVALID_NETMASK, "is not a valid netmask in address /%scope8"),
        std::make_tuple("::1/::1", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/::1"),
        std::make_tuple("::1/1::", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/1::"),
        std::make_tuple("::1/-1", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/-1"),
        std::make_tuple("::1/+1", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/+1"),
        std::make_tuple("::1/ 1 ", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/ 1 "),
        std::make_tuple("::1/word", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/word"),
        std::make_tuple("::1/::", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/::"),
        std::make_tuple("::1%scope/word", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1%scope/word"),
        
        std::make_tuple("2001:db8::/24", error_code::HAS_HOST_BITS_SET, "has host bits set in address 2001:db8::/24"),
        std::make_tuple("2001:db8:://", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 2001:db8:://"),
        std::make_tuple("2001:db8:://128", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 2001:db8:://128"),
        std::make_tuple("2001:db8::/128/128", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 2001:db8::/128/128"),

        std::make_tuple("/8", error_code::EMPTY_ADDRESS, "address cannot be empty"),
        std::make_tuple("google.com", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address google.com has invalid symbol"),
        std::make_tuple("1.2.3.4", error_code::LEAST_3_PARTS, "least 3 parts in address 1.2.3.4"),
        std::make_tuple("10/8", error_code::LEAST_3_PARTS, "least 3 parts in address 10/8"),
        std::make_tuple("1234:axy::b", error_code::PART_HAS_INVALID_SYMBOL, "in part 0 of address 1234:axy::b has invalid symbols"),
        std::make_tuple("google.com%scope", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address google.com%scope has invalid symbol"),
        std::make_tuple("1.2.3.4%scope", error_code::LEAST_3_PARTS, "least 3 parts in address 1.2.3.4%scope"),
        std::make_tuple("10%scope/8", error_code::LEAST_3_PARTS, "least 3 parts in address 10%scope/8"),
        std::make_tuple("1234:axy::b%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 0 of address 1234:axy::b%scope has invalid symbols")
    ));

TEST(ipv6_network, Comparison) {
    // TODO
}

using ToStringNetworkIpv6Params = TestWithParam<std::tuple<const char*, const char*, const char*, const char*>>;
TEST_P(ToStringNetworkIpv6Params, to_string) {
    const auto expected_full = std::get<1>(GetParam());
    const auto expected_compact = std::get<2>(GetParam());
    const auto expected_compressed = std::get<3>(GetParam());

    const auto actual = ipv6_network::parse(std::get<0>(GetParam()));

    std::ostringstream ss_full; ss_full << full << actual;
    std::ostringstream ss_default; ss_default << actual;
    std::ostringstream ss_compact; ss_compact << compact << actual;
    std::ostringstream ss_compressed; ss_compressed << compressed << actual;
    
    ASSERT_EQ(actual.to_string(format::full), std::string(expected_full));
    ASSERT_EQ(actual.to_string(format::compact), std::string(expected_compact));
    ASSERT_EQ(actual.to_string(format::compressed), std::string(expected_compressed));
    ASSERT_EQ(actual.to_string(), std::string(expected_compressed));
    ASSERT_EQ((std::string) actual, std::string(expected_compressed));
    ASSERT_EQ(std::to_string(actual), std::string(expected_compressed));
    ASSERT_EQ(ss_full.str(), std::string(expected_full));
    ASSERT_EQ(ss_default.str(), std::string(expected_compressed));
    ASSERT_EQ(ss_compact.str(), std::string(expected_compact));
    ASSERT_EQ(ss_compressed.str(), std::string(expected_compressed));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, ToStringNetworkIpv6Params,
    testing::Values(
        std::make_tuple("2001:db8::", "2001:0db8:0000:0000:0000:0000:0000:0000/128", "2001:db8:0:0:0:0:0:0/128", "2001:db8::/128"),
        std::make_tuple("2001:db8::/32", "2001:0db8:0000:0000:0000:0000:0000:0000/32", "2001:db8:0:0:0:0:0:0/32", "2001:db8::/32"),
        std::make_tuple("2001:db8::%scope/32", "2001:0db8:0000:0000:0000:0000:0000:0000%scope/32", "2001:db8:0:0:0:0:0:0%scope/32", "2001:db8::%scope/32")
    ));

TEST(ipv6_network, Hash) {
    auto net1 = ipv6_network::parse("2001:db8::");
    auto net2 = ipv6_network::parse("2001:db8::/128");
    auto net3 = ipv6_network::parse("2001:db8::/32");
    auto net4 = ipv6_network::parse("2001:db8::%scope/32");
    auto hash_functor = std::hash<ipv6_network>{};

    ASSERT_EQ(net1.hash(), sizeof(size_t) == 8 ? 15427398023827107304ULL : 3105424018U);
    ASSERT_EQ(net2.hash(), sizeof(size_t) == 8 ? 15427398023827107304ULL : 3105424018U);
    ASSERT_EQ(net3.hash(), sizeof(size_t) == 8 ? 15947883579910446492ULL : 3052474356U);
    ASSERT_EQ(net4.hash(), sizeof(size_t) == 8 ? 1368690913789066344ULL : 589600209U);

    ASSERT_EQ(hash_functor(net1), sizeof(size_t) == 8 ? 15427398023827107304ULL : 3105424018U);
    ASSERT_EQ(hash_functor(net2), sizeof(size_t) == 8 ? 15427398023827107304ULL : 3105424018U);
    ASSERT_EQ(hash_functor(net3), sizeof(size_t) == 8 ? 15947883579910446492ULL : 3052474356U);
    ASSERT_EQ(hash_functor(net4), sizeof(size_t) == 8 ? 1368690913789066344ULL : 589600209U);
}

TEST(ipv6_network, Containers) {
    auto net1 = ipv6_network::parse("2001:db8::");
    auto net2 = ipv6_network::parse("2001:db8::/64");
    auto net3 = ipv6_network::parse("2001:db8::/32");

    std::vector<ipv6_network> vec;
    vec.push_back(net1);
    vec.push_back(net2);
    vec.emplace_back(net3);
    ASSERT_EQ(vec[0], net1);
    ASSERT_EQ(vec[1], net2);
    ASSERT_EQ(vec[2], net3);

    std::map<ipv6_network, int> map;
    map[net2] = 2;
    map[net1] = 1;
    map[net3] = 3;
    auto it = map.begin();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(it++->first, net3);
    ASSERT_EQ(it++->first, net2);
    ASSERT_EQ(it++->first, net1);
    
    auto net3_with_scope = ipv6_network::parse("2001:db8::%scope/32");
    std::unordered_map<ipv6_network, int> unordered_map;
    unordered_map[net2] = 2;
    unordered_map[net1] = 1;
    unordered_map[net3] = 3;
    unordered_map[net3] = 4;
    unordered_map[net3_with_scope] = 0;
    ASSERT_EQ(unordered_map.size(), 4);
    ASSERT_EQ(unordered_map[net1], 1);
    ASSERT_EQ(unordered_map[net2], 2);
    ASSERT_EQ(unordered_map[net3], 4);
    ASSERT_EQ(unordered_map[net3_with_scope], 0);
}

TEST(ipv6_network, Swap) {
    auto net1 = ipv6_network::parse("2001:db8::");
    auto net2 = ipv6_network::parse("2001:db8::%scope/32");
    
    std::swap(net1, net2);

    ASSERT_EQ(net1, ipv6_network::parse("2001:db8::%scope/32"));
    ASSERT_EQ(net2, ipv6_network::parse("2001:db8::"));
}
