#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

template <size_t N>
static constexpr error_code get_parse_error(const char(&address)[N]) noexcept {
    error_code err = error_code::NO_ERROR;
    ipv4_address::parse(address, err);
    return err;
}

TEST(ipv4_address, CompileTime)
{
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    auto ip1 = ipv4_address::parse<"127.0.0.1">();
    ASSERT_EQ(ip1.to_uint32(), 0x7F000001);
    
    constexpr auto ip2 = ipv4_address::parse<"127.0.0.1">();
    constexpr auto ip2_uint32 = ip2.to_uint32();
    constexpr auto ip2_bytes = ip2.bytes();
    constexpr auto ip2_byte_0 = ip2_bytes[0];
    constexpr auto ip2_byte_1 = ip2_bytes.at(1);
    constexpr auto ip2_byte_2 = *(ip2_bytes.begin() + 2);
    constexpr auto ip2_byte_3 = ip2_bytes.back();
    ASSERT_EQ(ip2_uint32, 0x7F000001);
    ASSERT_EQ(ip2_byte_0, 0x7F);
    ASSERT_EQ(ip2_byte_1, 0x00);
    ASSERT_EQ(ip2_byte_2, 0x00);
    ASSERT_EQ(ip2_byte_3, 0x01);

    auto ip3 = ipv4_address::from_uint32<0x7F000001>();
    ASSERT_EQ(ip3.to_uint32(), 0x7F000001);

    constexpr auto ip4 = ipv4_address::from_uint32<0x7F000002>();
    constexpr auto ip4_uint32 = ip4.to_uint32();
    ASSERT_EQ(ip4_uint32, 0x7F000002);

    constexpr auto b1 = ip2 < ip4;
    constexpr auto b2 = ip2 > ip4;
    constexpr auto b3 = ip2 <= ip4;
    constexpr auto b4 = ip2 >= ip4;
    constexpr auto b5 = ip2 == ip4;
    constexpr auto b6 = ip2 != ip4;
    
    ASSERT_TRUE(b1);
    ASSERT_FALSE(b2);
    ASSERT_TRUE(b3);
    ASSERT_FALSE(b4);
    ASSERT_FALSE(b5);
    ASSERT_TRUE(b6);
#endif
    constexpr auto ip5 = ipv4_address::parse("127.0.0.1");
    constexpr auto ip5_uint32 = ip5.to_uint32();
    constexpr auto ip5_bytes = ip5.bytes();
    constexpr auto ip5_byte_0 = ip5_bytes[0];
    constexpr auto ip5_byte_1 = ip5_bytes.at(1);
    constexpr auto ip5_byte_2 = *(ip5_bytes.begin() + 2);
    constexpr auto ip5_byte_3 = ip5_bytes.back();
    ASSERT_EQ(ip5_uint32, 0x7F000001);
    ASSERT_EQ(ip5_byte_0, 0x7F);
    ASSERT_EQ(ip5_byte_1, 0x00);
    ASSERT_EQ(ip5_byte_2, 0x00);
    ASSERT_EQ(ip5_byte_3, 0x01);

    constexpr auto err = get_parse_error("127.0.0.256");
    ASSERT_EQ(err, error_code::OCTET_EXCEEDED_255);

    constexpr auto ip6 = ipv4_address::from_uint32(0x7F000002);
    constexpr auto ip6_uint32 = ip6.to_uint32();
    ASSERT_EQ(ip6_uint32, 0x7F000002);

    constexpr auto ip7 = ipv4_address::from_bytes({0xC0, 0xA8, 0x00, 0x01});
    constexpr auto ip7_uint32 = ip7.to_uint32();
    ASSERT_EQ(ip7_uint32, 0xC0A80001);

    constexpr uint8_t bytes[] = {0xC0, 0xA8, 0x00, 0x01};
    constexpr auto ip8 = ipv4_address::from_bytes(bytes, 3);
    constexpr auto ip8_uint32 = ip8.to_uint32();
    ASSERT_EQ(ip8_uint32, 0xC0A80000);

    constexpr auto b7 = ip5 < ip6;
    constexpr auto b8 = ip5 > ip6;
    constexpr auto b9 = ip5 <= ip6;
    constexpr auto b10 = ip5 >= ip6;
    constexpr auto b11 = ip5 == ip6;
    constexpr auto b12 = ip5 != ip6;
    
    ASSERT_TRUE(b7);
    ASSERT_FALSE(b8);
    ASSERT_TRUE(b9);
    ASSERT_FALSE(b10);
    ASSERT_FALSE(b11);
    ASSERT_TRUE(b12);
}

TEST(ipv4_address, DefaultCtor) {
    ipv4_address::base_type expected_empty { 0, 0, 0, 0};

    ipv4_address ip;
    
    EXPECT_EQ(ip.bytes(), expected_empty);
    EXPECT_EQ(ip.to_uint32(), 0);
    EXPECT_EQ(uint32_t(ip), 0);
    EXPECT_EQ(ip.size, 4);
    EXPECT_EQ(ip.version, version::V4);
}

TEST(ipv4_address, CopyCtor) {
    ipv4_address::base_type expected_ip { 127, 0, 0, 1};

    auto ip = ipv4_address::parse("127.0.0.1");
    auto ip_copy = ip;

    EXPECT_EQ(ip.bytes(), expected_ip);
    EXPECT_EQ(ip.to_uint32(), 0x7F000001);
    EXPECT_EQ(uint32_t(ip), 0x7F000001);

    EXPECT_EQ(ip_copy.bytes(), expected_ip);
    EXPECT_EQ(ip_copy.to_uint32(), 0x7F000001);
    EXPECT_EQ(uint32_t(ip_copy), 0x7F000001);
}

TEST(ipv4_address, CopyOperator) {
    ipv4_address::base_type expected_ip { 127, 0, 0, 1};

    auto ip = ipv4_address::parse("127.0.0.1");
    ipv4_address ip_copy;

    EXPECT_EQ(ip_copy.to_uint32(), 0);
    ip_copy = ip;

    EXPECT_EQ(ip.bytes(), expected_ip);
    EXPECT_EQ(ip.to_uint32(), 0x7F000001);
    EXPECT_EQ(uint32_t(ip), 0x7F000001);

    EXPECT_EQ(ip_copy.bytes(), expected_ip);
    EXPECT_EQ(ip_copy.to_uint32(), 0x7F000001);
    EXPECT_EQ(uint32_t(ip_copy), 0x7F000001);
}

using FromUint32Params = TestWithParam<std::tuple<uint32_t, const char*>>;
TEST_P(FromUint32Params, from_uint32) {
    ASSERT_EQ(ipv4_address::from_uint32(get<0>(GetParam())), ipv4_address::parse(get<1>(GetParam())));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, FromUint32Params,
    testing::Values(
            std::make_tuple(0, "0.0.0.0"),
            std::make_tuple(0xC0A80001, "192.168.0.1")
    ));

using FromBytesParams = TestWithParam<ipv4_address::base_type>;
TEST_P(FromBytesParams, from_bytes) {
    ASSERT_EQ(ipv4_address::from_bytes(GetParam()).bytes(), GetParam());
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, FromBytesParams,
    testing::Values(
        ipv4_address::base_type{0x00, 0x08, 0x00, 0x00},
        ipv4_address::base_type{0xC0, 0xA8, 0x00, 0x01}
    ));

using AddressParserParams = TestWithParam<std::tuple<const char*, uint32_t, std::array<uint8_t, 4>>>;
TEST_P(AddressParserParams, parse) {
    auto excepted_uint32 = get<1>(GetParam());
    auto excepted_bytes = get<2>(GetParam());

    auto ip = ipv4_address::parse(get<0>(GetParam()));
    auto actual_uint32 = ip.to_uint32();
    auto actual_bytes = ip.bytes();

    ASSERT_EQ(actual_uint32, excepted_uint32);
    ASSERT_EQ(actual_bytes[0], excepted_bytes[0]);
    ASSERT_EQ(actual_bytes[1], excepted_bytes[1]);
    ASSERT_EQ(actual_bytes[2], excepted_bytes[2]);
    ASSERT_EQ(actual_bytes[3], excepted_bytes[3]);

    std::string s1;
    ipv4_address addr;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> addr >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    ASSERT_EQ(addr.to_uint32(), excepted_uint32);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, AddressParserParams,
    testing::Values(
        std::make_tuple("0.0.0.0",     0x00000000, std::array<uint8_t, 4>{ 0x00, 0x00, 0x00, 0x00 }),
        std::make_tuple("100.64.0.0",  0x64400000, std::array<uint8_t, 4>{ 0x64, 0x40, 0x00, 0x00 }),
        std::make_tuple("127.0.0.1",   0x7F000001, std::array<uint8_t, 4>{ 0x7F, 0x00, 0x00, 0x01 }),
        std::make_tuple("192.168.1.1", 0xC0A80101, std::array<uint8_t, 4>{ 0xC0, 0xA8, 0x01, 0x01 })
    ));

using InvalidAddressParserParams = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidAddressParserParams, parse) {
    auto expected_address = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err;
    ipv4_address::parse(expected_address, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    ipv4_address::base_type expected_empty { 0, 0, 0, 0};
    auto error_ip = ipv4_address::parse(expected_address);

    EXPECT_EQ(error_ip.bytes(), expected_empty);
    ASSERT_EQ(error_ip.to_uint32(), 0);
    EXPECT_EQ(uint32_t(error_ip), 0);
#else
    EXPECT_THAT(
        [address=expected_address]() { ipv4_address::parse(address); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, InvalidAddressParserParams,
    testing::Values(
        std::make_tuple("", error_code::EMPTY_ADDRESS, "address cannot be empty"),

        std::make_tuple("127",             error_code::EXPECTED_4_OCTETS, "expected 4 octets in 127"),
        std::make_tuple("127.0",           error_code::EXPECTED_4_OCTETS, "expected 4 octets in 127.0"),
        std::make_tuple("127.0.0",         error_code::EXPECTED_4_OCTETS, "expected 4 octets in 127.0.0"),
        std::make_tuple("42.42.42.42.42",  error_code::EXPECTED_4_OCTETS, "expected 4 octets in 42.42.42.42.42"),
        std::make_tuple("192.168.0.1.com", error_code::EXPECTED_4_OCTETS, "expected 4 octets in 192.168.0.1.com"),
        std::make_tuple("42.42.42.42...",  error_code::EXPECTED_4_OCTETS, "expected 4 octets in 42.42.42.42..."),

        std::make_tuple("...42.42.42.42", error_code::EMPTY_OCTET, "empty octet 0 in address ...42.42.42.42"),
        std::make_tuple("42..42.42.42",   error_code::EMPTY_OCTET, "empty octet 1 in address 42..42.42.42"),
        std::make_tuple("42.42..42.42",   error_code::EMPTY_OCTET, "empty octet 2 in address 42.42..42.42"),
        std::make_tuple("42.42.42..42",   error_code::EMPTY_OCTET, "empty octet 3 in address 42.42.42..42"),
        std::make_tuple("42.42..42",      error_code::EMPTY_OCTET, "empty octet 2 in address 42.42..42"),
        std::make_tuple(".42.42.42.42",   error_code::EMPTY_OCTET, "empty octet 0 in address .42.42.42.42"),
        std::make_tuple(".",              error_code::EMPTY_OCTET, "empty octet 0 in address ."),
        std::make_tuple("42..42.42",      error_code::EMPTY_OCTET, "empty octet 1 in address 42..42.42"),
        std::make_tuple("...",            error_code::EMPTY_OCTET, "empty octet 0 in address ..."),
        std::make_tuple("127.0.0.",       error_code::EMPTY_OCTET, "empty octet 3 in address 127.0.0."),

        std::make_tuple("0x0a.0x0a.0x0a.0x0a", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address 0x0a.0x0a.0x0a.0x0a has invalid symbol"),
        std::make_tuple("0xa.0x0a.0x0a.0x0a",  error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address 0xa.0x0a.0x0a.0x0a has invalid symbol"),
        std::make_tuple("42.42.42.-0",         error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 3 of address 42.42.42.-0 has invalid symbol"),
        std::make_tuple("42.42.42.+0",         error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 3 of address 42.42.42.+0 has invalid symbol"),
        std::make_tuple("42.42.42.-42",        error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 3 of address 42.42.42.-42 has invalid symbol"),
        std::make_tuple("+1.+2.+3.4",          error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address +1.+2.+3.4 has invalid symbol"),
        std::make_tuple("1.2.3.4e0",           error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 3 of address 1.2.3.4e0 has invalid symbol"),
        std::make_tuple("1.2.3.4::",           error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 3 of address 1.2.3.4:: has invalid symbol"),
        std::make_tuple("1.a.2.3",             error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 1 of address 1.a.2.3 has invalid symbol"),
        std::make_tuple("127.0.0.1/24",        error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 3 of address 127.0.0.1/24 has invalid symbol"),
        
        std::make_tuple("1271.0.0.1",   error_code::OCTET_MORE_3_CHARACTERS, "in octet 0 of address 1271.0.0.1 more 3 characters"),
        std::make_tuple("127.1271.0.1", error_code::OCTET_MORE_3_CHARACTERS, "in octet 1 of address 127.1271.0.1 more 3 characters"),
        std::make_tuple("127.0.1271.1", error_code::OCTET_MORE_3_CHARACTERS, "in octet 2 of address 127.0.1271.1 more 3 characters"),
        std::make_tuple("127.0.0.1271", error_code::OCTET_MORE_3_CHARACTERS, "in octet 3 of address 127.0.0.1271 more 3 characters"),

        std::make_tuple("257.0.0.0",     error_code::OCTET_EXCEEDED_255, "octet 0 of address 257.0.0.0 exceeded 255"),
        std::make_tuple("127.258.0.1",   error_code::OCTET_EXCEEDED_255, "octet 1 of address 127.258.0.1 exceeded 255"),
        std::make_tuple("127.0.700.1",   error_code::OCTET_EXCEEDED_255, "octet 2 of address 127.0.700.1 exceeded 255"),
        std::make_tuple("192.168.0.999", error_code::OCTET_EXCEEDED_255, "octet 3 of address 192.168.0.999 exceeded 255"),

        std::make_tuple("000.000.000.000", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 0 of address 000.000.000.000"),
        std::make_tuple("192.168.000.001", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 2 of address 192.168.000.001"),
        std::make_tuple("016.016.016.016", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 0 of address 016.016.016.016"),
        std::make_tuple("001.000.008.016", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 0 of address 001.000.008.016"),
        std::make_tuple("01.2.3.40",       error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 0 of address 01.2.3.40"),
        std::make_tuple("1.02.3.40",       error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 1 of address 1.02.3.40"),
        std::make_tuple("1.2.03.40",       error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 2 of address 1.2.03.40"),
        std::make_tuple("1.2.3.040",       error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 3 of address 1.2.3.040")
    ));

TEST(ipv4_address, Comparison) {
    auto ip1 = ipv4_address::parse("127.239.0.1");
    auto ip2 = ipv4_address::parse("127.240.0.1");
    auto ip3 = ipv4_address::parse("127.240.0.1");
    
    ASSERT_TRUE(ip1 < ip2);
    ASSERT_TRUE(ip1 <= ip2);
    ASSERT_FALSE(ip1 > ip2);
    ASSERT_FALSE(ip1 >= ip2);
    ASSERT_FALSE(ip1 == ip2);
    ASSERT_TRUE(ip1 != ip2);
    
    ASSERT_FALSE(ip2 < ip1);
    ASSERT_FALSE(ip2 <= ip1);
    ASSERT_TRUE(ip2 > ip1);
    ASSERT_TRUE(ip2 >= ip1);
    ASSERT_FALSE(ip2 == ip1);
    ASSERT_TRUE(ip2 != ip1);
    
    ASSERT_FALSE(ip2 < ip3);
    ASSERT_TRUE(ip2 <= ip3);
    ASSERT_FALSE(ip2 > ip3);
    ASSERT_TRUE(ip2 >= ip3);
    ASSERT_TRUE(ip2 == ip3);
    ASSERT_FALSE(ip2 != ip3);
    
    ASSERT_FALSE(ip3 < ip2);
    ASSERT_TRUE(ip3 <= ip2);
    ASSERT_FALSE(ip3 > ip2);
    ASSERT_TRUE(ip3 >= ip2);
    ASSERT_TRUE(ip3 == ip2);
    ASSERT_FALSE(ip3 != ip2);
}

using ToStringParams = TestWithParam<const char*>;
TEST_P(ToStringParams, to_string) {
    const auto expected = GetParam();

    const auto actual = ipv4_address::parse(expected);

    std::ostringstream ss;
    ss << actual;

    ASSERT_EQ(actual.to_string(), std::string(expected));
    ASSERT_EQ(std::to_string(actual), std::string(expected));
    ASSERT_EQ(ss.str(), std::string(expected));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, ToStringParams,
    testing::Values(
        "0.0.0.0",
        "127.0.0.1",
        "255.0.42.42"
    ));

TEST(ipv4_address, Hash) {
    auto ip1 = ipv4_address::parse("127.0.0.1");
    auto ip2 = ipv4_address::parse("127.0.0.2");
    auto ip3 = ipv4_address::parse("127.0.0.3");
    auto hash_functor = std::hash<ipv4_address>{};

    ASSERT_EQ(ip1.hash(), 4785142202);
    ASSERT_EQ(ip2.hash(), 4785142203);
    ASSERT_EQ(ip3.hash(), 4785142204);

    ASSERT_EQ(hash_functor(ip1), 4785142202);
    ASSERT_EQ(hash_functor(ip2), 4785142203);
    ASSERT_EQ(hash_functor(ip3), 4785142204);
}

TEST(ipv4_address, Containers) {
    auto ip1 = ipv4_address::parse("127.0.0.1");
    auto ip2 = ipv4_address::parse("127.0.0.2");
    auto ip3 = ipv4_address::parse("127.0.0.3");

    std::vector<ipv4_address> vec;
    vec.push_back(ip1);
    vec.push_back(ip2);
    vec.emplace_back(ip3);
    vec.emplace_back(ipv4_address::base_type {127, 0, 0, 4});
    ASSERT_EQ(vec[0], ip1);
    ASSERT_EQ(vec[1], ip2);
    ASSERT_EQ(vec[2], ip3);
    ASSERT_EQ(vec[3], ipv4_address::parse("127.0.0.4"));

    std::map<ipv4_address, int> map;
    map[ip2] = 2;
    map[ip1] = 1;
    map[ip3] = 3;
    auto it = map.begin();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(it++->first, ip1);
    ASSERT_EQ(it++->first, ip2);
    ASSERT_EQ(it++->first, ip3);
    
    std::unordered_map<ipv4_address, int> unordered_map;
    unordered_map[ip2] = 2;
    unordered_map[ip1] = 1;
    unordered_map[ip3] = 3;
    unordered_map[ip3] = 4;
    ASSERT_EQ(unordered_map.size(), 3);
    ASSERT_EQ(unordered_map[ip1], 1);
    ASSERT_EQ(unordered_map[ip2], 2);
    ASSERT_EQ(unordered_map[ip3], 4);
}

TEST(ipv4_address, Swap) {
    auto ip1 = ipv4_address::parse("127.0.0.1");
    auto ip2 = ipv4_address::parse("127.0.0.2");
    
    std::swap(ip1, ip2);

    ASSERT_EQ(ip1, ipv4_address::parse("127.0.0.2"));
    ASSERT_EQ(ip2, ipv4_address::parse("127.0.0.1"));
}
