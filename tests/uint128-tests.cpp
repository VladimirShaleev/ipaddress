#include <gtest/gtest.h>

#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

TEST(uint128_t, Ctors) {
    constexpr uint128_t value1;
    constexpr uint128_t value2{};
    constexpr uint128_t value3 = 1;
    constexpr uint128_t value4(1, 2);
    constexpr uint128_t value5 { 1, 2 };
    constexpr uint128_t value6 = { 1, 2 };
    constexpr uint128_t value7 { 1 };
    constexpr uint128_t value8 = { 1 };

    ASSERT_EQ(value1.high, 0);
    ASSERT_EQ(value1.low, 0);
    ASSERT_EQ(value2.high, 0);
    ASSERT_EQ(value2.low, 0);
    ASSERT_EQ(value3.high, 0);
    ASSERT_EQ(value3.low, 1);
    ASSERT_EQ(value4.high, 1);
    ASSERT_EQ(value4.low, 2);
    ASSERT_EQ(value5.high, 1);
    ASSERT_EQ(value5.low, 2);
    ASSERT_EQ(value6.high, 1);
    ASSERT_EQ(value6.low, 2);
    ASSERT_EQ(value7.high, 0);
    ASSERT_EQ(value7.low, 1);
    ASSERT_EQ(value8.high, 0);
    ASSERT_EQ(value8.low, 1);
}

TEST(uint128_t, Copy) {
    constexpr uint128_t value = 5;

    constexpr uint128_t copy_ctor = value;
    uint128_t copy_operator;
    copy_operator = value;

    ASSERT_EQ(copy_ctor.high, 0);
    ASSERT_EQ(copy_ctor.low, 5);
    ASSERT_EQ(copy_operator.high, 0);
    ASSERT_EQ(copy_operator.low, 5);
}

TEST(uint128_t, Move) {
    constexpr uint128_t value = 5;

    constexpr uint128_t move_ctor = std::move(value);
    uint128_t move_operator;
    move_operator = std::move(move_ctor);

    ASSERT_EQ(move_operator.high, 0);
    ASSERT_EQ(move_operator.low, 5);
}

TEST(uint128_t, OperatorBool) {
    constexpr uint128_t value1 = 0;
    constexpr uint128_t value2 = 5;
    
    constexpr auto actual1 = (bool) value1;
    constexpr auto actual2 = (bool) value2;

    ASSERT_FALSE(actual1);
    ASSERT_TRUE(actual2);
}

TEST(uint128_t, OperatorT) {
    constexpr uint128_t value { 4, 140185576636287 };
    
    constexpr auto actual1 = (char) value;
    constexpr auto actual2 = (signed char) value;
    constexpr auto actual3 = (unsigned char) value;
    constexpr auto actual4 = (short) value;
    constexpr auto actual5 = (unsigned short) value;
    constexpr auto actual6 = (int) value;
    constexpr auto actual7 = (unsigned int) value;
    constexpr auto actual8 = (long) value;
    constexpr auto actual9 = (unsigned long) value;
    constexpr auto actual10 = (long long) value;
    constexpr auto actual11 = (unsigned long long) value;

    ASSERT_EQ(actual1, (char)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual2, (signed char)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual3, (unsigned char)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual4, (short)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual5, (unsigned short)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual6, (int)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual7, (unsigned int)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual8, (long)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual9, (unsigned long)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual10, (long long)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual11, (unsigned long long)(0x7F7F7F7F7F7F));
}

TEST(uint128_t, Arithmetic) {
    constexpr uint128_t value1 { 4, 5 };
    constexpr uint128_t value2 { 4, 0xFFFFFFFFFFFFFFFFULL - 1 };
    constexpr uint128_t value3 { 4, 0xFFFFFFFFFFFFFFFFULL };

    constexpr auto plus1 = +value1;
    constexpr auto plus2 = +value2;
    constexpr auto plus3 = +value3;
    ASSERT_EQ(plus1.high, 4);
    ASSERT_EQ(plus1.low, 5);
    ASSERT_EQ(plus2.high, 4);
    ASSERT_EQ(plus2.low, 0xFFFFFFFFFFFFFFFFULL - 1);
    ASSERT_EQ(plus3.high, 4);
    ASSERT_EQ(plus3.low, 0xFFFFFFFFFFFFFFFFULL);

    constexpr auto minus1 = -value1;
    constexpr auto minus2 = -value2;
    constexpr auto minus3 = -value3;
    ASSERT_EQ(minus1.high, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus1.low, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus2.high, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus2.low, 2);
    ASSERT_EQ(minus3.high, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus3.low, 1);
    
    constexpr auto inv1 = ~value1;
    constexpr auto inv2 = ~value2;
    constexpr auto inv3 = ~value3;
    ASSERT_EQ(inv1.high, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(inv1.low, 0xFFFFFFFFFFFFFFFFULL - 5);
    ASSERT_EQ(inv2.high, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(inv2.low, 1);
    ASSERT_EQ(inv3.high, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(inv3.low, 0);
    
    constexpr auto sum1 = value1 + 1;
    constexpr auto sum2 = value2 + 1;
    constexpr auto sum3 = value3 + 1;
    constexpr auto sum4 = value1 + uint128_t(10, 0xFFFFFFFFFFFFFFFFULL - 2);
    constexpr auto sum5 = 1 + value1;
    ASSERT_EQ(sum1.high, 4);
    ASSERT_EQ(sum1.low, 6);
    ASSERT_EQ(sum2.high, 4);
    ASSERT_EQ(sum2.low, 0xFFFFFFFFFFFFFFFFULL);
    ASSERT_EQ(sum3.high, 5);
    ASSERT_EQ(sum3.low, 0);
    ASSERT_EQ(sum4.high, 15);
    ASSERT_EQ(sum4.low, 2);
    ASSERT_EQ(sum5.high, 4);
    ASSERT_EQ(sum5.low, 6);
    
    constexpr auto sub1 = value1 - 6;
    constexpr auto sub2 = value2 - 7;
    constexpr auto sub3 = value3 - 1;
    constexpr auto sub4 = value1 - uint128_t(2, 0xFFFFFFFFFFFFFFFFULL - 2);
    constexpr auto sub5 = 10 - uint128_t(7);
    ASSERT_EQ(sub1.high, 3);
    ASSERT_EQ(sub1.low, 0xFFFFFFFFFFFFFFFFULL);
    ASSERT_EQ(sub2.high, 4);
    ASSERT_EQ(sub2.low, 0xFFFFFFFFFFFFFFFFULL - 8);
    ASSERT_EQ(sub3.high, 4);
    ASSERT_EQ(sub3.low, 0xFFFFFFFFFFFFFFFFULL - 1);
    ASSERT_EQ(sub4.high, 1);
    ASSERT_EQ(sub4.low, 8);
    ASSERT_EQ(sub5.high, 0);
    ASSERT_EQ(sub5.low, 3);
    
    constexpr auto mul1 = value1 * 6;
    constexpr auto mul2 = value2 * 7;
    constexpr auto mul3 = value3 * 1;
    constexpr auto mul4 = 1000 * uint128_t(2, 0xFFFFFFFFFFFFFFFFULL - 2);
    ASSERT_EQ(mul1.high, 24);
    ASSERT_EQ(mul1.low, 30);
    ASSERT_EQ(mul2.high, 34);
    ASSERT_EQ(mul2.low, 0XFFFFFFFFFFFFFFF2);
    ASSERT_EQ(mul3.high, 4);
    ASSERT_EQ(mul3.low, 0xFFFFFFFFFFFFFFFFULL);
    ASSERT_EQ(mul4.high, 0xBB7);
    ASSERT_EQ(mul4.low, 0XFFFFFFFFFFFFF448);
}
