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
    ipv6_address::parse(address, err);
    return err;
}

template <size_t N1, size_t N2>
static constexpr ipv6_address set_scope_id(const char(&address)[N1], const char(&scope)[N2]) noexcept {
    auto ip = ipv6_address::parse(address);
    ip.set_scope_id(scope);
    return ip;
}

TEST(ipv6_address, CompileTime) {
    constexpr ipv6_address::base_type ip_bytes { 0x20, 0x01, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    auto ip1 = ipv6_address::parse<"2001:db8::1">();
    ASSERT_EQ(ip1.bytes(), ip_bytes);

    constexpr auto ip2 = ipv6_address::parse<"2001:db8::1%1">();
    constexpr auto ip2_bytes = ip2.bytes();
    constexpr auto ip2_byte_0 = ip2_bytes[0];
    constexpr auto ip2_byte_1 = ip2_bytes.at(1);
    constexpr auto ip2_byte_2 = *(ip2_bytes.begin() + 2);
    constexpr auto ip2_byte_3 = ip2_bytes.back();
    ASSERT_EQ(ip2_byte_0, 0x20);
    ASSERT_EQ(ip2_byte_1, 0x01);
    ASSERT_EQ(ip2_byte_2, 0x0D);
    ASSERT_EQ(ip2_byte_3, 0x01);

    constexpr auto ip3 = ip2;
    constexpr auto ip3_bytes = ip3.bytes();
    constexpr auto ip3_byte_0 = ip3_bytes[0];
    constexpr auto ip3_byte_1 = ip3_bytes.at(1);
    constexpr auto ip3_byte_2 = *(ip3_bytes.begin() + 2);
    constexpr auto ip3_byte_3 = ip3_bytes.back();
    ASSERT_EQ(ip3_byte_0, 0x20);
    ASSERT_EQ(ip3_byte_1, 0x01);
    ASSERT_EQ(ip3_byte_2, 0x0D);
    ASSERT_EQ(ip3_byte_3, 0x01);

    constexpr auto ip4 = ipv6_address::parse<"2001:db8::1%2">();

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

    constexpr auto ip5 = ipv6_address::parse("2001:db8::1");
    constexpr auto ip5_bytes = ip5.bytes();
    constexpr auto ip5_byte_0 = ip5_bytes[0];
    constexpr auto ip5_byte_1 = ip5_bytes.at(1);
    constexpr auto ip5_byte_2 = *(ip5_bytes.begin() + 2);
    constexpr auto ip5_byte_3 = ip5_bytes.back();
    ASSERT_EQ(ip5_byte_0, 0x20);
    ASSERT_EQ(ip5_byte_1, 0x01);
    ASSERT_EQ(ip5_byte_2, 0x0D);
    ASSERT_EQ(ip5_byte_3, 0x01);

    constexpr auto err = get_parse_error("2001:db8::11111");
    ASSERT_EQ(err, error_code::PART_IS_MORE_4_CHARS);

    constexpr auto ip6 = ipv6_address::parse("2001:db8::1%test");

    constexpr auto ip7 = ipv6_address::from_bytes({ 0x20, 0x01, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 });
    constexpr auto ip7_bytes = ip7.bytes();
    ASSERT_EQ(ip7_bytes, ip_bytes);

    constexpr uint8_t bytes[] = { 0x20, 0x01, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
    constexpr auto ip8 = ipv6_address::from_bytes(bytes, 16);
    constexpr auto ip8_bytes = ip8.bytes();
    ASSERT_EQ(ip8_bytes, ip_bytes);

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

    constexpr auto ip9 = ipv6_address::parse("2001:db8::1%123");

    constexpr auto ip5_scope_id = ip5.get_scope_id();
    constexpr auto ip6_scope_id = ip6.get_scope_id();
    constexpr auto ip9_scope_id = ip9.get_scope_id();
    constexpr auto ip5_has_scope_id = (bool) ip5_scope_id;
    constexpr auto ip6_has_scope_id = (bool) ip6_scope_id;
    constexpr auto ip9_has_scope_id = (bool) ip9_scope_id;
    constexpr auto ip5_has_string = ip5_scope_id.has_string();
    constexpr auto ip6_has_string = ip6_scope_id.has_string();
    constexpr auto ip9_has_string = ip9_scope_id.has_string();
    constexpr auto ip5_has_uint32 = ip5_scope_id.has_uint32();
    constexpr auto ip6_has_uint32 = ip6_scope_id.has_uint32();
    constexpr auto ip9_has_uint32 = ip9_scope_id.has_uint32();
    constexpr auto ip5_get_uint32 = ip5_scope_id.get_uint32();
    constexpr auto ip6_get_uint32 = ip6_scope_id.get_uint32();
    constexpr auto ip9_get_uint32 = ip9_scope_id.get_uint32();
    constexpr auto ip5_uint32 = (uint32_t) ip5_scope_id;
    constexpr auto ip6_uint32 = (uint32_t) ip6_scope_id;
    constexpr auto ip9_uint32 = (uint32_t) ip9_scope_id;
    constexpr auto eq = ip6_scope_id == ip9_scope_id;
    constexpr auto ne = ip6_scope_id != ip9_scope_id;
    constexpr auto less = ip6_scope_id < ip9_scope_id;
    constexpr auto great = ip6_scope_id > ip9_scope_id;
    constexpr auto less_eq = ip6_scope_id <= ip9_scope_id;
    constexpr auto great_eq = ip6_scope_id >= ip9_scope_id;

    ASSERT_FALSE(ip5_has_scope_id);
    ASSERT_TRUE(ip6_has_scope_id);
    ASSERT_TRUE(ip9_has_scope_id);
    ASSERT_FALSE(ip5_has_string);
    ASSERT_TRUE(ip6_has_string);
    ASSERT_TRUE(ip9_has_string);
    ASSERT_FALSE(ip5_has_uint32);
    ASSERT_FALSE(ip6_has_uint32);
    ASSERT_TRUE(ip9_has_uint32);
    ASSERT_EQ(ip5_get_uint32, 0);
    ASSERT_EQ(ip6_get_uint32, 0);
    ASSERT_EQ(ip9_get_uint32, 123);
    ASSERT_EQ(ip5_uint32, 0);
    ASSERT_EQ(ip6_uint32, 0);
    ASSERT_EQ(ip9_uint32, 123);
    ASSERT_FALSE(eq);
    ASSERT_TRUE(ne);
    ASSERT_FALSE(less);
    ASSERT_TRUE(great);
    ASSERT_FALSE(less_eq);
    ASSERT_TRUE(great_eq);

    constexpr auto ip10 = set_scope_id("2001:db8::1", "5");
    constexpr auto ip10_scope = ip10.get_scope_id().get_uint32();
    ASSERT_EQ(ip10_scope, 5);

    constexpr auto ip11 = "2001:db8::1"_ipv6;
    constexpr auto ip12 = "0001:0002:0003:0004:0005:0006:0007:0008%123456789abcdefg"_ipv6;
    
    ASSERT_EQ(ip11, ipv6_address::parse("2001:db8::1"));
    ASSERT_EQ(ip12, ipv6_address::parse("1:2:3:4:5:6:7:8%123456789abcdefg"));
}

TEST(ipv6_address, DefaultCtor) {
    ipv6_address::base_type expected_empty {
         0, 0, 0, 0, 
         0, 0, 0, 0,
         0, 0, 0, 0,
         0, 0, 0, 0};

    ipv6_address ip;

    EXPECT_EQ(ip.bytes(), expected_empty);
    EXPECT_EQ(ip.size, 16);
    EXPECT_EQ(ip.version, ip_version::V6);
}

TEST(ipv6_address, CopyCtor) {
    ipv6_address::base_type expected_ip { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 255, 255,
        0, 0, 0, 0};

    auto ip = ipv6_address::parse("::ffff:0:0");
    auto ip_copy = ip;

    EXPECT_EQ(ip.bytes(), expected_ip);

    EXPECT_EQ(ip_copy.bytes(), expected_ip);
    EXPECT_EQ(ip_copy, ipv6_address(expected_ip));
}

TEST(ipv6_address, CopyOperator) {
    ipv6_address::base_type expected_null { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0};
    ipv6_address::base_type expected_ip { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 255, 255,
        0, 0, 0, 0};

    auto ip = ipv6_address::parse("::ffff:0:0");
    ipv6_address ip_copy;

    EXPECT_EQ(ip_copy.bytes(), expected_null);
    ip_copy = ip;

    EXPECT_EQ(ip.bytes(), expected_ip);
    EXPECT_EQ(ip_copy.bytes(), expected_ip);
}

using FromBytesIpv6Params = TestWithParam<std::tuple<ipv6_address::base_type, const char*>>;
TEST_P(FromBytesIpv6Params, from_bytes) {
    const auto& bytes = std::get<0>(GetParam());
    const auto scope = std::get<1>(GetParam());

    auto ip1 = ipv6_address::from_bytes(bytes);
    auto ip2 = ipv6_address::from_bytes(bytes.data(), bytes.size());

    EXPECT_EQ(ip1.bytes(), bytes);
    EXPECT_FALSE(ip1.get_scope_id());

    EXPECT_EQ(ip2.bytes(), bytes);
    EXPECT_FALSE(ip2.get_scope_id());

    ip1.set_scope_id(scope);
    ip2.set_scope_id("test");

    EXPECT_EQ(ip1.bytes(), bytes);
    EXPECT_EQ(std::string(ip1.get_scope_id()), scope);

    EXPECT_EQ(ip2.bytes(), bytes);
    EXPECT_EQ(std::string(ip2.get_scope_id()), "test");
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, FromBytesIpv6Params,
    testing::Values(
        std::make_tuple(ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, ""),
        std::make_tuple(ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, "eth2")
    ));

using AddressParserIpv6Params = TestWithParam<std::tuple<const char*, ipv6_address::base_type, bool, bool, const char*, uint32_t>>;
TEST_P(AddressParserIpv6Params, parse) {
    const auto& excepted_bytes = get<1>(GetParam());
    const auto excepted_scope_has_str = get<2>(GetParam());
    const auto excepted_scope_has_int = get<3>(GetParam());
    const auto excepted_scope_str = get<4>(GetParam());
    const auto excepted_scope_int = get<5>(GetParam());

    const auto ip = ipv6_address::parse(get<0>(GetParam()));
    const auto& actual_bytes = ip.bytes();
    auto actual_scope_id = ip.get_scope_id();

    auto actual_scope_has = (bool) actual_scope_id;
    auto actual_scope_has_str = actual_scope_id.has_string();
    auto actual_scope_has_int = actual_scope_id.has_uint32();
    auto actual_scope_str = actual_scope_id.get_string();
    auto actual_scope_int = actual_scope_id.get_uint32();
    std::string actual_scope_str_2 = actual_scope_id;
    uint32_t actual_scope_int_2 = actual_scope_id;

    ASSERT_EQ(actual_bytes, excepted_bytes);
    ASSERT_EQ(excepted_scope_has_str, actual_scope_has);
    ASSERT_EQ(excepted_scope_has_str, actual_scope_has_str);
    ASSERT_EQ(excepted_scope_has_int, actual_scope_has_int);
    ASSERT_EQ(excepted_scope_str, actual_scope_str);
    ASSERT_EQ(excepted_scope_int, actual_scope_int);
    ASSERT_EQ(excepted_scope_str, actual_scope_str_2);
    ASSERT_EQ(excepted_scope_int, actual_scope_int_2);

    std::string s1;
    ipv6_address addr;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> addr >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    ASSERT_EQ(addr, ip);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, AddressParserIpv6Params,
    testing::Values(
        std::make_tuple("2001:db8:0:0:1:0:0:1", ipv6_address::base_type{ 0x20, 0x01, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("2001:DB8::1", ipv6_address::base_type{ 0x20, 0x01, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("2001:db8::1", ipv6_address::base_type{ 0x20, 0x01, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("2001:0db8:85a3:0000:0000:8a2e:0370:7334", ipv6_address::base_type{ 0x20, 0x01, 0x0D, 0xB8, 0x85, 0xa3, 0x00, 0x00, 0x00, 0x00, 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34 }, false, false, "", 0),
        std::make_tuple("fe80::1ff:fe23:4567:890a", ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, false, false, "", 0),
        std::make_tuple("::", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("::127.0.0.1", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("0000::0000", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("::c0a8:1", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("000::c0a8:0001", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("::1", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("::ffff:0:0", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("::ffff:0:0:0", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("64:ff9b::", ipv6_address::base_type{ 0x00, 0x64, 0xff, 0x9b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("64:ff9b:1::", ipv6_address::base_type{ 0x00, 0x64, 0xff, 0x9b, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("100::", ipv6_address::base_type{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("ff02::1:3", ipv6_address::base_type{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03 }, false, false, "", 0),
        std::make_tuple("fe80::1ff:fe23:4567:890a%eth2", ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, true, false, "eth2", 0),
        std::make_tuple("fe80::1ff:fe23:4567:890a%25eth01234567", ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, true, false, "25eth01234567", 0),
        std::make_tuple("fe80::1ff:fe23:4567:890a%3", ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, true, true, "3", 3),
        std::make_tuple("fe80::1ff:fe23:4567:890a%31", ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, true, true, "31", 31),
        std::make_tuple("1:2:3:4:5:6:42.42.42.1", ipv6_address::base_type{ 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x2A, 0x2A, 0x2A, 0x01 }, false, false, "", 0)
    ));

using InvalidAddressIpv6Params = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidAddressIpv6Params, parse) {
    auto expected_address = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err;
    ipv6_address::parse(expected_address, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    ipv6_address::base_type expected_empty { 0 };
    auto error_ip = ipv6_address::parse(expected_address);

    EXPECT_EQ(error_ip.bytes(), expected_empty);
#else
    EXPECT_THAT(
        [address=expected_address]() { ipv6_address::parse(address); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
    EXPECT_THAT(
        [address=expected_address]() { ipv6_address::parse(address); },
        Throws<parse_error>(Property(&parse_error::code, Eq(expected_error_code))));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, InvalidAddressIpv6Params,
    testing::Values(
        std::make_tuple("", error_code::EMPTY_ADDRESS, "address cannot be empty"),

        std::make_tuple(":", error_code::LEAST_3_PARTS, "least 3 parts in address :"),
        std::make_tuple(":1", error_code::LEAST_3_PARTS, "least 3 parts in address :1"),
        std::make_tuple("FEDC:9878", error_code::LEAST_3_PARTS, "least 3 parts in address FEDC:9878"),
        std::make_tuple(":%scope", error_code::LEAST_3_PARTS, "least 3 parts in address :%scope"),
        std::make_tuple(":1%scope", error_code::LEAST_3_PARTS, "least 3 parts in address :1%scope"),
        std::make_tuple("FEDC:9878%scope", error_code::LEAST_3_PARTS, "least 3 parts in address FEDC:9878%scope"),

        std::make_tuple("10:9:8:7:6:5:4:3:2:1", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:2:1"),
        std::make_tuple("10:9:8:7:6:5:4:3:42.42.42.42", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:42.42.42.42"),
        std::make_tuple("1:2:3:4:5:6:7:42.42.42.42", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 1:2:3:4:5:6:7:42.42.42.42"),
        std::make_tuple("10:9:8:7:6:5:4:3:2:1%scope", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:2:1%scope"),
        std::make_tuple("10:9:8:7:6:5:4:3:42.42.42.42%scope", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:42.42.42.42%scope"),

        std::make_tuple("3ffe::1::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1"),
        std::make_tuple("1::2::3::4:5", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 1::2::3::4:5"),
        std::make_tuple("2001::db:::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001::db:::1"),
        std::make_tuple("3ffe::1::", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::"),
        std::make_tuple("::3ffe::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address ::3ffe::1"),
        std::make_tuple(":3ffe::1::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1"),
        std::make_tuple("3ffe::1::1:", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1:"),
        std::make_tuple(":3ffe::1::1:", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1:"),
        std::make_tuple(":::", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :::"),
        std::make_tuple("2001:db8:::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1"),
        std::make_tuple("3ffe::1::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1%scope"),
        std::make_tuple("1::2::3::4:5%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 1::2::3::4:5%scope"),
        std::make_tuple("2001::db:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001::db:::1%scope"),
        std::make_tuple("3ffe::1::%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::%scope"),
        std::make_tuple("::3ffe::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address ::3ffe::1%scope"),
        std::make_tuple(":3ffe::1::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1%scope"),
        std::make_tuple("3ffe::1::1:%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1:%scope"),
        std::make_tuple(":3ffe::1::1:%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1:%scope"),
        std::make_tuple(":::%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :::%scope"),
        std::make_tuple("2001:db8:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1%scope"),

        std::make_tuple("::00000", error_code::PART_IS_MORE_4_CHARS, "in part 2 of address ::00000 more 4 characters"),
        std::make_tuple("3ffe::10000", error_code::PART_IS_MORE_4_CHARS, "in part 2 of address 3ffe::10000 more 4 characters"),
        std::make_tuple("02001:db8::", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 02001:db8:: more 4 characters"),
        std::make_tuple("2001:888888::1", error_code::PART_IS_MORE_4_CHARS, "in part 1 of address 2001:888888::1 more 4 characters"),
        std::make_tuple("::00000%scope", error_code::PART_IS_MORE_4_CHARS, "in part 2 of address ::00000%scope more 4 characters"),
        std::make_tuple("3ffe::10000%scope", error_code::PART_IS_MORE_4_CHARS, "in part 2 of address 3ffe::10000%scope more 4 characters"),
        std::make_tuple("02001:db8::%scope", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 02001:db8::%scope more 4 characters"),
        std::make_tuple("2001:888888::1%scope", error_code::PART_IS_MORE_4_CHARS, "in part 1 of address 2001:888888::1%scope more 4 characters"),
        std::make_tuple("1.2.3.4::%scope", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 1.2.3.4::%scope more 4 characters"),
        std::make_tuple("1.2.3.4::", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 1.2.3.4:: more 4 characters"),
        std::make_tuple("::1.2.3.4:%scope", error_code::PART_IS_MORE_4_CHARS, "in part 2 of address ::1.2.3.4:%scope more 4 characters"),
        std::make_tuple("::1.2.3.4.5.6.7.8.9", error_code::PART_IS_MORE_4_CHARS, "in part 2 of address ::1.2.3.4.5.6.7.8.9 more 4 characters"),
        std::make_tuple("123456789abcfeh::", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 123456789abcfeh:: more 4 characters"),
        std::make_tuple("123456789abcfe::", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 123456789abcfe:: more 4 characters"),

        std::make_tuple("3ffe::1.1.1.net", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 2 of address 3ffe::1.1.1.net has invalid symbol"),
        std::make_tuple("3ffe::1.1.1.net%scope", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 2 of address 3ffe::1.1.1.net%scope has invalid symbol"),
        std::make_tuple("3ffe::goog", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::goog has invalid symbols"),
        std::make_tuple("3ffe::-0", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::-0 has invalid symbols"),
        std::make_tuple("3ffe::+0", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::+0 has invalid symbols"),
        std::make_tuple("3ffe::-1", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::-1 has invalid symbols"),
        std::make_tuple("1234:axy::b", error_code::PART_HAS_INVALID_SYMBOL, "in part 4 of address 1234:axy::b has invalid symbols"),
        std::make_tuple("3ffe::goog%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::goog%scope has invalid symbols"),
        std::make_tuple("3ffe::-0%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::-0%scope has invalid symbols"),
        std::make_tuple("3ffe::+0%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::+0%scope has invalid symbols"),
        std::make_tuple("3ffe::-1%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::-1%scope has invalid symbols"),
        std::make_tuple("1234:axy::b%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 4 of address 1234:axy::b%scope has invalid symbols"),
        std::make_tuple("::1/24", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address ::1/24 has invalid symbols"),

        std::make_tuple("3ffe::1::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1"),
        std::make_tuple("1::2::3::4:5", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 1::2::3::4:5"),
        std::make_tuple("2001::db:::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001::db:::1"),
        std::make_tuple("3ffe::1::", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::"),
        std::make_tuple("::3ffe::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address ::3ffe::1"),
        std::make_tuple(":3ffe::1::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1"),
        std::make_tuple("3ffe::1::1:", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1:"),
        std::make_tuple(":3ffe::1::1:", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1:"),
        std::make_tuple(":::", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :::"),
        std::make_tuple("2001:db8:::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1"),
        std::make_tuple("3ffe::1::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1%scope"),
        std::make_tuple("1::2::3::4:5%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 1::2::3::4:5%scope"),
        std::make_tuple("2001::db:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001::db:::1%scope"),
        std::make_tuple("3ffe::1::%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::%scope"),
        std::make_tuple("::3ffe::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address ::3ffe::1%scope"),
        std::make_tuple(":3ffe::1::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1%scope"),
        std::make_tuple("3ffe::1::1:%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1:%scope"),
        std::make_tuple(":3ffe::1::1:%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1:%scope"),
        std::make_tuple(":::%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :::%scope"),
        std::make_tuple("2001:db8:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1%scope"),

        std::make_tuple(":2001:db8::1", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :2001:db8::1"),
        std::make_tuple(":1:2:3:4:5:6:7", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :1:2:3:4:5:6:7"),
        std::make_tuple(":1:2:3:4:5:6:", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :1:2:3:4:5:6:"),
        std::make_tuple(":6:5:4:3:2:1::", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :6:5:4:3:2:1::"),
        std::make_tuple(":2001:db8::1%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :2001:db8::1%scope"),
        std::make_tuple(":1:2:3:4:5:6:7%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :1:2:3:4:5:6:7%scope"),
        std::make_tuple(":1:2:3:4:5:6:%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :1:2:3:4:5:6:%scope"),
        std::make_tuple(":6:5:4:3:2:1::%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :6:5:4:3:2:1::%scope"),

        std::make_tuple("2001:db8::1:", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 2001:db8::1:"),
        std::make_tuple("1:2:3:4:5:6:7:", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 1:2:3:4:5:6:7:"),
        std::make_tuple("::7:6:5:4:3:2:", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address ::7:6:5:4:3:2:"),
        std::make_tuple("2001:db8::1:%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 2001:db8::1:%scope"),
        std::make_tuple("1:2:3:4:5:6:7:%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 1:2:3:4:5:6:7:%scope"),
        std::make_tuple("::7:6:5:4:3:2:%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address ::7:6:5:4:3:2:%scope"),
        std::make_tuple("8:7:6:5:4:3:2:1::", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 8:7:6:5:4:3:2:1::"),
        std::make_tuple("8:7:6:5:4:3:2:1::%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 8:7:6:5:4:3:2:1::%scope"),

        std::make_tuple("9:8:7:6:5:4:3::2:1", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 9:8:7:6:5:4:3::2:1"),
        std::make_tuple("::8:7:6:5:4:3:2:1", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address ::8:7:6:5:4:3:2:1"),
        std::make_tuple("8:7:6:5:4:3::2:1", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 8:7:6:5:4:3::2:1"),
        std::make_tuple("9:8:7:6:5:4:3::2:1%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 9:8:7:6:5:4:3::2:1%scope"),
        std::make_tuple("::8:7:6:5:4:3:2:1%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address ::8:7:6:5:4:3:2:1%scope"),
        std::make_tuple("1:2:3:4:5::6:7:8", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 1:2:3:4:5::6:7:8"),
        std::make_tuple("1:2:3:4:5::6:7:8%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 1:2:3:4:5::6:7:8%scope"),

        std::make_tuple("1:2:3:4:5:6:7:8:", error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON, "exactly 8 parts expected without '::' in address 1:2:3:4:5:6:7:8:"),
        std::make_tuple("7:6:5:4:3:2:1", error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON, "exactly 8 parts expected without '::' in address 7:6:5:4:3:2:1"),
        std::make_tuple("7:6:5:4:3:2:1%scope", error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON, "exactly 8 parts expected without '::' in address 7:6:5:4:3:2:1%scope"),
        
        std::make_tuple("::%123456789abcdefgh", error_code::SCOPE_ID_IS_TOO_LONG, "scope id is too long in address ::%123456789abcdefgh"),

        std::make_tuple("::1%", error_code::INVALID_SCOPE_ID, "invalid scope id in address ::1%"),
        std::make_tuple("::1%scope%", error_code::INVALID_SCOPE_ID, "invalid scope id in address ::1%scope%"),
        std::make_tuple("::1%scope_id/24", error_code::INVALID_SCOPE_ID, "invalid scope id in address ::1%scope_id/24") 
    ));

TEST(ipv6_address, Comparison) {
    auto ip1 = ipv6_address::parse("2001:db8::1");
    auto ip2 = ipv6_address::parse("2001:db8::2");
    auto ip3 = ipv6_address::parse("2001:db8::2");
    auto ip4 = ipv6_address::parse("2001:db8::2%abc");
    auto ip5 = ipv6_address::parse("2001:db8::2%abc");
    auto ip6 = ipv6_address::parse("2001:db8::2%acc");

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

    ASSERT_TRUE(ip3 < ip4);
    ASSERT_TRUE(ip3 <= ip4);
    ASSERT_FALSE(ip3 > ip4);
    ASSERT_FALSE(ip3 >= ip4);
    ASSERT_FALSE(ip3 == ip4);
    ASSERT_TRUE(ip3 != ip4);
    
    ASSERT_FALSE(ip4 < ip3);
    ASSERT_FALSE(ip4 <= ip3);
    ASSERT_TRUE(ip4 > ip3);
    ASSERT_TRUE(ip4 >= ip3);
    ASSERT_FALSE(ip4 == ip3);
    ASSERT_TRUE(ip4 != ip3);

    ASSERT_FALSE(ip4 < ip5);
    ASSERT_TRUE(ip4 <= ip5);
    ASSERT_FALSE(ip4 > ip5);
    ASSERT_TRUE(ip4 >= ip5);
    ASSERT_TRUE(ip4 == ip5);
    ASSERT_FALSE(ip4 != ip5);
    
    ASSERT_FALSE(ip5 < ip4);
    ASSERT_TRUE(ip5 <= ip4);
    ASSERT_FALSE(ip5 > ip4);
    ASSERT_TRUE(ip5 >= ip4);
    ASSERT_TRUE(ip5 == ip4);
    ASSERT_FALSE(ip5 != ip4);

    ASSERT_TRUE(ip5 < ip6);
    ASSERT_TRUE(ip5 <= ip6);
    ASSERT_FALSE(ip5 > ip6);
    ASSERT_FALSE(ip5 >= ip6);
    ASSERT_FALSE(ip5 == ip6);
    ASSERT_TRUE(ip5 != ip6);
    
    ASSERT_FALSE(ip6 < ip5);
    ASSERT_FALSE(ip6 <= ip5);
    ASSERT_TRUE(ip6 > ip5);
    ASSERT_TRUE(ip6 >= ip5);
    ASSERT_FALSE(ip6 == ip5);
    ASSERT_TRUE(ip6 != ip5);
}

using ToStringIpv6Params = TestWithParam<std::tuple<const char*, const char*, const char*, const char*>>;
TEST_P(ToStringIpv6Params, to_string) {
    const auto expected_address = std::get<0>(GetParam());
    const auto expected_full = std::get<1>(GetParam());
    const auto expected_compact = std::get<2>(GetParam());
    const auto expected_compressed = std::get<3>(GetParam());

    const auto actual = ipv6_address::parse(expected_address);

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
    ipv6_address, ToStringIpv6Params,
    testing::Values(
        std::make_tuple("2001:db8:0:0:1:0:0:1", "2001:0db8:0000:0000:0001:0000:0000:0001", "2001:db8:0:0:1:0:0:1", "2001:db8::1:0:0:1"),
        std::make_tuple("2001:DB8::1", "2001:0db8:0000:0000:0000:0000:0000:0001", "2001:db8:0:0:0:0:0:1", "2001:db8::1"),
        std::make_tuple("2001:db8::1", "2001:0db8:0000:0000:0000:0000:0000:0001", "2001:db8:0:0:0:0:0:1", "2001:db8::1"),
        std::make_tuple("2001:0db8:85a3:0000:0000:8a2e:0370:7334", "2001:0db8:85a3:0000:0000:8a2e:0370:7334", "2001:db8:85a3:0:0:8a2e:370:7334", "2001:db8:85a3::8a2e:370:7334"),
        std::make_tuple("fe80::1ff:fe23:4567:890a", "fe80:0000:0000:0000:01ff:fe23:4567:890a", "fe80:0:0:0:1ff:fe23:4567:890a", "fe80::1ff:fe23:4567:890a"),
        std::make_tuple("::", "0000:0000:0000:0000:0000:0000:0000:0000", "0:0:0:0:0:0:0:0", "::"),
        std::make_tuple("::127.0.0.1", "0000:0000:0000:0000:0000:0000:7f00:0001", "0:0:0:0:0:0:7f00:1", "::7f00:1"),
        std::make_tuple("0000::0000", "0000:0000:0000:0000:0000:0000:0000:0000", "0:0:0:0:0:0:0:0", "::"),
        std::make_tuple("::c0a8:1", "0000:0000:0000:0000:0000:0000:c0a8:0001", "0:0:0:0:0:0:c0a8:1", "::c0a8:1"),
        std::make_tuple("000::c0a8:0001", "0000:0000:0000:0000:0000:0000:c0a8:0001", "0:0:0:0:0:0:c0a8:1", "::c0a8:1"),
        std::make_tuple("::1", "0000:0000:0000:0000:0000:0000:0000:0001", "0:0:0:0:0:0:0:1", "::1"),
        std::make_tuple("::ffff:0:0", "0000:0000:0000:0000:0000:ffff:0000:0000", "0:0:0:0:0:ffff:0:0", "::ffff:0:0"),
        std::make_tuple("::ffff:0:0:0", "0000:0000:0000:0000:ffff:0000:0000:0000", "0:0:0:0:ffff:0:0:0", "::ffff:0:0:0"),
        std::make_tuple("64:ff9b::", "0064:ff9b:0000:0000:0000:0000:0000:0000", "64:ff9b:0:0:0:0:0:0", "64:ff9b::"),
        std::make_tuple("64:ff9b:1::", "0064:ff9b:0001:0000:0000:0000:0000:0000", "64:ff9b:1:0:0:0:0:0", "64:ff9b:1::"),
        std::make_tuple("100::", "0100:0000:0000:0000:0000:0000:0000:0000", "100:0:0:0:0:0:0:0", "100::"),
        std::make_tuple("ff02::1:3", "ff02:0000:0000:0000:0000:0000:0001:0003", "ff02:0:0:0:0:0:1:3", "ff02::1:3"),
        std::make_tuple("fe80::1ff:fe23:4567:890a%eth2", "fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2", "fe80:0:0:0:1ff:fe23:4567:890a%eth2", "fe80::1ff:fe23:4567:890a%eth2"),
        std::make_tuple("fe80::1ff:fe23:4567:890a%25eth01234567", "fe80:0000:0000:0000:01ff:fe23:4567:890a%25eth01234567", "fe80:0:0:0:1ff:fe23:4567:890a%25eth01234567", "fe80::1ff:fe23:4567:890a%25eth01234567"),
        std::make_tuple("fe80::1ff:fe23:4567:890a%3", "fe80:0000:0000:0000:01ff:fe23:4567:890a%3", "fe80:0:0:0:1ff:fe23:4567:890a%3", "fe80::1ff:fe23:4567:890a%3"),
        std::make_tuple("fe80::1ff:fe23:4567:890a%31", "fe80:0000:0000:0000:01ff:fe23:4567:890a%31", "fe80:0:0:0:1ff:fe23:4567:890a%31", "fe80::1ff:fe23:4567:890a%31"),
        std::make_tuple("1:2:3:4:5:6:42.42.42.1", "0001:0002:0003:0004:0005:0006:2a2a:2a01", "1:2:3:4:5:6:2a2a:2a01", "1:2:3:4:5:6:2a2a:2a01"),
        std::make_tuple("0:0:0:4:5:6:42.42.42.1%test", "0000:0000:0000:0004:0005:0006:2a2a:2a01%test", "0:0:0:4:5:6:2a2a:2a01%test", "::4:5:6:2a2a:2a01%test")
    ));

TEST(ipv6_address, Hash) {
    auto ip1 = ipv6_address::parse("2001:db8::1");
    auto ip2 = ipv6_address::parse("2001:db8::2");
    auto ip3 = ipv6_address::parse("2001:db8::3");
    auto ip3_with_scope = ipv6_address::parse("2001:db8::3%scope");
    auto hash_functor = std::hash<ipv6_address>{};

    ASSERT_EQ(ip1.hash(), sizeof(size_t) == 8 ? 6222153021643990098ULL : 1539407693U);
    ASSERT_EQ(ip2.hash(), sizeof(size_t) == 8 ? 6116520725720747177ULL : 2230264075U);
    ASSERT_EQ(ip3.hash(), sizeof(size_t) == 8 ? 16584966935191602359ULL : 4145152766U);
    ASSERT_EQ(ip3_with_scope.hash(), sizeof(size_t) == 8 ? 460899037339188753ULL : 844563635U);

    ASSERT_EQ(hash_functor(ip1), sizeof(size_t) == 8 ? 6222153021643990098ULL : 1539407693U);
    ASSERT_EQ(hash_functor(ip2), sizeof(size_t) == 8 ? 6116520725720747177ULL : 2230264075U);
    ASSERT_EQ(hash_functor(ip3), sizeof(size_t) == 8 ? 16584966935191602359ULL : 4145152766U);
    ASSERT_EQ(hash_functor(ip3_with_scope), sizeof(size_t) == 8 ? 460899037339188753ULL : 844563635U);
}

TEST(ipv6_address, Containers) {
    auto ip1 = ipv6_address::parse("2001:db8::1");
    auto ip2 = ipv6_address::parse("2001:db8::2");
    auto ip3 = ipv6_address::parse("2001:db8::3");

    std::vector<ipv6_address> vec;
    vec.push_back(ip1);
    vec.push_back(ip2);
    vec.emplace_back(ip3);
    vec.emplace_back(ipv6_address::base_type {0x20, 0x01, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04 });
    ASSERT_EQ(vec[0], ip1);
    ASSERT_EQ(vec[1], ip2);
    ASSERT_EQ(vec[2], ip3);
    ASSERT_EQ(vec[3], ipv6_address::parse("2001:db8::4"));

    std::map<ipv6_address, int> map;
    map[ip2] = 2;
    map[ip1] = 1;
    map[ip3] = 3;
    auto it = map.begin();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(it++->first, ip1);
    ASSERT_EQ(it++->first, ip2);
    ASSERT_EQ(it++->first, ip3);
    
    auto ip3_with_scope = ipv6_address::parse("2001:db8::3%scope");
    std::unordered_map<ipv6_address, int> unordered_map;
    unordered_map[ip2] = 2;
    unordered_map[ip1] = 1;
    unordered_map[ip3] = 3;
    unordered_map[ip3] = 4;
    unordered_map[ip3_with_scope] = 0;
    ASSERT_EQ(unordered_map.size(), 4);
    ASSERT_EQ(unordered_map[ip1], 1);
    ASSERT_EQ(unordered_map[ip2], 2);
    ASSERT_EQ(unordered_map[ip3], 4);
    ASSERT_EQ(unordered_map[ip3_with_scope], 0);
}

TEST(ipv6_address, Swap) {
    auto ip1 = ipv6_address::parse("2001:db8::1");
    auto ip2 = ipv6_address::parse("2001:db8::2%test");
    
    std::swap(ip1, ip2);

    ASSERT_EQ(ip1, ipv6_address::parse("2001:db8::2%test"));
    ASSERT_EQ(ip2, ipv6_address::parse("2001:db8::1"));
}

TEST(ipv6_address, reverse_pointer) {
    auto ip = ipv6_address::parse("2001:db8::1");
    
    const auto actual = ip.reverse_pointer();

    ASSERT_EQ(actual, "1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.8.b.d.0.1.0.0.2.ip6.arpa");
}

TEST(ipv6_address, literals) {
    auto ip1 = "2001:db8::1"_ipv6;
    auto ip2 = "0001:0002:0003:0004:0005:0006:0007:0008%123456789abcdefg"_ipv6;

    ASSERT_EQ(ip1, ipv6_address::parse("2001:db8::1"));
    ASSERT_EQ(ip2, ipv6_address::parse("1:2:3:4:5:6:7:8%123456789abcdefg"));
}

using IsMulticastIpv6Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsMulticastIpv6Params, is_multicast) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_address::parse(std::get<0>(GetParam())).is_multicast();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, IsMulticastIpv6Params,
    testing::Values(
        std::make_tuple("ffff::", true),
        std::make_tuple("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", true),
        std::make_tuple("ff00::", true),
        std::make_tuple("fdff::", false)
    ));

using IsPrivateIpv6Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsPrivateIpv6Params, is_private) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_address::parse(std::get<0>(GetParam())).is_private();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, IsPrivateIpv6Params,
    testing::Values(
        std::make_tuple("fc00::", true),
        std::make_tuple("fc00:ffff:ffff:ffff::", true),
        std::make_tuple("fbff:ffff::", false),
        std::make_tuple("fe00::", false),
        std::make_tuple("::ffff:192.168.1.1", true),
        std::make_tuple("::ffff:172.32.0.0", false)
    ));

using IsGlobalIpv6Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsGlobalIpv6Params, is_global) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_address::parse(std::get<0>(GetParam())).is_global();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, IsGlobalIpv6Params,
    testing::Values(
        std::make_tuple("200::1", true)
    ));

using IsReservedIpv6Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsReservedIpv6Params, is_reserved) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_address::parse(std::get<0>(GetParam())).is_reserved();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, IsReservedIpv6Params,
    testing::Values(
        std::make_tuple("100::", true)
    ));

using IsLoopbackIpv6Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsLoopbackIpv6Params, is_loopback) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_address::parse(std::get<0>(GetParam())).is_loopback();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, IsLoopbackIpv6Params,
    testing::Values(
        std::make_tuple("0:0::0:01", true),
        std::make_tuple("::1", true),
        std::make_tuple("::2", false)
    ));

using IsLinkLocalIpv6Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsLinkLocalIpv6Params, is_link_local) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_address::parse(std::get<0>(GetParam())).is_link_local();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, IsLinkLocalIpv6Params,
    testing::Values(
        std::make_tuple("fea0::", true),
        std::make_tuple("febf:ffff::", true),
        std::make_tuple("fe7f:ffff::", false),
        std::make_tuple("fec0::", false)
    ));

using IsUnspecifiedIpv6Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsUnspecifiedIpv6Params, is_unspecified) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_address::parse(std::get<0>(GetParam())).is_unspecified();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, IsUnspecifiedIpv6Params,
    testing::Values(
        std::make_tuple("0::0", true),
        std::make_tuple("::1", false)
    ));

using IsSiteLocalIpv6Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsSiteLocalIpv6Params, is_site_local) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_address::parse(std::get<0>(GetParam())).is_site_local();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, IsSiteLocalIpv6Params,
    testing::Values(
        std::make_tuple("fecf::", true),
        std::make_tuple("feff:ffff:ffff:ffff::", true),
        std::make_tuple("fbf:ffff::", false),
        std::make_tuple("ff00::", false)
    ));

using Ipv4MappedIpv6Params = TestWithParam<std::tuple<const char*, const char*, bool>>;
TEST_P(Ipv4MappedIpv6Params, ipv4_mapped) {
    const auto expected_ipv4 = std::get<1>(GetParam());
    const auto expected_has_ipv4 = std::get<2>(GetParam());

    const auto actual = ipv6_address::parse(std::get<0>(GetParam())).ipv4_mapped();

    ASSERT_EQ(actual.has_value(), expected_has_ipv4);
    ASSERT_EQ(actual.value(), ipv4_address::parse(expected_ipv4));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, Ipv4MappedIpv6Params,
    testing::Values(
        std::make_tuple("::ffff:192.168.1.1%test", "192.168.1.1", true),
        std::make_tuple("::c0a8:101", "0.0.0.0", false),
        std::make_tuple("::ffff:c0a8:101", "192.168.1.1", true)
    ));

using SixtofourIpv6Params = TestWithParam<std::tuple<const char*, const char*, bool>>;
TEST_P(SixtofourIpv6Params, sixtofour) {
    const auto expected_ipv4 = std::get<1>(GetParam());
    const auto expected_has_ipv4 = std::get<2>(GetParam());

    const auto actual = ipv6_address::parse(std::get<0>(GetParam())).sixtofour();

    ASSERT_EQ(actual.has_value(), expected_has_ipv4);
    ASSERT_EQ(actual.value(), ipv4_address::parse(expected_ipv4));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, SixtofourIpv6Params,
    testing::Values(
        std::make_tuple("2002:ac1d:2d64::1", "172.29.45.100", true),
        std::make_tuple("2000:ac1d:2d64::1", "0.0.0.0", false)
    ));

using TeredoIpv6Params = TestWithParam<std::tuple<const char*, const char*, const char*, bool>>;
TEST_P(TeredoIpv6Params, teredo) {
    const auto expected_server = std::get<1>(GetParam());
    const auto expected_client = std::get<2>(GetParam());
    const auto expected_has_ipv4 = std::get<3>(GetParam());

    const auto actual = ipv6_address::parse(std::get<0>(GetParam())).teredo();

    ASSERT_EQ(actual.has_value(), expected_has_ipv4);
    ASSERT_EQ(actual.value().first, ipv4_address::parse(expected_server));
    ASSERT_EQ(actual.value().second, ipv4_address::parse(expected_client));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, TeredoIpv6Params,
    testing::Values(
        std::make_tuple("2001:0000:4136:e378:8000:63bf:3fff:fdd2", "65.54.227.120", "192.0.2.45", true),
        std::make_tuple("2000::4136:e378:8000:63bf:3fff:fdd2", "0.0.0.0", "0.0.0.0", false),
        std::make_tuple("2001:0001:4136:e378:8000:63bf:3fff:fdd2", "0.0.0.0", "0.0.0.0", false),
        std::make_tuple("2001:0:5ef5:79fd:0:59d:a0e5:ba1", "94.245.121.253", "95.26.244.94", true)
    ));
