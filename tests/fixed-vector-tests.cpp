#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#ifdef IPADDRESS_TEST_MODULE
import ipaddress;
#else
#include <ipaddress/ipaddress.hpp>
#endif

using namespace testing;
using namespace ipaddress;

TEST(fixed_vector, ConstexprDefaultCtor) {
    IPADDRESS_CONSTEXPR fixed_vector<int, 0> vec0;
    IPADDRESS_CONSTEXPR auto vec0_is_empty = vec0.empty();
    IPADDRESS_CONSTEXPR auto vec0_size = vec0.size();
    IPADDRESS_CONSTEXPR auto vec0_max_size = vec0.max_size();
    IPADDRESS_CONSTEXPR auto vec0_capacity = vec0.capacity();
    IPADDRESS_CONSTEXPR auto vec0_data = vec0.data();
    ASSERT_TRUE(vec0_is_empty);
    ASSERT_EQ(vec0_size, 0);
    ASSERT_EQ(vec0_max_size, 0);
    ASSERT_EQ(vec0_capacity, 0);
    ASSERT_EQ(vec0_data, nullptr);

    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec5;
    IPADDRESS_CONSTEXPR auto vec5_is_empty = vec5.empty();
    IPADDRESS_CONSTEXPR auto vec5_size = vec5.size();
    IPADDRESS_CONSTEXPR auto vec5_max_size = vec5.max_size();
    IPADDRESS_CONSTEXPR auto vec5_capacity = vec5.capacity();
    ASSERT_TRUE(vec5_is_empty);
    ASSERT_EQ(vec5_size, 0);
    ASSERT_EQ(vec5_max_size, 5);
    ASSERT_EQ(vec5_capacity, 5);
}

TEST(fixed_vector, ConstexprCtorN) {
    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec5(size_t(3));
    IPADDRESS_CONSTEXPR auto vec5_is_empty = vec5.empty();
    IPADDRESS_CONSTEXPR auto vec5_size = vec5.size();
    IPADDRESS_CONSTEXPR auto vec5_max_size = vec5.max_size();
    IPADDRESS_CONSTEXPR auto vec5_capacity = vec5.capacity();
    IPADDRESS_CONSTEXPR auto vec5_0 = vec5[0];
    IPADDRESS_CONSTEXPR auto vec5_1 = vec5[1];
    IPADDRESS_CONSTEXPR auto vec5_2 = vec5[2];
    ASSERT_FALSE(vec5_is_empty);
    ASSERT_EQ(vec5_size, 3);
    ASSERT_EQ(vec5_max_size, 5);
    ASSERT_EQ(vec5_capacity, 5);
    ASSERT_EQ(vec5_0, 0);
    ASSERT_EQ(vec5_1, 0);
    ASSERT_EQ(vec5_2, 0);
}

TEST(fixed_vector, ConstexprCtorNValue) {
    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec5(size_t(3), 2);
    IPADDRESS_CONSTEXPR auto vec5_is_empty = vec5.empty();
    IPADDRESS_CONSTEXPR auto vec5_size = vec5.size();
    IPADDRESS_CONSTEXPR auto vec5_max_size = vec5.max_size();
    IPADDRESS_CONSTEXPR auto vec5_capacity = vec5.capacity();
    IPADDRESS_CONSTEXPR auto vec5_0 = vec5[0];
    IPADDRESS_CONSTEXPR auto vec5_1 = vec5[1];
    IPADDRESS_CONSTEXPR auto vec5_2 = vec5[2];
    ASSERT_FALSE(vec5_is_empty);
    ASSERT_EQ(vec5_size, 3);
    ASSERT_EQ(vec5_max_size, 5);
    ASSERT_EQ(vec5_capacity, 5);
    ASSERT_EQ(vec5_0, 2);
    ASSERT_EQ(vec5_1, 2);
    ASSERT_EQ(vec5_2, 2);
}

TEST(fixed_vector, ConstexprCtorIterators) {
    IPADDRESS_CONSTEXPR int inits[] {1, 3, 5};

    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec5(inits, inits + sizeof(inits) / sizeof(inits[0]));
    IPADDRESS_CONSTEXPR auto vec5_is_empty = vec5.empty();
    IPADDRESS_CONSTEXPR auto vec5_size = vec5.size();
    IPADDRESS_CONSTEXPR auto vec5_max_size = vec5.max_size();
    IPADDRESS_CONSTEXPR auto vec5_capacity = vec5.capacity();
    IPADDRESS_CONSTEXPR auto vec5_0 = vec5[0];
    IPADDRESS_CONSTEXPR auto vec5_1 = vec5[1];
    IPADDRESS_CONSTEXPR auto vec5_2 = vec5[2];
    ASSERT_FALSE(vec5_is_empty);
    ASSERT_EQ(vec5_size, 3);
    ASSERT_EQ(vec5_max_size, 5);
    ASSERT_EQ(vec5_capacity, 5);
    ASSERT_EQ(vec5_0, 1);
    ASSERT_EQ(vec5_1, 3);
    ASSERT_EQ(vec5_2, 5);
}

TEST(fixed_vector, ConstexprCtorInitializerList) {
    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec5 {1, 3, 5};
    IPADDRESS_CONSTEXPR auto vec5_is_empty = vec5.empty();
    IPADDRESS_CONSTEXPR auto vec5_size = vec5.size();
    IPADDRESS_CONSTEXPR auto vec5_max_size = vec5.max_size();
    IPADDRESS_CONSTEXPR auto vec5_capacity = vec5.capacity();
    IPADDRESS_CONSTEXPR auto vec5_0 = vec5[0];
    IPADDRESS_CONSTEXPR auto vec5_1 = vec5[1];
    IPADDRESS_CONSTEXPR auto vec5_2 = vec5[2];
    ASSERT_FALSE(vec5_is_empty);
    ASSERT_EQ(vec5_size, 3);
    ASSERT_EQ(vec5_max_size, 5);
    ASSERT_EQ(vec5_capacity, 5);
    ASSERT_EQ(vec5_0, 1);
    ASSERT_EQ(vec5_1, 3);
    ASSERT_EQ(vec5_2, 5);
}

IPADDRESS_CONSTEXPR fixed_vector<int, 5> test_assign_n_value(size_t n, int value) {
    fixed_vector<int, 5> vec {2};
    vec.assign(n, value);
    return vec;
}

template <size_t N>
IPADDRESS_CONSTEXPR fixed_vector<int, 5> test_assign_iterators(const int(&values)[N]) {
    fixed_vector<int, 5> vec {2};
    vec.assign(values, values + N);
    return vec;
}

IPADDRESS_CONSTEXPR fixed_vector<int, 5> test_assign_initializer_list(std::initializer_list<int> init_list) {
    fixed_vector<int, 5> vec {2};
    vec.assign(init_list);
    return vec;
}

TEST(fixed_vector, assign) {
    IPADDRESS_CONSTEXPR auto vec0 = test_assign_n_value(3, 1);    
    IPADDRESS_CONSTEXPR auto vec0_size = vec0.size();
    IPADDRESS_CONSTEXPR auto vec0_max_size = vec0.max_size();
    IPADDRESS_CONSTEXPR auto vec0_capacity = vec0.capacity();
    IPADDRESS_CONSTEXPR auto vec0_0 = vec0[0];
    IPADDRESS_CONSTEXPR auto vec0_1 = vec0[1];
    IPADDRESS_CONSTEXPR auto vec0_2 = vec0[2];
    ASSERT_EQ(vec0_size, 3);
    ASSERT_EQ(vec0_max_size, 5);
    ASSERT_EQ(vec0_capacity, 5);
    ASSERT_EQ(vec0_0, 1);
    ASSERT_EQ(vec0_1, 1);
    ASSERT_EQ(vec0_2, 1);
    
    IPADDRESS_CONSTEXPR auto vec1 = test_assign_iterators({ 5, 7, 3});
    IPADDRESS_CONSTEXPR auto vec1_size = vec1.size();
    IPADDRESS_CONSTEXPR auto vec1_max_size = vec1.max_size();
    IPADDRESS_CONSTEXPR auto vec1_capacity = vec1.capacity();
    IPADDRESS_CONSTEXPR auto vec1_0 = vec1[0];
    IPADDRESS_CONSTEXPR auto vec1_1 = vec1[1];
    IPADDRESS_CONSTEXPR auto vec1_2 = vec1[2];
    ASSERT_EQ(vec1_size, 3);
    ASSERT_EQ(vec1_max_size, 5);
    ASSERT_EQ(vec1_capacity, 5);
    ASSERT_EQ(vec1_0, 5);
    ASSERT_EQ(vec1_1, 7);
    ASSERT_EQ(vec1_2, 3);
    
    IPADDRESS_CONSTEXPR auto vec2 = test_assign_initializer_list({ 4, 3});
    IPADDRESS_CONSTEXPR auto vec2_size = vec2.size();
    IPADDRESS_CONSTEXPR auto vec2_max_size = vec2.max_size();
    IPADDRESS_CONSTEXPR auto vec2_capacity = vec2.capacity();
    IPADDRESS_CONSTEXPR auto vec2_0 = vec2[0];
    IPADDRESS_CONSTEXPR auto vec2_1 = vec2[1];
    ASSERT_EQ(vec2_size, 2);
    ASSERT_EQ(vec2_max_size, 5);
    ASSERT_EQ(vec2_capacity, 5);
    ASSERT_EQ(vec2_0, 4);
    ASSERT_EQ(vec2_1, 3);
}

TEST(fixed_vector, at) {
    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec {1, 3, 5};
    IPADDRESS_CONSTEXPR auto vec_0 = vec.at(0);
    IPADDRESS_CONSTEXPR auto vec_1 = vec.at(1);
    IPADDRESS_CONSTEXPR auto vec_2 = vec.at(2);
    ASSERT_EQ(vec_0, 1);
    ASSERT_EQ(vec_1, 3);
    ASSERT_EQ(vec_2, 5);
}

TEST(fixed_vector, front) {
    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec {1, 3, 5};
    IPADDRESS_CONSTEXPR auto front = vec.front();
    ASSERT_EQ(front, 1);
}

TEST(fixed_vector, back) {
    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec {1, 3, 5};
    IPADDRESS_CONSTEXPR auto back = vec.back();
    ASSERT_EQ(back, 5);
}

TEST(fixed_vector, data) {
    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec {1, 3, 5};
    IPADDRESS_CONSTEXPR auto data_0 = vec.data()[0];
    IPADDRESS_CONSTEXPR auto data_1 = vec.data()[1];
    IPADDRESS_CONSTEXPR auto data_2 = vec.data()[2];
    ASSERT_EQ(data_0, 1);
    ASSERT_EQ(data_1, 3);
    ASSERT_EQ(data_2, 5);
}

TEST(fixed_vector, empty) {
    IPADDRESS_CONSTEXPR fixed_vector<int, 0> vec0;
    IPADDRESS_CONSTEXPR auto vec0_is_empty = vec0.empty();
    ASSERT_TRUE(vec0_is_empty);

    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec1;
    IPADDRESS_CONSTEXPR auto vec1_is_empty = vec1.empty();
    ASSERT_TRUE(vec1_is_empty);
    
    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec2{ 1, 2, 3 };
    IPADDRESS_CONSTEXPR auto vec2_is_empty = vec2.empty();
    ASSERT_FALSE(vec2_is_empty);
}

TEST(fixed_vector, size) {
    IPADDRESS_CONSTEXPR fixed_vector<int, 0> vec0;
    IPADDRESS_CONSTEXPR auto vec0_size = vec0.size();
    ASSERT_EQ(vec0_size, 0);

    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec1;
    IPADDRESS_CONSTEXPR auto vec1_size = vec1.size();
    ASSERT_EQ(vec1_size, 0);
    
    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec2{ 1, 2, 3 };
    IPADDRESS_CONSTEXPR auto vec2_size = vec2.size();
    ASSERT_EQ(vec2_size, 3);
}

TEST(fixed_vector, max_size) {
    IPADDRESS_CONSTEXPR fixed_vector<int, 0> vec0;
    IPADDRESS_CONSTEXPR auto vec0_max_size = vec0.max_size();
    ASSERT_EQ(vec0_max_size, 0);

    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec1;
    IPADDRESS_CONSTEXPR auto vec1_max_size = vec1.max_size();
    ASSERT_EQ(vec1_max_size, 5);
    
    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec2{ 1, 2, 3 };
    IPADDRESS_CONSTEXPR auto vec2_max_size = vec2.max_size();
    ASSERT_EQ(vec2_max_size, 5);
}

TEST(fixed_vector, capacity) {
    IPADDRESS_CONSTEXPR fixed_vector<int, 0> vec0;
    IPADDRESS_CONSTEXPR auto vec0_capacity = vec0.capacity();
    ASSERT_EQ(vec0_capacity, 0);

    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec1;
    IPADDRESS_CONSTEXPR auto vec1_capacity = vec1.capacity();
    ASSERT_EQ(vec1_capacity, 5);
    
    IPADDRESS_CONSTEXPR fixed_vector<int, 5> vec2{ 1, 2, 3 };
    IPADDRESS_CONSTEXPR auto vec2_capacity = vec2.capacity();
    ASSERT_EQ(vec2_capacity, 5);
}

IPADDRESS_CONSTEXPR fixed_vector<int, 5> test_resize_n(size_t n) {
    fixed_vector<int, 5> vec {2};
    vec.resize(n);
    return vec;
}

IPADDRESS_CONSTEXPR fixed_vector<int, 5> test_resize_n_value(size_t n, int value) {
    fixed_vector<int, 5> vec {2};
    vec.resize(n, value);
    return vec;
}

IPADDRESS_CONSTEXPR fixed_vector<int, 5> test_resize_truncate(size_t n) {
    fixed_vector<int, 5> vec { 1, 2, 3, 4, 5 };
    vec.resize(n);
    return vec;
}

TEST(fixed_vector, resize) {
    IPADDRESS_CONSTEXPR auto vec0 = test_resize_n(3);
    IPADDRESS_CONSTEXPR auto vec0_size = vec0.size();
    IPADDRESS_CONSTEXPR auto vec0_max_size = vec0.max_size();
    IPADDRESS_CONSTEXPR auto vec0_capacity = vec0.capacity();
    IPADDRESS_CONSTEXPR auto vec0_0 = vec0[0];
    IPADDRESS_CONSTEXPR auto vec0_1 = vec0[1];
    IPADDRESS_CONSTEXPR auto vec0_2 = vec0[2];
    ASSERT_EQ(vec0_size, 3);
    ASSERT_EQ(vec0_max_size, 5);
    ASSERT_EQ(vec0_capacity, 5);
    ASSERT_EQ(vec0_0, 2);
    ASSERT_EQ(vec0_1, 0);
    ASSERT_EQ(vec0_2, 0);
    
    IPADDRESS_CONSTEXPR auto vec1 = test_resize_n_value(3, 4);
    IPADDRESS_CONSTEXPR auto vec1_size = vec1.size();
    IPADDRESS_CONSTEXPR auto vec1_max_size = vec1.max_size();
    IPADDRESS_CONSTEXPR auto vec1_capacity = vec1.capacity();
    IPADDRESS_CONSTEXPR auto vec1_0 = vec1[0];
    IPADDRESS_CONSTEXPR auto vec1_1 = vec1[1];
    IPADDRESS_CONSTEXPR auto vec1_2 = vec1[2];
    ASSERT_EQ(vec1_size, 3);
    ASSERT_EQ(vec1_max_size, 5);
    ASSERT_EQ(vec1_capacity, 5);
    ASSERT_EQ(vec1_0, 2);
    ASSERT_EQ(vec1_1, 4);
    ASSERT_EQ(vec1_2, 4);

    IPADDRESS_CONSTEXPR auto vec2 = test_resize_truncate(3);
    IPADDRESS_CONSTEXPR auto vec2_size = vec2.size();
    IPADDRESS_CONSTEXPR auto vec2_max_size = vec2.max_size();
    IPADDRESS_CONSTEXPR auto vec2_capacity = vec2.capacity();
    IPADDRESS_CONSTEXPR auto vec2_0 = vec2[0];
    IPADDRESS_CONSTEXPR auto vec2_1 = vec2[1];
    IPADDRESS_CONSTEXPR auto vec2_2 = vec2[2];
    ASSERT_EQ(vec2_size, 3);
    ASSERT_EQ(vec2_max_size, 5);
    ASSERT_EQ(vec2_capacity, 5);
    ASSERT_EQ(vec2_0, 1);
    ASSERT_EQ(vec2_1, 2);
    ASSERT_EQ(vec2_2, 3);
}

IPADDRESS_CONSTEXPR fixed_vector<int, 5> test_reserve(size_t n) {
    fixed_vector<int, 5> vec {2};
    vec.reserve(n);
    return vec;
}

TEST(fixed_vector, reserve) {
    IPADDRESS_CONSTEXPR auto vec = test_reserve(3);
    IPADDRESS_CONSTEXPR auto vec_size = vec.size();
    IPADDRESS_CONSTEXPR auto vec_max_size = vec.max_size();
    IPADDRESS_CONSTEXPR auto vec_capacity = vec.capacity();
    IPADDRESS_CONSTEXPR auto vec_0 = vec[0];
    ASSERT_EQ(vec_size, 1);
    ASSERT_EQ(vec_max_size, 5);
    ASSERT_EQ(vec_capacity, 5);
    ASSERT_EQ(vec_0, 2);
}

IPADDRESS_CONSTEXPR fixed_vector<int, 5> test_shrink_to_fit() {
    fixed_vector<int, 5> vec {2};
    vec.shrink_to_fit();
    return vec;
}

TEST(fixed_vector, shrink_to_fit) {
    IPADDRESS_CONSTEXPR auto vec = test_shrink_to_fit();
    IPADDRESS_CONSTEXPR auto vec_size = vec.size();
    IPADDRESS_CONSTEXPR auto vec_max_size = vec.max_size();
    IPADDRESS_CONSTEXPR auto vec_capacity = vec.capacity();
    IPADDRESS_CONSTEXPR auto vec_0 = vec[0];
    ASSERT_EQ(vec_size, 1);
    ASSERT_EQ(vec_max_size, 5);
    ASSERT_EQ(vec_capacity, 5);
    ASSERT_EQ(vec_0, 2);
}

IPADDRESS_CONSTEXPR std::tuple<fixed_vector<int, 5>, int, int> test_insert_value(const int& value) {
    fixed_vector<int, 5> vec {1, 2};
    const auto insert1 = *vec.insert(vec.begin() + 1, value);
    const auto insert2 = *vec.insert(vec.end(), 7);
    return { vec, insert1, insert2 };
}

IPADDRESS_CONSTEXPR std::tuple<fixed_vector<int, 6>, int, int> test_insert_n_value(const int& value) {
    fixed_vector<int, 6> vec {1, 2};
    const auto insert1 = *vec.insert(vec.begin() + 1, size_t(2), value);
    const auto insert2 = *vec.insert(vec.end(), size_t(2), 7);
    return { vec, insert1, insert2 };
}

IPADDRESS_CONSTEXPR std::tuple<fixed_vector<int, 6>, int, int, int, int> test_insert_initializer_list() {
    fixed_vector<int, 6> vec {1, 2};
    const auto it1 = vec.insert(vec.begin() + 1, {4, 5});
    const auto it2 = vec.insert(vec.end(), {7, 8});
    return { vec, *it1, *(it1 + 1), *it2, *(it2 + 1) };
}

IPADDRESS_CONSTEXPR std::pair<fixed_vector<int, 5>, bool> test_insert_none() {
    constexpr int values[] { 3, 4 };
    fixed_vector<int, 5> vec { 1, 2 };
    auto pos_m = vec.begin() + 1;
    auto pos_end = vec.end();
    auto insert0 = vec.insert(pos_m, size_t(0), 7);
    auto insert1 = vec.insert(pos_end, size_t(0), 7);
    auto insert2 = vec.insert(pos_m, values, values);
    auto insert3 = vec.insert(pos_end, values, values);
    auto insert4 = vec.insert(pos_m, {});
    auto insert5 = vec.insert(pos_end, {});
    return { vec,
        insert0 != pos_m || insert1 != pos_end ||
        insert2 != pos_m || insert3 != pos_end || 
        insert4 != pos_m || insert5 != pos_end };
}

TEST(fixed_vector, insert) {
    IPADDRESS_CONSTEXPR auto result0 = test_insert_value(4);
    IPADDRESS_CONSTEXPR auto vec0 = std::get<0>(result0);
    IPADDRESS_CONSTEXPR auto insert0_1 = std::get<1>(result0);
    IPADDRESS_CONSTEXPR auto insert0_2 = std::get<2>(result0);
    IPADDRESS_CONSTEXPR auto vec0_size = vec0.size();
    IPADDRESS_CONSTEXPR auto vec0_max_size = vec0.max_size();
    IPADDRESS_CONSTEXPR auto vec0_capacity = vec0.capacity();
    IPADDRESS_CONSTEXPR auto vec0_0 = vec0[0];
    IPADDRESS_CONSTEXPR auto vec0_1 = vec0[1];
    IPADDRESS_CONSTEXPR auto vec0_2 = vec0[2];
    IPADDRESS_CONSTEXPR auto vec0_3 = vec0[3];
    ASSERT_EQ(vec0_size, 4);
    ASSERT_EQ(vec0_max_size, 5);
    ASSERT_EQ(vec0_capacity, 5);
    ASSERT_EQ(vec0_0, 1);
    ASSERT_EQ(vec0_1, 4);
    ASSERT_EQ(vec0_2, 2);
    ASSERT_EQ(vec0_3, 7);
    ASSERT_EQ(insert0_1, 4);
    ASSERT_EQ(insert0_2, 7);

    IPADDRESS_CONSTEXPR auto result1 = test_insert_n_value(4);
    IPADDRESS_CONSTEXPR auto vec1 = std::get<0>(result1);
    IPADDRESS_CONSTEXPR auto insert1_1 = std::get<1>(result1);
    IPADDRESS_CONSTEXPR auto insert1_2 = std::get<2>(result1);
    IPADDRESS_CONSTEXPR auto vec1_size = vec1.size();
    IPADDRESS_CONSTEXPR auto vec1_max_size = vec1.max_size();
    IPADDRESS_CONSTEXPR auto vec1_capacity = vec1.capacity();
    IPADDRESS_CONSTEXPR auto vec1_0 = vec1[0];
    IPADDRESS_CONSTEXPR auto vec1_1 = vec1[1];
    IPADDRESS_CONSTEXPR auto vec1_2 = vec1[2];
    IPADDRESS_CONSTEXPR auto vec1_3 = vec1[3];
    IPADDRESS_CONSTEXPR auto vec1_4 = vec1[4];
    IPADDRESS_CONSTEXPR auto vec1_5 = vec1[5];
    ASSERT_EQ(vec1_size, 6);
    ASSERT_EQ(vec1_max_size, 6);
    ASSERT_EQ(vec1_capacity, 6);
    ASSERT_EQ(vec1_0, 1);
    ASSERT_EQ(vec1_1, 4);
    ASSERT_EQ(vec1_2, 4);
    ASSERT_EQ(vec1_3, 2);
    ASSERT_EQ(vec1_4, 7);
    ASSERT_EQ(vec1_5, 7);
    ASSERT_EQ(insert1_1, 4);
    ASSERT_EQ(insert1_2, 7);

    IPADDRESS_CONSTEXPR auto result2 = test_insert_initializer_list();
    IPADDRESS_CONSTEXPR auto vec2 = std::get<0>(result2);
    IPADDRESS_CONSTEXPR auto insert2_1 = std::get<1>(result2);
    IPADDRESS_CONSTEXPR auto insert2_2 = std::get<2>(result2);
    IPADDRESS_CONSTEXPR auto insert2_3 = std::get<3>(result2);
    IPADDRESS_CONSTEXPR auto insert2_4 = std::get<4>(result2);
    IPADDRESS_CONSTEXPR auto vec2_size = vec2.size();
    IPADDRESS_CONSTEXPR auto vec2_max_size = vec2.max_size();
    IPADDRESS_CONSTEXPR auto vec2_capacity = vec2.capacity();
    IPADDRESS_CONSTEXPR auto vec2_0 = vec2[0];
    IPADDRESS_CONSTEXPR auto vec2_1 = vec2[1];
    IPADDRESS_CONSTEXPR auto vec2_2 = vec2[2];
    IPADDRESS_CONSTEXPR auto vec2_3 = vec2[3];
    IPADDRESS_CONSTEXPR auto vec2_4 = vec2[4];
    IPADDRESS_CONSTEXPR auto vec2_5 = vec2[5];
    ASSERT_EQ(vec2_size, 6);
    ASSERT_EQ(vec2_max_size, 6);
    ASSERT_EQ(vec2_capacity, 6);
    ASSERT_EQ(vec2_0, 1);
    ASSERT_EQ(vec2_1, 4);
    ASSERT_EQ(vec2_2, 5);
    ASSERT_EQ(vec2_3, 2);
    ASSERT_EQ(vec2_4, 7);
    ASSERT_EQ(vec2_5, 8);
    ASSERT_EQ(insert2_1, 4);
    ASSERT_EQ(insert2_2, 5);
    ASSERT_EQ(insert2_3, 7);
    ASSERT_EQ(insert2_4, 8);

    IPADDRESS_CONSTEXPR auto result3 = test_insert_none();
    IPADDRESS_CONSTEXPR auto vec3 = std::get<0>(result3);
    IPADDRESS_CONSTEXPR auto insert3 = std::get<1>(result3);
    IPADDRESS_CONSTEXPR auto vec3_size = vec3.size();
    IPADDRESS_CONSTEXPR auto vec3_max_size = vec3.max_size();
    IPADDRESS_CONSTEXPR auto vec3_capacity = vec3.capacity();
    IPADDRESS_CONSTEXPR auto vec3_0 = vec3[0];
    IPADDRESS_CONSTEXPR auto vec3_1 = vec3[1];
    ASSERT_EQ(vec3_size, 2);
    ASSERT_EQ(vec3_max_size, 5);
    ASSERT_EQ(vec3_capacity, 5);
    ASSERT_EQ(vec3_0, 1);
    ASSERT_EQ(vec3_1, 2);
    ASSERT_FALSE(insert3);
}

IPADDRESS_CONSTEXPR std::tuple<fixed_vector<int, 5>, int, int> test_emplace() {
    fixed_vector<int, 5> vec { 1, 2 };
    const auto insert1 = *vec.emplace(vec.begin() + 1, 5);
    const auto insert2 = *vec.emplace(vec.end(), 7);
    return { vec, insert1, insert2 };
}

TEST(fixed_vector, emplace) {
    IPADDRESS_CONSTEXPR auto result = test_emplace();
    IPADDRESS_CONSTEXPR auto vec = std::get<0>(result);
    IPADDRESS_CONSTEXPR auto insert1 = std::get<1>(result);
    IPADDRESS_CONSTEXPR auto insert2 = std::get<2>(result);
    IPADDRESS_CONSTEXPR auto vec_size = vec.size();
    IPADDRESS_CONSTEXPR auto vec_max_size = vec.max_size();
    IPADDRESS_CONSTEXPR auto vec_capacity = vec.capacity();
    IPADDRESS_CONSTEXPR auto vec_0 = vec[0];
    IPADDRESS_CONSTEXPR auto vec_1 = vec[1];
    IPADDRESS_CONSTEXPR auto vec_2 = vec[2];
    IPADDRESS_CONSTEXPR auto vec_3 = vec[3];
    ASSERT_EQ(vec_size, 4);
    ASSERT_EQ(vec_max_size, 5);
    ASSERT_EQ(vec_capacity, 5);
    ASSERT_EQ(vec_0, 1);
    ASSERT_EQ(vec_1, 5);
    ASSERT_EQ(vec_2, 2);
    ASSERT_EQ(vec_3, 7);
    ASSERT_EQ(insert1, 5);
    ASSERT_EQ(insert2, 7);
}

IPADDRESS_CONSTEXPR std::pair<fixed_vector<int, 5>, int> test_emplace_back() {
    fixed_vector<int, 5> vec { 1, 2 };
    const auto& insert = vec.emplace_back(7);
    return { vec, insert };
}

TEST(fixed_vector, emplace_back) {
    IPADDRESS_CONSTEXPR auto result = test_emplace_back();
    IPADDRESS_CONSTEXPR auto vec = std::get<0>(result);
    IPADDRESS_CONSTEXPR auto insert = std::get<1>(result);
    IPADDRESS_CONSTEXPR auto vec_size = vec.size();
    IPADDRESS_CONSTEXPR auto vec_max_size = vec.max_size();
    IPADDRESS_CONSTEXPR auto vec_capacity = vec.capacity();
    IPADDRESS_CONSTEXPR auto vec_0 = vec[0];
    IPADDRESS_CONSTEXPR auto vec_1 = vec[1];
    IPADDRESS_CONSTEXPR auto vec_2 = vec[2];
    ASSERT_EQ(vec_size, 3);
    ASSERT_EQ(vec_max_size, 5);
    ASSERT_EQ(vec_capacity, 5);
    ASSERT_EQ(vec_0, 1);
    ASSERT_EQ(vec_1, 2);
    ASSERT_EQ(vec_2, 7);
    ASSERT_EQ(insert, 7);
}

IPADDRESS_CONSTEXPR std::tuple<fixed_vector<int, 3>, int, int*> test_try_emplace_back() {
    fixed_vector<int, 3> vec { 1, 2 };
    const auto insert1 = *vec.try_emplace_back(7);
    const auto insert2 = vec.try_emplace_back(5);
    return { vec, insert1, insert2 };
}

TEST(fixed_vector, try_emplace_back) {
    IPADDRESS_CONSTEXPR auto result = test_try_emplace_back();
    IPADDRESS_CONSTEXPR auto vec = std::get<0>(result);
    IPADDRESS_CONSTEXPR auto insert1 = std::get<1>(result);
    IPADDRESS_CONSTEXPR auto insert2 = std::get<2>(result);
    IPADDRESS_CONSTEXPR auto vec_size = vec.size();
    IPADDRESS_CONSTEXPR auto vec_max_size = vec.max_size();
    IPADDRESS_CONSTEXPR auto vec_capacity = vec.capacity();
    IPADDRESS_CONSTEXPR auto vec_0 = vec[0];
    IPADDRESS_CONSTEXPR auto vec_1 = vec[1];
    IPADDRESS_CONSTEXPR auto vec_2 = vec[2];
    ASSERT_EQ(vec_size, 3);
    ASSERT_EQ(vec_max_size, 3);
    ASSERT_EQ(vec_capacity, 3);
    ASSERT_EQ(vec_0, 1);
    ASSERT_EQ(vec_1, 2);
    ASSERT_EQ(vec_2, 7);
    ASSERT_EQ(insert1, 7);
    ASSERT_EQ(insert2, nullptr);
}

IPADDRESS_CONSTEXPR std::pair<fixed_vector<int, 3>, int> test_unchecked_emplace_back() {
    fixed_vector<int, 3> vec { 1, 2 };
    const auto& insert = vec.unchecked_emplace_back(7);
    return { vec, insert };
}

TEST(fixed_vector, unchecked_emplace_back) {
    IPADDRESS_CONSTEXPR auto result = test_unchecked_emplace_back();
    IPADDRESS_CONSTEXPR auto vec = std::get<0>(result);
    IPADDRESS_CONSTEXPR auto insert = std::get<1>(result);
    IPADDRESS_CONSTEXPR auto vec_size = vec.size();
    IPADDRESS_CONSTEXPR auto vec_max_size = vec.max_size();
    IPADDRESS_CONSTEXPR auto vec_capacity = vec.capacity();
    IPADDRESS_CONSTEXPR auto vec_0 = vec[0];
    IPADDRESS_CONSTEXPR auto vec_1 = vec[1];
    IPADDRESS_CONSTEXPR auto vec_2 = vec[2];
    ASSERT_EQ(vec_size, 3);
    ASSERT_EQ(vec_max_size, 3);
    ASSERT_EQ(vec_capacity, 3);
    ASSERT_EQ(vec_0, 1);
    ASSERT_EQ(vec_1, 2);
    ASSERT_EQ(vec_2, 7);
    ASSERT_EQ(insert, 7);
}

IPADDRESS_CONSTEXPR std::tuple<fixed_vector<int, 5>, int, int> test_push_back(const int& value) {
    fixed_vector<int, 5> vec { 1, 2 };
    const auto& insert1 = vec.push_back(7);
    const auto& insert2 = vec.push_back(value);
    return { vec, insert1, insert2 };
}

TEST(fixed_vector, push_back) {
    IPADDRESS_CONSTEXPR auto result = test_push_back(5);
    IPADDRESS_CONSTEXPR auto vec = std::get<0>(result);
    IPADDRESS_CONSTEXPR auto insert1 = std::get<1>(result);
    IPADDRESS_CONSTEXPR auto insert2 = std::get<2>(result);
    IPADDRESS_CONSTEXPR auto vec_size = vec.size();
    IPADDRESS_CONSTEXPR auto vec_max_size = vec.max_size();
    IPADDRESS_CONSTEXPR auto vec_capacity = vec.capacity();
    IPADDRESS_CONSTEXPR auto vec_0 = vec[0];
    IPADDRESS_CONSTEXPR auto vec_1 = vec[1];
    IPADDRESS_CONSTEXPR auto vec_2 = vec[2];
    IPADDRESS_CONSTEXPR auto vec_3 = vec[3];
    ASSERT_EQ(vec_size, 4);
    ASSERT_EQ(vec_max_size, 5);
    ASSERT_EQ(vec_capacity, 5);
    ASSERT_EQ(vec_0, 1);
    ASSERT_EQ(vec_1, 2);
    ASSERT_EQ(vec_2, 7);
    ASSERT_EQ(vec_3, 5);
    ASSERT_EQ(insert1, 7);
    ASSERT_EQ(insert2, 5);
}

IPADDRESS_CONSTEXPR std::tuple<fixed_vector<int, 4>, int, int, int*, int*> test_try_push_back(const int& value) {
    fixed_vector<int, 4> vec { 1, 2 };
    const auto insert1 = *vec.try_push_back(7);
    const auto insert2 = *vec.try_push_back(value);
    const auto insert3 = vec.try_push_back(3);
    const auto insert4 = vec.try_push_back(value);
    return { vec, insert1, insert2, insert3, insert4 };
}

TEST(fixed_vector, try_push_back) {
    IPADDRESS_CONSTEXPR auto result = test_try_push_back(5);
    IPADDRESS_CONSTEXPR auto vec = std::get<0>(result);
    IPADDRESS_CONSTEXPR auto insert1 = std::get<1>(result);
    IPADDRESS_CONSTEXPR auto insert2 = std::get<2>(result);
    IPADDRESS_CONSTEXPR auto insert3 = std::get<3>(result);
    IPADDRESS_CONSTEXPR auto insert4 = std::get<4>(result);
    IPADDRESS_CONSTEXPR auto vec_size = vec.size();
    IPADDRESS_CONSTEXPR auto vec_max_size = vec.max_size();
    IPADDRESS_CONSTEXPR auto vec_capacity = vec.capacity();
    IPADDRESS_CONSTEXPR auto vec_0 = vec[0];
    IPADDRESS_CONSTEXPR auto vec_1 = vec[1];
    IPADDRESS_CONSTEXPR auto vec_2 = vec[2];
    IPADDRESS_CONSTEXPR auto vec_3 = vec[3];
    ASSERT_EQ(vec_size, 4);
    ASSERT_EQ(vec_max_size, 4);
    ASSERT_EQ(vec_capacity, 4);
    ASSERT_EQ(vec_0, 1);
    ASSERT_EQ(vec_1, 2);
    ASSERT_EQ(vec_2, 7);
    ASSERT_EQ(vec_3, 5);
    ASSERT_EQ(insert1, 7);
    ASSERT_EQ(insert2, 5);
    ASSERT_EQ(insert3, nullptr);
    ASSERT_EQ(insert4, nullptr);
}

IPADDRESS_CONSTEXPR std::tuple<fixed_vector<int, 4>, int, int> test_unchecked_push_back(const int& value) {
    fixed_vector<int, 4> vec { 1, 2 };
    const auto& insert1 = vec.unchecked_push_back(7);
    const auto& insert2 = vec.unchecked_push_back(value);
    return { vec, insert1, insert2 };
}

TEST(fixed_vector, unchecked_push_back) {
    IPADDRESS_CONSTEXPR auto result = test_unchecked_push_back(5);
    IPADDRESS_CONSTEXPR auto vec = std::get<0>(result);
    IPADDRESS_CONSTEXPR auto insert1 = std::get<1>(result);
    IPADDRESS_CONSTEXPR auto insert2 = std::get<2>(result);
    IPADDRESS_CONSTEXPR auto vec_size = vec.size();
    IPADDRESS_CONSTEXPR auto vec_max_size = vec.max_size();
    IPADDRESS_CONSTEXPR auto vec_capacity = vec.capacity();
    IPADDRESS_CONSTEXPR auto vec_0 = vec[0];
    IPADDRESS_CONSTEXPR auto vec_1 = vec[1];
    IPADDRESS_CONSTEXPR auto vec_2 = vec[2];
    IPADDRESS_CONSTEXPR auto vec_3 = vec[3];
    ASSERT_EQ(vec_size, 4);
    ASSERT_EQ(vec_max_size, 4);
    ASSERT_EQ(vec_capacity, 4);
    ASSERT_EQ(vec_0, 1);
    ASSERT_EQ(vec_1, 2);
    ASSERT_EQ(vec_2, 7);
    ASSERT_EQ(vec_3, 5);
    ASSERT_EQ(insert1, 7);
    ASSERT_EQ(insert2, 5);
}

IPADDRESS_CONSTEXPR fixed_vector<int, 5> test_pop_back() {
    fixed_vector<int, 5> vec { 1, 2, 3 };
    vec.pop_back();
    return vec;
}

TEST(fixed_vector, pop_back) {
    IPADDRESS_CONSTEXPR auto vec = test_pop_back();
    IPADDRESS_CONSTEXPR auto vec_size = vec.size();
    IPADDRESS_CONSTEXPR auto vec_max_size = vec.max_size();
    IPADDRESS_CONSTEXPR auto vec_capacity = vec.capacity();
    IPADDRESS_CONSTEXPR auto vec_0 = vec[0];
    IPADDRESS_CONSTEXPR auto vec_1 = vec[1];
    ASSERT_EQ(vec_size, 2);
    ASSERT_EQ(vec_max_size, 5);
    ASSERT_EQ(vec_capacity, 5);
    ASSERT_EQ(vec_0, 1);
    ASSERT_EQ(vec_1, 2);
}

IPADDRESS_CONSTEXPR fixed_vector<int, 5> test_clear() {
    fixed_vector<int, 5> vec { 1, 2, 3 };
    vec.clear();
    return vec;
}

TEST(fixed_vector, clear) {
    IPADDRESS_CONSTEXPR auto vec = test_clear();
    IPADDRESS_CONSTEXPR auto vec_size = vec.size();
    IPADDRESS_CONSTEXPR auto vec_max_size = vec.max_size();
    IPADDRESS_CONSTEXPR auto vec_capacity = vec.capacity();
    ASSERT_EQ(vec_size, 0);
    ASSERT_EQ(vec_max_size, 5);
    ASSERT_EQ(vec_capacity, 5);
}

IPADDRESS_CONSTEXPR std::pair<fixed_vector<int, 5>, int> test_erase() {
    fixed_vector<int, 5> vec { 1, 2, 3 };
    auto it = vec.erase(vec.begin() + 1);
    return { vec, *it };
}

IPADDRESS_CONSTEXPR std::pair<fixed_vector<int, 5>, int> test_erase_none() {
    fixed_vector<int, 5> vec { 1, 2, 3 };
    auto it = vec.erase(vec.begin() + 1, vec.begin() + 1);
    return { vec, *it };
}

IPADDRESS_CONSTEXPR std::pair<fixed_vector<int, 5>, int> test_erase_begin() {
    fixed_vector<int, 5> vec { 1, 2, 3 };
    auto it = vec.erase(vec.begin());
    return { vec, *it };
}

IPADDRESS_CONSTEXPR std::pair<fixed_vector<int, 5>, bool> test_erase_last() {
    fixed_vector<int, 5> vec { 1, 2, 3 };
    auto it = vec.erase(vec.end() - 1);
    return { vec, !(it == vec.end()) };
}

IPADDRESS_CONSTEXPR std::pair<fixed_vector<int, 5>, bool> test_erase_all() {
    fixed_vector<int, 5> vec { 1, 2, 3 };
    auto it = vec.erase(vec.begin(), vec.end());
    return { vec, !(it == vec.end()) };
}

IPADDRESS_CONSTEXPR std::pair<fixed_vector<int, 5>, int> test_erase_range() {
    fixed_vector<int, 5> vec { 1, 2, 3 };
    auto it = vec.erase(vec.begin(), vec.end() - 1);
    return { vec, *it };
}

TEST(fixed_vector, erase) {
    IPADDRESS_CONSTEXPR auto vec_result = test_erase();
    IPADDRESS_CONSTEXPR auto vec = std::get<0>(vec_result);
    IPADDRESS_CONSTEXPR auto vec_next = std::get<1>(vec_result);
    IPADDRESS_CONSTEXPR auto vec_size = vec.size();
    IPADDRESS_CONSTEXPR auto vec_max_size = vec.max_size();
    IPADDRESS_CONSTEXPR auto vec_capacity = vec.capacity();
    IPADDRESS_CONSTEXPR auto vec_0 = vec[0];
    IPADDRESS_CONSTEXPR auto vec_1 = vec[1];
    ASSERT_EQ(vec_next, 3);
    ASSERT_EQ(vec_size, 2);
    ASSERT_EQ(vec_max_size, 5);
    ASSERT_EQ(vec_capacity, 5);
    ASSERT_EQ(vec_0, 1);
    ASSERT_EQ(vec_1, 3);

    IPADDRESS_CONSTEXPR auto vec_none_result = test_erase_none();
    IPADDRESS_CONSTEXPR auto vec_none = std::get<0>(vec_none_result);
    IPADDRESS_CONSTEXPR auto vec_none_next = std::get<1>(vec_none_result);
    IPADDRESS_CONSTEXPR auto vec_none_size = vec_none.size();
    IPADDRESS_CONSTEXPR auto vec_none_max_size = vec_none.max_size();
    IPADDRESS_CONSTEXPR auto vec_none_capacity = vec_none.capacity();
    IPADDRESS_CONSTEXPR auto vec_none_0 = vec_none[0];
    IPADDRESS_CONSTEXPR auto vec_none_1 = vec_none[1];
    IPADDRESS_CONSTEXPR auto vec_none_2 = vec_none[2];
    ASSERT_EQ(vec_none_next, 2);
    ASSERT_EQ(vec_none_size, 3);
    ASSERT_EQ(vec_none_max_size, 5);
    ASSERT_EQ(vec_none_capacity, 5);
    ASSERT_EQ(vec_none_0, 1);
    ASSERT_EQ(vec_none_1, 2);
    ASSERT_EQ(vec_none_2, 3);

    IPADDRESS_CONSTEXPR auto vec_begin_result = test_erase_begin();
    IPADDRESS_CONSTEXPR auto vec_begin = std::get<0>(vec_begin_result);
    IPADDRESS_CONSTEXPR auto vec_begin_next = std::get<1>(vec_begin_result);
    IPADDRESS_CONSTEXPR auto vec_begin_size = vec_begin.size();
    IPADDRESS_CONSTEXPR auto vec_begin_max_size = vec_begin.max_size();
    IPADDRESS_CONSTEXPR auto vec_begin_capacity = vec_begin.capacity();
    IPADDRESS_CONSTEXPR auto vec_begin_0 = vec_begin[0];
    IPADDRESS_CONSTEXPR auto vec_begin_1 = vec_begin[1];
    ASSERT_EQ(vec_begin_next, 2);
    ASSERT_EQ(vec_begin_size, 2);
    ASSERT_EQ(vec_begin_max_size, 5);
    ASSERT_EQ(vec_begin_capacity, 5);
    ASSERT_EQ(vec_begin_0, 2);
    ASSERT_EQ(vec_begin_1, 3);

    IPADDRESS_CONSTEXPR auto vec_last_result = test_erase_last();
    IPADDRESS_CONSTEXPR auto vec_last = std::get<0>(vec_last_result);
    IPADDRESS_CONSTEXPR auto vec_last_has_next = std::get<1>(vec_last_result);
    IPADDRESS_CONSTEXPR auto vec_last_size = vec_last.size();
    IPADDRESS_CONSTEXPR auto vec_last_max_size = vec_last.max_size();
    IPADDRESS_CONSTEXPR auto vec_last_capacity = vec_last.capacity();
    IPADDRESS_CONSTEXPR auto vec_last_0 = vec_last[0];
    IPADDRESS_CONSTEXPR auto vec_last_1 = vec_last[1];
    ASSERT_FALSE(vec_last_has_next);
    ASSERT_EQ(vec_last_size, 2);
    ASSERT_EQ(vec_last_max_size, 5);
    ASSERT_EQ(vec_last_capacity, 5);
    ASSERT_EQ(vec_last_0, 1);
    ASSERT_EQ(vec_last_1, 2);

    IPADDRESS_CONSTEXPR auto vec_all_result = test_erase_all();
    IPADDRESS_CONSTEXPR auto vec_all = std::get<0>(vec_all_result);
    IPADDRESS_CONSTEXPR auto vec_all_has_next = std::get<1>(vec_all_result);
    IPADDRESS_CONSTEXPR auto vec_all_size = vec_all.size();
    IPADDRESS_CONSTEXPR auto vec_all_max_size = vec_all.max_size();
    IPADDRESS_CONSTEXPR auto vec_all_capacity = vec_all.capacity();
    ASSERT_FALSE(vec_all_has_next);
    ASSERT_EQ(vec_all_size, 0);
    ASSERT_EQ(vec_all_max_size, 5);
    ASSERT_EQ(vec_all_capacity, 5);

    IPADDRESS_CONSTEXPR auto vec_range_result = test_erase_range();
    IPADDRESS_CONSTEXPR auto vec_range = std::get<0>(vec_range_result);
    IPADDRESS_CONSTEXPR auto vec_range_next = std::get<1>(vec_range_result);
    IPADDRESS_CONSTEXPR auto vec_range_size = vec_range.size();
    IPADDRESS_CONSTEXPR auto vec_range_max_size = vec_range.max_size();
    IPADDRESS_CONSTEXPR auto vec_range_capacity = vec_range.capacity();
    IPADDRESS_CONSTEXPR auto vec_range_0 = vec_range[0];
    ASSERT_EQ(vec_range_next, 3);
    ASSERT_EQ(vec_range_size, 1);
    ASSERT_EQ(vec_range_max_size, 5);
    ASSERT_EQ(vec_range_capacity, 5);
    ASSERT_EQ(vec_range_0, 3);
}

IPADDRESS_CONSTEXPR std::pair<fixed_vector<int, 5>, fixed_vector<int, 5>> test_swap() {
    fixed_vector<int, 5> vec1 { 1, 2, 3 };
    fixed_vector<int, 5> vec2 { 4, 5 };
    vec1.swap(vec2);
    return { vec1, vec2 };
}

TEST(fixed_vector, swap) {
    IPADDRESS_CONSTEXPR auto result = test_swap();
    IPADDRESS_CONSTEXPR auto vec1 = std::get<0>(result);
    IPADDRESS_CONSTEXPR auto vec2 = std::get<1>(result);
    ASSERT_EQ(vec1.size(), 2);
    ASSERT_EQ(vec2.size(), 3);
    ASSERT_EQ(vec1[0], 4);
    ASSERT_EQ(vec1[1], 5);
    ASSERT_EQ(vec2[0], 1);
    ASSERT_EQ(vec2[1], 2);
    ASSERT_EQ(vec2[2], 3);
}
