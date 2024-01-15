#include <gtest/gtest.h>

#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

#if IPADDRESS_CPP_VERSION >= 20
#  define TEST_FIXED_STRING          fixed_string_20
#  define TEST_FIXED_STRING_ITERATOR fixed_string_iterator_20
#elif IPADDRESS_CPP_VERSION >= 17
#  define TEST_FIXED_STRING          fixed_string_17
#  define TEST_FIXED_STRING_ITERATOR fixed_string_iterator_17
#elif IPADDRESS_CPP_VERSION >= 14
#  define TEST_FIXED_STRING          fixed_string_14
#  define TEST_FIXED_STRING_ITERATOR fixed_string_iterator_14
#elif IPADDRESS_CPP_VERSION >= 11
#  define TEST_FIXED_STRING          fixed_string_11
#  define TEST_FIXED_STRING_ITERATOR fixed_string_iterator_11
#endif

#if IPADDRESS_CPP_VERSION >= 17
#  define VAL_FIXED_STRING(name, str) fixed_string name = str
#else
#  define VAL_FIXED_STRING(name, str) auto name = make_fixed_string(str)
#endif

TEST(TEST_FIXED_STRING, ConstexprCtor)
{
    constexpr VAL_FIXED_STRING(str_11, "test string");
    constexpr auto str_11_length = str_11.length;
    constexpr auto str_11_size = str_11.size();
    constexpr auto str_11_empty = str_11.empty();
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

TEST(TEST_FIXED_STRING, ConstexprCtorEmpty)
{
    constexpr VAL_FIXED_STRING(str_0, "");
    constexpr auto str_0_length = str_0.length;
    constexpr auto str_0_size = str_0.size();
    constexpr auto str_0_empty = str_0.empty();
    EXPECT_EQ(str_0_length, 0);
    EXPECT_EQ(str_0_size, 0);
    EXPECT_EQ(str_0_empty, true);
}

TEST(TEST_FIXED_STRING, ConstexprDefaultCtor)
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

TEST(TEST_FIXED_STRING, ConstexprCopyCtor)
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

TEST(TEST_FIXED_STRING, ConstexprIterators)
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

TEST(TEST_FIXED_STRING, ConstexprEmptyIterators)
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

TEST(TEST_FIXED_STRING, ConstexprAt)
{
    constexpr VAL_FIXED_STRING(str, "test");
    constexpr auto at0 = str.at(0);
    constexpr auto at1 = str.at(1);
    constexpr auto at2 = str.at(2);
    constexpr auto at3 = str.at(3);
    EXPECT_THROW(str.at(4), std::out_of_range);
    
    EXPECT_EQ(at0, 't');
    EXPECT_EQ(at1, 'e');
    EXPECT_EQ(at2, 's');
    EXPECT_EQ(at3, 't');
    
    constexpr auto c0 = str[0];
    constexpr auto c1 = str[1];
    constexpr auto c2 = str[2];
    constexpr auto c3 = str[3];
    EXPECT_THROW(str[4], std::out_of_range);
    
    EXPECT_EQ(c0, 't');
    EXPECT_EQ(c1, 'e');
    EXPECT_EQ(c2, 's');
    EXPECT_EQ(c3, 't');
}

TEST(TEST_FIXED_STRING, ConstexprFrontBack)
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

TEST(TEST_FIXED_STRING_ITERATOR, ConstexprCategory)
{
#if IPADDRESS_CPP_VERSION >= 20
    EXPECT_TRUE(std::random_access_iterator<fixed_string_iterator>);
#else
    using category = std::iterator_traits<fixed_string_iterator>::iterator_category;
    
    constexpr auto actual = std::is_same<std::random_access_iterator_tag, category>::value;

    EXPECT_TRUE(actual);
#endif
}

TEST(TEST_FIXED_STRING_ITERATOR, ConstexprArithmetic)
{
    constexpr fixed_string_iterator iter("string", 6);
    constexpr auto c0 = *iter;
    EXPECT_EQ(c0, 's');

    constexpr auto at = iter[3];
    EXPECT_EQ(at, 'i');
    
    constexpr auto iter_pre_increment = ++fixed_string_iterator("string", 6);
    constexpr auto pre_increment = *iter_pre_increment;
    EXPECT_EQ(pre_increment, 't');
    
    constexpr auto iter_post_increment = fixed_string_iterator("string", 6)++;
    EXPECT_EQ(*iter_post_increment, 's');
    
    constexpr auto iter_pre_decrement = --(fixed_string_iterator("string", 6) + 5);
    constexpr auto pre_decrement = *iter_pre_decrement;
    EXPECT_EQ(pre_decrement, 'n');
    
    constexpr auto iter_post_decrement = (fixed_string_iterator("string", 6) + 5)--;
    EXPECT_EQ(*iter_post_decrement, 'g');

    constexpr auto iter_assign_plus = fixed_string_iterator("string", 6) += 2;
    EXPECT_EQ(*iter_assign_plus, 'r');
    
    constexpr auto iter_assign_minus = (fixed_string_iterator("string", 6) + 3) -= 1;
    EXPECT_EQ(*iter_assign_minus, 'r');

    constexpr auto iter_addition = iter + 4;
    EXPECT_EQ(*iter_addition, 'n');

    constexpr auto iter_subtraction = iter_pre_increment - 1;
    EXPECT_EQ(*iter_subtraction, 's');

    constexpr auto iter_outside_addition = 3 + iter;
    EXPECT_EQ(*iter_outside_addition, 'i');

    auto diff = iter_assign_plus - iter_pre_increment;
    EXPECT_EQ(diff, 1);

#if IPADDRESS_CPP_VERSION >= 20
    auto addr_diff = std::to_address(iter_assign_plus) - std::to_address(iter);
    EXPECT_EQ(addr_diff, 2);
#endif
}

TEST(TEST_FIXED_STRING_ITERATOR, ConstexprCompare) 
{
    constexpr fixed_string_iterator iter1("string", 6);

    constexpr fixed_string_iterator iter2 = iter1;
    constexpr auto iter1_2_eq = iter1 == iter2;
    constexpr auto iter1_2_not_eq = iter1 != iter2;
    constexpr auto iter1_2_less = iter1 < iter2;
    constexpr auto iter1_2_less_or_eq = iter1 <= iter2;
    constexpr auto iter1_2_great_or_eq = iter1 >= iter2;
    constexpr auto iter1_2_great = iter1 > iter2;
    
    EXPECT_TRUE(iter1_2_eq);
    EXPECT_FALSE(iter1_2_not_eq);
    EXPECT_FALSE(iter1_2_less);
    EXPECT_TRUE(iter1_2_less_or_eq);
    EXPECT_TRUE(iter1_2_great_or_eq);
    EXPECT_FALSE(iter1_2_great);
    
    constexpr fixed_string_iterator iter3 = iter1 + 3;
    constexpr auto iter1_3_eq = iter1 == iter3;
    constexpr auto iter1_3_not_eq = iter1 != iter3;
    constexpr auto iter1_3_less = iter1 < iter3;
    constexpr auto iter1_3_less_or_eq = iter1 <= iter3;
    constexpr auto iter1_3_great_or_eq = iter1 >= iter3;
    constexpr auto iter1_3_great = iter1 > iter3;
    
    EXPECT_FALSE(iter1_3_eq);
    EXPECT_TRUE(iter1_3_not_eq);
    EXPECT_TRUE(iter1_3_less);
    EXPECT_TRUE(iter1_3_less_or_eq);
    EXPECT_FALSE(iter1_3_great_or_eq);
    EXPECT_FALSE(iter1_3_great);
}
