#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

struct StringUintParam : public TestWithParam<std::tuple<const char*, uint32_t>> {
    const char* GetString() const noexcept {
        return std::get<0>(GetParam());
    }

    uint32_t GetUint() const noexcept {
        return std::get<1>(GetParam());
    }
};

//constexpr bytes<5> test() {
//    bytes<5> a{};
//    a[1] = 2;
//    return a;
//}

TEST(ipv4_address, Test)
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

TEST(ipv4_address, EmptyAddress) {
    ipv4_address::base_type expected_empty { 0, 0, 0, 0};

    error_code err;
    auto ip = ipv4_address::parse("", err);

    EXPECT_EQ(err, error_code::EMPTY_ADDRESS);
    EXPECT_EQ(ip.bytes(), expected_empty);
    EXPECT_EQ(ip.to_uint32(), 0);
    EXPECT_EQ(uint32_t(ip), 0);

    EXPECT_THAT(
        []() { ipv4_address::parse(""); },
        ThrowsMessage<parse_error>(StrEq("address cannot be empty")));
}

struct LeadingZerosParams : public StringUintParam {};
TEST_P(LeadingZerosParams, LeadingZeros) {
    auto expected_address = GetString();
    auto expected_octet = GetUint();

    std::ostringstream ss;
    ss << "leading zeros are not permitted in octet " << expected_octet << " of address " << expected_address;

    EXPECT_THAT(
        [address=expected_address]() { ipv4_address::parse(address); },
        ThrowsMessage<parse_error>(StrEq(ss.str())));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, LeadingZerosParams,
    testing::Values(
            std::make_tuple("000.000.000.000", 0),
            std::make_tuple("192.168.000.001", 2),
            std::make_tuple("016.016.016.016", 0),
            std::make_tuple("001.000.008.016", 0),
            std::make_tuple("01.2.3.40", 0),
            std::make_tuple("1.02.3.40", 1),
            std::make_tuple("1.2.03.40", 2),
            std::make_tuple("1.2.3.040", 3)
    ));

struct FromUint32Params : public StringUintParam {};
TEST_P(FromUint32Params, FromUint32) {
    ASSERT_EQ(ipv4_address::from_uint32(GetUint()), ipv4_address::parse(GetString()));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, FromUint32Params,
    testing::Values(
            std::make_tuple("0.0.0.0", 0),
            std::make_tuple("192.168.0.1", 0xC0A80001)
    ));
