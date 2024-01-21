#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ipv4_address, CompileTime)
{
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    auto ipv4 = ipv4_address::parse<"127.0.0.255">();
    constexpr auto ipv4_ = ipv4_address::parse<"127.0.0.1">();
    constexpr auto u = ipv4_.to_uint32();

    auto ipv6 = ipv6_address::parse<"::ffff:0:0">();
    auto ipv6_2 = ipv6_address::parse("::ffff:0:0:127.0.0.255");
#endif
    try {
        error_code err;
        std::string addr = "127.0.0.255";
        const char* addr2 = "127.0.0.255";
        //constexpr std::string_view addr3 = "127.0.0.255";
        auto ss = ipv4_address::parse(addr, err);
        auto ss2 = ipv4_address::parse(addr2, err);
        //constexpr auto ss3 = ipv4_address::parse(addr3);

        auto ipv4_2 = ipv4_address::parse("127.0.0.255");
    
        auto ipv4_2_ = ipv4_address::parse("127.0.0.256", err);
        auto b = ipv4_2_;
    }
    catch (const std::exception& exc) {
        auto b = exc;
    }
    //constexpr auto d = ipv4;

    //constexpr auto ipv4 = parse<"127.0.0.1">();

    // ipv4_address::ip_from_string()

    constexpr auto ip1 = ipv4_address::from_uint32(0x7F000001);
    constexpr auto ip2 = ipv4_address::parse("127.0.0.1");
    constexpr auto cmp = ip1 == ip2;

    //constexpr auto bbbb = test();

    auto aaa = ip1;
}

TEST(ipv4_address, DefaultCtor) {
    ipv4_address::base_type expected_empty { 0, 0, 0, 0};

    ipv4_address ip;

    EXPECT_EQ(ip.bytes(), expected_empty);
    EXPECT_EQ(ip.to_uint32(), 0);
    EXPECT_EQ(uint32_t(ip), 0);
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
TEST_P(FromUint32Params, FromUint32) {
    ASSERT_EQ(ipv4_address::from_uint32(get<0>(GetParam())), ipv4_address::parse(get<1>(GetParam())));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, FromUint32Params,
    testing::Values(
            std::make_tuple(0, "0.0.0.0"),
            std::make_tuple(0xC0A80001, "192.168.0.1")
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
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, AddressParserParams,
    testing::Values(
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

        std::make_tuple("127", error_code::EXPECTED_4_OCTETS, "expected 4 octets in 127"),
        std::make_tuple("127.0", error_code::EXPECTED_4_OCTETS, "expected 4 octets in 127.0"),
        std::make_tuple("127.0.0", error_code::EXPECTED_4_OCTETS, "expected 4 octets in 127.0.0"),
        std::make_tuple("42.42.42.42.42", error_code::EXPECTED_4_OCTETS, "expected 4 octets in 42.42.42.42.42"),
        std::make_tuple("192.168.0.1.com", error_code::EXPECTED_4_OCTETS, "expected 4 octets in 192.168.0.1.com"),
        std::make_tuple("42.42.42.42...", error_code::EXPECTED_4_OCTETS, "expected 4 octets in 42.42.42.42..."),

        std::make_tuple("...42.42.42.42", error_code::EMPTY_OCTET, "empty octet 0 in address ...42.42.42.42"),
        std::make_tuple("42..42.42.42", error_code::EMPTY_OCTET, "empty octet 1 in address 42..42.42.42"),
        std::make_tuple("42.42..42.42", error_code::EMPTY_OCTET, "empty octet 2 in address 42.42..42.42"),
        std::make_tuple("42.42.42..42", error_code::EMPTY_OCTET, "empty octet 3 in address 42.42.42..42"),
        std::make_tuple("42.42..42", error_code::EMPTY_OCTET, "empty octet 2 in address 42.42..42"),
        std::make_tuple(".42.42.42.42", error_code::EMPTY_OCTET, "empty octet 0 in address .42.42.42.42"),
        std::make_tuple(".", error_code::EMPTY_OCTET, "empty octet 0 in address ."),
        std::make_tuple("42..42.42", error_code::EMPTY_OCTET, "empty octet 1 in address 42..42.42"),
        std::make_tuple("...", error_code::EMPTY_OCTET, "empty octet 0 in address ..."),
        std::make_tuple("127.0.0.", error_code::EMPTY_OCTET, "empty octet 3 in address 127.0.0."),

        std::make_tuple("0x0a.0x0a.0x0a.0x0a", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address 0x0a.0x0a.0x0a.0x0a has invalid symbol"),
        std::make_tuple("0xa.0x0a.0x0a.0x0a", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address 0xa.0x0a.0x0a.0x0a has invalid symbol"),
        std::make_tuple("42.42.42.-0", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 3 of address 42.42.42.-0 has invalid symbol"),
        std::make_tuple("42.42.42.+0", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 3 of address 42.42.42.+0 has invalid symbol"),
        std::make_tuple("42.42.42.-42", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 3 of address 42.42.42.-42 has invalid symbol"),
        std::make_tuple("+1.+2.+3.4", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address +1.+2.+3.4 has invalid symbol"),
        std::make_tuple("1.2.3.4e0", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 3 of address 1.2.3.4e0 has invalid symbol"),
        std::make_tuple("1.2.3.4::", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 3 of address 1.2.3.4:: has invalid symbol"),
        std::make_tuple("1.a.2.3", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 1 of address 1.a.2.3 has invalid symbol"),
        std::make_tuple("127.0.0.1/24", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 3 of address 127.0.0.1/24 has invalid symbol"),
        
        std::make_tuple("1271.0.0.1", error_code::OCTET_MORE_3_CHARACTERS, "in octet 0 of address 1271.0.0.1 more 3 characters"),
        std::make_tuple("127.1271.0.1", error_code::OCTET_MORE_3_CHARACTERS, "in octet 1 of address 127.1271.0.1 more 3 characters"),
        std::make_tuple("127.0.1271.1", error_code::OCTET_MORE_3_CHARACTERS, "in octet 2 of address 127.0.1271.1 more 3 characters"),
        std::make_tuple("127.0.0.1271", error_code::OCTET_MORE_3_CHARACTERS, "in octet 3 of address 127.0.0.1271 more 3 characters"),

        std::make_tuple("257.0.0.0", error_code::OCTET_EXCEEDED_255, "octet 0 of address 257.0.0.0 exceeded 255"),
        std::make_tuple("127.258.0.1", error_code::OCTET_EXCEEDED_255, "octet 1 of address 127.258.0.1 exceeded 255"),
        std::make_tuple("127.0.700.1", error_code::OCTET_EXCEEDED_255, "octet 2 of address 127.0.700.1 exceeded 255"),
        std::make_tuple("192.168.0.999", error_code::OCTET_EXCEEDED_255, "octet 3 of address 192.168.0.999 exceeded 255"),

        std::make_tuple("000.000.000.000", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 0 of address 000.000.000.000"),
        std::make_tuple("192.168.000.001", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 2 of address 192.168.000.001"),
        std::make_tuple("016.016.016.016", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 0 of address 016.016.016.016"),
        std::make_tuple("001.000.008.016", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 0 of address 001.000.008.016"),
        std::make_tuple("01.2.3.40", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 0 of address 01.2.3.40"),
        std::make_tuple("1.02.3.40", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 1 of address 1.02.3.40"),
        std::make_tuple("1.2.03.40", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 2 of address 1.2.03.40"),
        std::make_tuple("1.2.3.040", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 3 of address 1.2.3.040")
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
