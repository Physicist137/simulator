#include <gtest/gtest.h>
#include <math/linear/static_vector.hpp>
#include <math/geometry/2D/line_segment.hpp>


TEST(LineSegment2D, ConstructorAndBasicFunctions) {
	// Define few vectors.
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	math::linear::StaticVector<float, 2> one = i + j;
	
	// Define few segments.
	math::geometry2::LineSegment<float> iseg(i);
	math::geometry2::LineSegment<float> jseg(j);
	math::geometry2::LineSegment<float> vseg(i, one);
	math::geometry2::LineSegment<float> hseg(j, one);
	math::geometry2::LineSegment<float> diag(zero, one);
	
	EXPECT_EQ(iseg.start(), zero);
	EXPECT_EQ(iseg.end(), i);
	EXPECT_EQ(iseg.displacement(), i);
	EXPECT_FLOAT_EQ(iseg.length_squared(), 1.0);
	EXPECT_FLOAT_EQ(iseg.length(), 1.0);
	
	EXPECT_EQ(jseg.start(), zero);
	EXPECT_EQ(jseg.end(), j);
	EXPECT_EQ(jseg.displacement(), j);
	EXPECT_FLOAT_EQ(jseg.length_squared(), 1.0);
	EXPECT_FLOAT_EQ(jseg.length(), 1.0);
	
	EXPECT_EQ(vseg.start(), i);
	EXPECT_EQ(vseg.end(), one);
	EXPECT_EQ(vseg.displacement(), j);
	EXPECT_FLOAT_EQ(vseg.length_squared(), 1.0);
	EXPECT_FLOAT_EQ(vseg.length(), 1.0);
	
	EXPECT_EQ(hseg.start(), j);
	EXPECT_EQ(hseg.end(), one);
	EXPECT_EQ(hseg.displacement(), i);
	EXPECT_FLOAT_EQ(hseg.length_squared(), 1.0);
	EXPECT_FLOAT_EQ(hseg.length(), 1.0);
	
	EXPECT_EQ(diag.start(), zero);
	EXPECT_EQ(diag.end(), one);
	EXPECT_EQ(diag.displacement(), i+j);
	EXPECT_FLOAT_EQ(diag.length_squared(), 2.0);
	EXPECT_FLOAT_EQ(diag.length() * diag.length(), 2.0);
}


TEST(LineSegment, CrossIntersection) {
	// Define few vectors.
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	math::linear::StaticVector<float, 2> one = i + j;
	
	// Define few segments.
	math::geometry2::LineSegment<float> up(zero, one);
	math::geometry2::LineSegment<float> down(j, i);
	
	// Get intersection data.
	auto inter1 = up.intersect(down);
	auto inter2 = down.intersect(up);
	
	// Were there intersection?
	EXPECT_TRUE(inter1.hasHit());
	EXPECT_TRUE(inter2.hasHit());
	
	// Intersection position.
	EXPECT_FLOAT_EQ(inter1.position().x(), 0.5);
	EXPECT_FLOAT_EQ(inter1.position().y(), 0.5);
	EXPECT_FLOAT_EQ(inter2.position().x(), 0.5);
	EXPECT_FLOAT_EQ(inter2.position().y(), 0.5);
	
	// Intersection t-parameters.
	EXPECT_FLOAT_EQ(inter1.thisParameter(), 0.5);
	EXPECT_FLOAT_EQ(inter1.otherParameter(), 0.5);
	EXPECT_FLOAT_EQ(inter2.thisParameter(), 0.5);
	EXPECT_FLOAT_EQ(inter2.otherParameter(), 0.5);
}


TEST(LineSegment, ParallelIntersection) {
	// Define few vectors.
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	math::linear::StaticVector<float, 2> one = i + j;
	
	// Define few segments.
	math::geometry2::LineSegment<float> down(zero, i);
	math::geometry2::LineSegment<float> up(j, i+j);
	
	// Get intersection data.
	auto inter = up.intersect(down);
	
	// Were there intersection?
	EXPECT_FALSE(inter.hasHit());
}
