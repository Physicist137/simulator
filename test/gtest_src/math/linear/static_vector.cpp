#include <gtest/gtest.h>
#include <math/linear/static_vector.hpp>

TEST(StaticVector, VectorConstructor) {
	math::linear::StaticVector<float, 3> vec3;
	math::linear::StaticVector<float, 4> vec4({1, 2, 3, 4, 5, 6});
	math::linear::StaticVector<float, 5> vec5 = {1, 2, 3, 4, 5};
	math::linear::StaticVector<float, 5> other = vec5;
	math::linear::StaticVector<float, 5> otherr(vec5);
	math::linear::StaticVector<float, 5> otherrr{vec5};
	math::linear::StaticVector<float, 5> otherrrr({vec5});
	
	EXPECT_EQ(vec3.x(), 0);
	EXPECT_EQ(vec3.y(), 0);
	EXPECT_EQ(vec3.z(), 0);
	
	EXPECT_EQ(vec4.x(), 1);
	EXPECT_EQ(vec4.y(), 2);
	EXPECT_EQ(vec4.z(), 3);
	EXPECT_EQ(vec4.w(), 4);
	
	for (unsigned i = 0; i < 5; ++i) {
		float result = static_cast<float>(i+1);
		EXPECT_EQ(vec5[i], result);
		EXPECT_EQ(other[i], result);
		EXPECT_EQ(otherr[i], result);
		EXPECT_EQ(otherrr[i], result);
		EXPECT_EQ(otherrrr[i], result);
	}
}

TEST(StaticVector, VectorOperations) {
	math::linear::StaticVector<float, 5> seq({1, 2, 3, 4, 5});
	math::linear::StaticVector<float, 5> two({2, 2, 2, 2, 2});
	math::linear::StaticVector<float, 5> dop({0, 1, 0, 0, 1});
	
	auto asd = 1.0f * seq + 0.5f * two;
	EXPECT_EQ(asd[0], 2.0);
	EXPECT_EQ(asd[1], 3.0);
	EXPECT_EQ(asd[2], 4.0);
	EXPECT_EQ(asd[3], 5.0);
	EXPECT_EQ(asd[4], 6.0);
	
	auto lol = seq * two * dop / 2.0f;
	EXPECT_EQ(lol[0], 0.0);
	EXPECT_EQ(lol[1], 2.0);
	EXPECT_EQ(lol[2], 0.0);
	EXPECT_EQ(lol[3], 0.0);
	EXPECT_EQ(lol[4], 5.0);
}