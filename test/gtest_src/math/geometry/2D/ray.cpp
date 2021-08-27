#include <gtest/gtest.h>
#include <math/linear/static_vector.hpp>
#include <math/geometry/2D/line_segment.hpp>
#include <math/geometry/2D/ray.hpp>
#include <cmath>


TEST(Ray2D, ConstructorAndBasicFunctions) {
	// Define few vectors.
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	math::linear::StaticVector<float, 2> one = i + j;
	
	// Define few segments.
	math::geometry2::Ray<float> horizontal(i);
	math::geometry2::Ray<float> vertical(zero, j);
	
	EXPECT_EQ(horizontal.start(), zero);
	EXPECT_EQ(horizontal.direction(), i);
	EXPECT_TRUE(horizontal.directed());

	EXPECT_EQ(vertical.start(), zero);
	EXPECT_EQ(vertical.direction(), j);
	EXPECT_TRUE(horizontal.directed());
}


TEST(Ray2D, CrossIntersection) {
	// Define few vectors.
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	math::linear::StaticVector<float, 2> one = i + j;
	
	// Define few segments.
	math::geometry2::Ray<float> horizontal(j-i, i);
	math::geometry2::Ray<float> vertical(i-j, j);
	math::geometry2::Ray<float> diag(-i-j);
	
	// Intersect.
	auto inter = horizontal.intersect(vertical);
	EXPECT_TRUE(inter.hasHit());
	EXPECT_FLOAT_EQ(inter.thisParameter(), 2.0);
	EXPECT_FLOAT_EQ(inter.otherParameter(), 2.0);
	
	inter = horizontal.intersect(diag);
	EXPECT_FALSE(inter.hasHit());
	
	inter = vertical.intersect(diag);
	EXPECT_FALSE(inter.hasHit());
	
	diag.setDirected(false);
	inter = horizontal.intersect(diag);
	EXPECT_TRUE(inter.hasHit());
	EXPECT_FLOAT_EQ(inter.thisParameter(), 2.0);
	EXPECT_FLOAT_EQ(inter.otherParameter(), -1.0);
	
	inter = vertical.intersect(diag);
	EXPECT_TRUE(inter.hasHit());
	EXPECT_FLOAT_EQ(inter.thisParameter(), 2.0);
	EXPECT_FLOAT_EQ(inter.otherParameter(), -1.0);
}