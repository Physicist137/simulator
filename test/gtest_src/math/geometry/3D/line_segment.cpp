#include <gtest/gtest.h>
#include <math/linear/static_vector.hpp>
#include <math/geometry/3D/line_segment.hpp>


TEST(LineSegment3D, ConstructorAndBasicFunctions) {
	// Define few vectors.
	math::linear::StaticVector<float, 3> zero({0.0, 0.0, 0.0});
	math::linear::StaticVector<float, 3> i({1.0, 0.0, 0.0});
	math::linear::StaticVector<float, 3> j({0.0, 1.0, 0.0});
	math::linear::StaticVector<float, 3> k({0.0, 0.0, 0.1});
	math::linear::StaticVector<float, 3> one = i + j;
	
	// Define few segments.
	math::geometry3::LineSegment<float> iseg(i);
	math::geometry3::LineSegment<float> jseg(j);
	math::geometry3::LineSegment<float> vseg(i, one);
	math::geometry3::LineSegment<float> hseg(j, one);
	math::geometry3::LineSegment<float> diag(zero, one);
	
	EXPECT_EQ(iseg.start(), zero);
	EXPECT_EQ(iseg.end(), i);
	EXPECT_EQ(iseg.displacement(), i);
	EXPECT_FLOAT_EQ(iseg.length_squared(), 1.0);
	
	EXPECT_EQ(jseg.start(), zero);
	EXPECT_EQ(jseg.end(), j);
	EXPECT_EQ(jseg.displacement(), j);
	EXPECT_FLOAT_EQ(jseg.length_squared(), 1.0);
	
	EXPECT_EQ(vseg.start(), i);
	EXPECT_EQ(vseg.end(), one);
	EXPECT_EQ(vseg.displacement(), j);
	EXPECT_FLOAT_EQ(vseg.length_squared(), 1.0);
	
	EXPECT_EQ(hseg.start(), j);
	EXPECT_EQ(hseg.end(), one);
	EXPECT_EQ(hseg.displacement(), i);
	EXPECT_FLOAT_EQ(hseg.length_squared(), 1.0);
	
	EXPECT_EQ(diag.start(), zero);
	EXPECT_EQ(diag.end(), one);
	EXPECT_EQ(diag.displacement(), i+j);
	EXPECT_FLOAT_EQ(diag.length_squared(), 2.0);
}


TEST(LineSegment3D, CrossIntersection) {
	// Define few vectors.
	math::linear::StaticVector<float, 3> zero({0.0, 0.0, 0.0});
	math::linear::StaticVector<float, 3> i({1.0, 0.0, 0.0});
	math::linear::StaticVector<float, 3> j({0.0, 1.0, 0.0});
	math::linear::StaticVector<float, 3> k({0.0, 0.0, 0.1});
	math::linear::StaticVector<float, 3> one = i + j;
	
	// Define few segments.
	math::geometry3::LineSegment<float> up(zero, i+j);
	math::geometry3::LineSegment<float> down(j, i);
	math::geometry3::LineSegment<float> uu(zero, i+j+k);
	math::geometry3::LineSegment<float> dd(j+k, i);
	
	// Get intersection data.
	auto plane_inter = up.intersect(down);
	auto space_inter = uu.intersect(dd);
	
	// Were there intersection?
	EXPECT_TRUE(plane_inter.hasHit());
	EXPECT_TRUE(space_inter.hasHit());
	
	// Intersection position.
	EXPECT_FLOAT_EQ(plane_inter.position().x(), 0.5);
	EXPECT_FLOAT_EQ(plane_inter.position().y(), 0.5);
	EXPECT_FLOAT_EQ(space_inter.position().x(), 0.5);
	EXPECT_FLOAT_EQ(space_inter.position().y(), 0.5);
	
	// Intersection t-parameters.
	EXPECT_FLOAT_EQ(plane_inter.thisParameter(), 0.5);
	EXPECT_FLOAT_EQ(plane_inter.otherParameter(), 0.5);
	EXPECT_FLOAT_EQ(space_inter.thisParameter(), 0.5);
	EXPECT_FLOAT_EQ(space_inter.otherParameter(), 0.5);
}


TEST(LineSegment3D, ParallelIntersection) {
	// Define few vectors.
	math::linear::StaticVector<float, 3> zero({0.0, 0.0, 0.0});
	math::linear::StaticVector<float, 3> i({1.0, 0.0, 0.0});
	math::linear::StaticVector<float, 3> j({0.0, 1.0, 0.0});
	math::linear::StaticVector<float, 3> k({0.0, 0.0, 0.1});
	
	// Define few segments.
	math::geometry3::LineSegment<float> down(zero, i);
	math::geometry3::LineSegment<float> up(j, i+j);
	math::geometry3::LineSegment<float> vertical(i+j, i+j+k);
	math::geometry3::LineSegment<float> horizontal(j, i);
	
	// Get intersection data.
	auto plane_inter = up.intersect(down);
	auto space_inter = horizontal.intersect(vertical);
	
	// Were there intersection?
	EXPECT_FALSE(plane_inter.hasHit());
	EXPECT_FALSE(space_inter.hasHit());
}