#include <gtest/gtest.h>

#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

#if IPADDRESS_CPP_VERSION >= 17
#  define VAL_FIXED_STRING(name, str) fixed_string name = str
#else
#  define VAL_FIXED_STRING(name, str) auto name = make_fixed_string(str)
#endif

TEST(fixed_string, ConstexprCtor)
{
    constexpr VAL_FIXED_STRING(str_11, "test string\0\0\0");
    constexpr auto str_11_max_length = str_11.max_length;
    constexpr auto str_11_length = str_11.length;
    constexpr auto str_11_size = str_11.size();
    constexpr auto str_11_empty = str_11.empty();
    EXPECT_EQ(str_11_max_length, 14);
    EXPECT_EQ(str_11_length, 11);
    EXPECT_EQ(str_11_size, 11);
    EXPECT_EQ(str_11_empty, false);
    
    constexpr VAL_FIXED_STRING(str_1, "a");
    constexpr auto str_1_length = str_1.length;
    constexpr auto str_1_size = str_1.size();
    constexpr auto str_1_empty = str_1.empty();
    EXPECT_EQ(str_1_length, 1);
    EXPECT_EQ(str_1_size, 1);
    EXPECT_EQ(str_1_empty, false);
}

TEST(fixed_string, ConstexprCtorEmpty)
{
    constexpr VAL_FIXED_STRING(str_0, "");
    constexpr auto str_0_length = str_0.length;
    constexpr auto str_0_size = str_0.size();
    constexpr auto str_0_empty = str_0.empty();
    EXPECT_EQ(str_0_length, 0);
    EXPECT_EQ(str_0_size, 0);
    EXPECT_EQ(str_0_empty, true);
}

TEST(fixed_string, ConstexprDefaultCtor)
{
#if IPADDRESS_CPP_VERSION >= 17
    constexpr fixed_string str_default;
#else
    constexpr fixed_string<0> str_default;
#endif
    constexpr auto str_default_length = str_default.length;
    constexpr auto str_default_size = str_default.size();
    constexpr auto str_default_empty = str_default.empty();
    EXPECT_EQ(str_default_length, 0);
    EXPECT_EQ(str_default_size, 0);
    EXPECT_EQ(str_default_empty, true);
}

TEST(fixed_string, ConstexprCopyCtor)
{
    constexpr VAL_FIXED_STRING(str, "test string");
#if IPADDRESS_CPP_VERSION >= 17
    constexpr fixed_string copy_str = str;
#else
    constexpr auto copy_str = str;
#endif

    EXPECT_EQ(copy_str.length, 11);
    EXPECT_EQ(copy_str.size(), 11);
}

template <typename Iter>
constexpr bool test_iterator(Iter begin, Iter end) {
    if (begin == end || *begin++ != 't') return false;
    if (begin == end || *begin++ != 'e') return false;
    if (begin == end || *begin++ != 's') return false;
    if (begin == end || *begin++ != 't') return false;
    if (begin == end || *begin++ != ' ') return false;
    if (begin == end || *begin++ != 'i') return false;
    if (begin == end || *begin++ != 't') return false;
    if (begin == end || *begin++ != 'e') return false;
    if (begin == end || *begin++ != 'r') return false;
    if (begin == end || *begin++ != 'a') return false;
    if (begin == end || *begin++ != 't') return false;
    if (begin == end || *begin++ != 'o') return false;
    if (begin == end || *begin++ != 'r') return false;
    if (begin != end) return false;
    return true;
}

template <typename Iter>
constexpr bool test_reverse_iterator(Iter begin, Iter end) {
    if (begin == end || *begin++ != 'r') return false;
    if (begin == end || *begin++ != 'o') return false;
    if (begin == end || *begin++ != 't') return false;
    if (begin == end || *begin++ != 'a') return false;
    if (begin == end || *begin++ != 'r') return false;
    if (begin == end || *begin++ != 'e') return false;
    if (begin == end || *begin++ != 't') return false;
    if (begin == end || *begin++ != 'i') return false;
    if (begin == end || *begin++ != ' ') return false;
    if (begin == end || *begin++ != 't') return false;
    if (begin == end || *begin++ != 's') return false;
    if (begin == end || *begin++ != 'e') return false;
    if (begin == end || *begin++ != 't') return false;
    if (begin != end) return false;
    return true;
}

TEST(fixed_string, ConstexprIterators)
{
    constexpr VAL_FIXED_STRING(str, "test iterator");

    constexpr auto actual = test_iterator(str.begin(), str.end());
    EXPECT_TRUE(actual);
    
    constexpr auto actual_const = test_iterator(str.cbegin(), str.cend());
    EXPECT_TRUE(actual_const);
    
    IPADDRESS_CONSTEXPR_17 auto actual_reverse = test_reverse_iterator(str.rbegin(), str.rend());
    EXPECT_TRUE(actual_reverse);
    
    IPADDRESS_CONSTEXPR_17 auto actual_const_reverse = test_reverse_iterator(str.crbegin(), str.crend());
    EXPECT_TRUE(actual_const_reverse);
    
    constexpr auto actual_ptr = test_iterator(str.data(), str.data() + str.size());
    EXPECT_TRUE(actual_ptr);
}

TEST(fixed_string, ConstexprEmptyIterators)
{
    constexpr VAL_FIXED_STRING(str, "");

    constexpr auto actual = str.begin() == str.end();
    EXPECT_TRUE(actual);
    
    constexpr auto actual_const = str.cbegin() == str.cend();
    EXPECT_TRUE(actual_const);
    
    IPADDRESS_CONSTEXPR_17 auto actual_reverse = str.rbegin() == str.rend();
    EXPECT_TRUE(actual_reverse);
    
    IPADDRESS_CONSTEXPR_17 auto actual_const_reverse = str.crbegin() == str.crend();
    EXPECT_TRUE(actual_const_reverse);
    
    constexpr auto actual_ptr = str.data() == str.data() + str.size();
    EXPECT_TRUE(actual_ptr);
}

TEST(fixed_string, ConstexprAt)
{
    constexpr VAL_FIXED_STRING(str, "test");
    constexpr auto at0 = str.at(0);
    constexpr auto at1 = str.at(1);
    constexpr auto at2 = str.at(2);
    constexpr auto at3 = str.at(3);
#ifndef IPADDRESS_NO_EXCEPTIONS
    EXPECT_THROW(str.at(4), std::out_of_range);
#endif

    EXPECT_EQ(at0, 't');
    EXPECT_EQ(at1, 'e');
    EXPECT_EQ(at2, 's');
    EXPECT_EQ(at3, 't');
    
    constexpr auto c0 = str[0];
    constexpr auto c1 = str[1];
    constexpr auto c2 = str[2];
    constexpr auto c3 = str[3];
#ifndef IPADDRESS_NO_EXCEPTIONS
    EXPECT_THROW(str[4], std::out_of_range);
#endif
    
    EXPECT_EQ(c0, 't');
    EXPECT_EQ(c1, 'e');
    EXPECT_EQ(c2, 's');
    EXPECT_EQ(c3, 't');
}

TEST(fixed_string, ConstexprFrontBack)
{
    constexpr VAL_FIXED_STRING(str_3, "str");
    constexpr auto front_3 = str_3.front();
    constexpr auto back_3 = str_3.back();
    EXPECT_EQ(front_3, 's');
    EXPECT_EQ(back_3, 'r');
    
    constexpr VAL_FIXED_STRING(str_1, "s");
    constexpr auto front_1 = str_1.front();
    constexpr auto back_1 = str_1.back();
    EXPECT_EQ(front_1, 's');
    EXPECT_EQ(back_1, 's');
}

TEST(fixed_string, ConstexprCompare)
{
    constexpr VAL_FIXED_STRING(str_1, "12345\0\0\0");
    constexpr VAL_FIXED_STRING(str_2, "12346");
    constexpr VAL_FIXED_STRING(str_3, "");
    constexpr VAL_FIXED_STRING(str_4, "\0\0\0");

    constexpr auto b_1 = str_1 == str_2;
    constexpr auto b_2 = str_1 != str_2;
    constexpr auto b_3 = str_1 < str_2;
    constexpr auto b_4 = str_1 > str_2;
    constexpr auto b_5 = str_1 <= str_2;
    constexpr auto b_6 = str_1 >= str_2;
    ASSERT_FALSE(b_1);
    ASSERT_TRUE(b_2);
    ASSERT_TRUE(b_3);
    ASSERT_FALSE(b_4);
    ASSERT_TRUE(b_5);
    ASSERT_FALSE(b_6);
    
    constexpr auto b_7 = str_2 == str_1;
    constexpr auto b_8 = str_2 != str_1;
    constexpr auto b_9 = str_2 < str_1;
    constexpr auto b_10 = str_2 > str_1;
    constexpr auto b_11 = str_2 <= str_1;
    constexpr auto b_12 = str_2 >= str_1;
    ASSERT_FALSE(b_7);
    ASSERT_TRUE(b_8);
    ASSERT_FALSE(b_9);
    ASSERT_TRUE(b_10);
    ASSERT_FALSE(b_11);
    ASSERT_TRUE(b_12);
    
    constexpr auto b_13 = str_2 == str_3;
    constexpr auto b_14 = str_2 != str_3;
    constexpr auto b_15 = str_2 < str_3;
    constexpr auto b_16 = str_2 > str_3;
    constexpr auto b_17 = str_2 <= str_3;
    constexpr auto b_18 = str_2 >= str_3;
    ASSERT_FALSE(b_13);
    ASSERT_TRUE(b_14);
    ASSERT_FALSE(b_15);
    ASSERT_TRUE(b_16);
    ASSERT_FALSE(b_17);
    ASSERT_TRUE(b_18);
    
    constexpr auto b_19 = str_3 == str_2;
    constexpr auto b_20 = str_3 != str_2;
    constexpr auto b_21 = str_3 < str_2;
    constexpr auto b_22 = str_3 > str_2;
    constexpr auto b_23 = str_3 <= str_2;
    constexpr auto b_24 = str_3 >= str_2;
    ASSERT_FALSE(b_19);
    ASSERT_TRUE(b_20);
    ASSERT_TRUE(b_21);
    ASSERT_FALSE(b_22);
    ASSERT_TRUE(b_23);
    ASSERT_FALSE(b_24);
    
    constexpr auto b_25 = str_3 == str_3;
    constexpr auto b_26 = str_3 != str_3;
    constexpr auto b_27 = str_3 < str_3;
    constexpr auto b_28 = str_3 > str_3;
    constexpr auto b_29 = str_3 <= str_3;
    constexpr auto b_30 = str_3 >= str_3;
    ASSERT_TRUE(b_25);
    ASSERT_FALSE(b_26);
    ASSERT_FALSE(b_27);
    ASSERT_FALSE(b_28);
    ASSERT_TRUE(b_29);
    ASSERT_TRUE(b_30);
    
    constexpr auto b_31 = str_2 == str_2;
    constexpr auto b_32 = str_2 != str_2;
    constexpr auto b_33 = str_2 < str_2;
    constexpr auto b_34 = str_2 > str_2;
    constexpr auto b_35 = str_2 <= str_2;
    constexpr auto b_36 = str_2 >= str_2;
    ASSERT_TRUE(b_31);
    ASSERT_FALSE(b_32);
    ASSERT_FALSE(b_33);
    ASSERT_FALSE(b_34);
    ASSERT_TRUE(b_35);
    ASSERT_TRUE(b_36);

    constexpr auto b_37 = str_3 == str_4;
    constexpr auto b_38 = str_3 != str_4;
    constexpr auto b_39 = str_3 < str_4;
    constexpr auto b_40 = str_3 > str_4;
    constexpr auto b_41 = str_3 <= str_4;
    constexpr auto b_42 = str_3 >= str_4;
    ASSERT_TRUE(b_37);
    ASSERT_FALSE(b_38);
    ASSERT_FALSE(b_39);
    ASSERT_FALSE(b_40);
    ASSERT_TRUE(b_41);
    ASSERT_TRUE(b_42);

    constexpr auto b_43 = str_4 == str_3;
    constexpr auto b_44 = str_4 != str_3;
    constexpr auto b_45 = str_4 < str_3;
    constexpr auto b_46 = str_4 > str_3;
    constexpr auto b_47 = str_4 <= str_3;
    constexpr auto b_48 = str_4 >= str_3;
    ASSERT_TRUE(b_43);
    ASSERT_FALSE(b_44);
    ASSERT_FALSE(b_45);
    ASSERT_FALSE(b_46);
    ASSERT_TRUE(b_47);
    ASSERT_TRUE(b_48);

    constexpr auto b_49 = str_4 == str_4;
    constexpr auto b_50 = str_4 != str_4;
    constexpr auto b_51 = str_4 < str_4;
    constexpr auto b_52 = str_4 > str_4;
    constexpr auto b_53 = str_4 <= str_4;
    constexpr auto b_54 = str_4 >= str_4;
    ASSERT_TRUE(b_49);
    ASSERT_FALSE(b_50);
    ASSERT_FALSE(b_51);
    ASSERT_FALSE(b_52);
    ASSERT_TRUE(b_53);
    ASSERT_TRUE(b_54);
}
