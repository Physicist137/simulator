#include <gtest/gtest.h>
#include <cmath>
#include <math/linear/static_vector.hpp>
#include <math/geometry/2D/simple_polygon.hpp>


TEST(SimplePolygon2D, ConstructorAndBasicFunctions) {
	// Define few vectors.
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	math::linear::StaticVector<float, 2> one = i + j;
	
	// Define few segments.
	math::geometry2::SimplePolygon<float> square;
	square.addVertex(zero);
	square.addVertex(i);
	square.addVertex(i+j);
	square.addVertex(j);
	
	math::geometry2::SimplePolygon<float> triangle;
	triangle.addVertex(zero);
	triangle.addVertex(j);
	triangle.addVertex(i);
	
	math::geometry2::SimplePolygon<float> trapezoid;
	trapezoid.addVertex(zero);
	trapezoid.addVertex(j);
	trapezoid.addVertex(i+j);
	trapezoid.addVertex(2.0f * i);

	EXPECT_EQ(square.numberOfEdges(), 4);
	EXPECT_EQ(square.numberOfVertices(), 4);
	EXPECT_EQ(square.edge(0).displacement(), i);
	EXPECT_EQ(square.edge(1).displacement(), j);
	EXPECT_EQ(square.edge(2).displacement(), -i);
	EXPECT_EQ(square.edge(3).displacement(), -j);
	EXPECT_FLOAT_EQ(square.perimeter(), 4.0);
	EXPECT_FLOAT_EQ(square.area(), 1.0);
	
	EXPECT_EQ(triangle.numberOfEdges(), 3);
	EXPECT_EQ(triangle.numberOfVertices(), 3);
	EXPECT_FLOAT_EQ(triangle.perimeter(), 2.0 + std::sqrt(2.0));
	EXPECT_FLOAT_EQ(triangle.area(), 0.5);
	
	EXPECT_EQ(trapezoid.numberOfEdges(), 4);
	EXPECT_EQ(trapezoid.numberOfVertices(), 4);
	EXPECT_FLOAT_EQ(trapezoid.perimeter(), 4.0 + std::sqrt(2.0));
	EXPECT_FLOAT_EQ(trapezoid.area(), 3.0 / 2.0);
}

TEST(SimplePolygon2D, IntersectionOperations) {
	// Define few vectors.
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	math::linear::StaticVector<float, 2> one = i + j;
	
	// Define few segments.
	math::geometry2::SimplePolygon<float> square;
	square.addVertex(zero);
	square.addVertex(i);
	square.addVertex(i+j);
	square.addVertex(j);
	
	math::geometry2::SimplePolygon<float> triangle;
	triangle.addVertex(zero);
	triangle.addVertex(j);
	triangle.addVertex(i);
	
	math::geometry2::SimplePolygon<float> trapezoid;
	trapezoid.addVertex(zero);
	trapezoid.addVertex(j);
	trapezoid.addVertex(i+j);
	trapezoid.addVertex(2.0f * i);
	
	math::geometry2::LineSegment<float> segment(0.5f * one - i, 0.5f * one);
	EXPECT_FALSE(segment.intersect(square.edge(0)).hasHit());
	EXPECT_FALSE(segment.intersect(square.edge(1)).hasHit());
	EXPECT_FALSE(segment.intersect(square.edge(2)).hasHit());
	
	auto inter = segment.intersect(square.edge(3));
	EXPECT_FLOAT_EQ(inter.thisParameter(), 0.5);
	EXPECT_FLOAT_EQ(inter.otherParameter(), 0.5);
	
	inter = square.intersect(segment);
	EXPECT_TRUE(inter.hasHit());
	EXPECT_FLOAT_EQ(inter.thisParameter(), 0.5);
	EXPECT_FLOAT_EQ(inter.otherParameter(), 0.5);
	
	EXPECT_FALSE(square.isInside(0.5f * one - i));
	EXPECT_TRUE(square.isInside(0.5f * one - 0.2f * j));
	EXPECT_TRUE(square.isInside(0.5f * one));
}
