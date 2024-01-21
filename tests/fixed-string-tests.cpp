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
