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
	EXPECT_EQ(iseg.length_squared(), 1.0);
	
	EXPECT_EQ(jseg.start(), zero);
	EXPECT_EQ(jseg.end(), j);
	EXPECT_EQ(jseg.displacement(), j);
	EXPECT_EQ(jseg.length_squared(), 1.0);
	
	EXPECT_EQ(vseg.start(), i);
	EXPECT_EQ(vseg.end(), one);
	EXPECT_EQ(vseg.displacement(), j);
	EXPECT_EQ(vseg.length_squared(), 1.0);
	
	EXPECT_EQ(hseg.start(), j);
	EXPECT_EQ(hseg.end(), one);
	EXPECT_EQ(hseg.displacement(), i);
	EXPECT_EQ(hseg.length_squared(), 1.0);
	
	EXPECT_EQ(diag.start(), zero);
	EXPECT_EQ(diag.end(), one);
	EXPECT_EQ(diag.displacement(), i+j);
	EXPECT_EQ(diag.length_squared(), 2.0);
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
	EXPECT_EQ(plane_inter.hasHit(), true);
	EXPECT_EQ(space_inter.hasHit(), true);
	
	// Intersection position.
	EXPECT_EQ(plane_inter.position().x(), 0.5);
	EXPECT_EQ(plane_inter.position().y(), 0.5);
	EXPECT_EQ(space_inter.position().x(), 0.5);
	EXPECT_EQ(space_inter.position().y(), 0.5);
	
	// Intersection t-parameters.
	EXPECT_EQ(plane_inter.thisLine(), 0.5);
	EXPECT_EQ(plane_inter.otherLine(), 0.5);
	EXPECT_EQ(space_inter.thisLine(), 0.5);
	EXPECT_EQ(space_inter.otherLine(), 0.5);
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
	EXPECT_EQ(plane_inter.hasHit(), false);
	EXPECT_EQ(space_inter.hasHit(), false);
}