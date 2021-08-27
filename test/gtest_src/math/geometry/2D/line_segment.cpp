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
	EXPECT_EQ(iseg.length_squared(), 1.0);
	EXPECT_EQ(iseg.length(), 1.0);
	
	EXPECT_EQ(jseg.start(), zero);
	EXPECT_EQ(jseg.end(), j);
	EXPECT_EQ(jseg.displacement(), j);
	EXPECT_EQ(jseg.length_squared(), 1.0);
	EXPECT_EQ(jseg.length(), 1.0);
	
	EXPECT_EQ(vseg.start(), i);
	EXPECT_EQ(vseg.end(), one);
	EXPECT_EQ(vseg.displacement(), j);
	EXPECT_EQ(vseg.length_squared(), 1.0);
	EXPECT_EQ(vseg.length(), 1.0);
	
	EXPECT_EQ(hseg.start(), j);
	EXPECT_EQ(hseg.end(), one);
	EXPECT_EQ(hseg.displacement(), i);
	EXPECT_EQ(hseg.length_squared(), 1.0);
	EXPECT_EQ(hseg.length(), 1.0);
	
	EXPECT_EQ(diag.start(), zero);
	EXPECT_EQ(diag.end(), one);
	EXPECT_EQ(diag.displacement(), i+j);
	EXPECT_EQ(diag.length_squared(), 2.0);
	
	float ls = diag.length() * diag.length();
	float expected = 2.0;
	float err = 1e-6;
	EXPECT_TRUE((ls >= expected - err)  and  (ls <= expected + err));
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
	EXPECT_EQ(inter1.hasHit(), true);
	EXPECT_EQ(inter2.hasHit(), true);
	
	// Intersection position.
	EXPECT_EQ(inter1.position().x(), 0.5);
	EXPECT_EQ(inter1.position().y(), 0.5);
	EXPECT_EQ(inter2.position().x(), 0.5);
	EXPECT_EQ(inter2.position().y(), 0.5);
	
	// Intersection t-parameters.
	EXPECT_EQ(inter1.thisLine(), 0.5);
	EXPECT_EQ(inter1.otherLine(), 0.5);
	EXPECT_EQ(inter2.thisLine(), 0.5);
	EXPECT_EQ(inter2.otherLine(), 0.5);
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
	EXPECT_EQ(inter.hasHit(), false);
}
