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
	
	// Square test.
	math::geometry2::SimplePolygon<float> square;
	square.addVertex(zero);
	square.addVertex(i);
	square.addVertex(i+j);
	square.addVertex(j);
	EXPECT_EQ(square.numberOfEdges(), 4);
	EXPECT_EQ(square.numberOfVertices(), 4);
	EXPECT_EQ(square.edge(0).displacement(), i);
	EXPECT_EQ(square.edge(1).displacement(), j);
	EXPECT_EQ(square.edge(2).displacement(), -i);
	EXPECT_EQ(square.edge(3).displacement(), -j);
	EXPECT_EQ(square.edge(4).displacement(), i);
	EXPECT_EQ(square.edge(5).displacement(), j);
	EXPECT_EQ(square.edge(6).displacement(), -i);
	EXPECT_EQ(square.edge(7).displacement(), -j);
	EXPECT_FLOAT_EQ(square.vertex(0).x(), 0.0);
	EXPECT_FLOAT_EQ(square.vertex(1).x(), 1.0);
	EXPECT_FLOAT_EQ(square.vertex(2).x(), 1.0);
	EXPECT_FLOAT_EQ(square.vertex(3).x(), 0.0);
	EXPECT_FLOAT_EQ(square.vertex(4).x(), 0.0);
	EXPECT_FLOAT_EQ(square.vertex(5).x(), 1.0);
	EXPECT_FLOAT_EQ(square.vertex(6).x(), 1.0);
	EXPECT_FLOAT_EQ(square.vertex(7).x(), 0.0);
	EXPECT_FLOAT_EQ(square.perimeter(), 4.0);
	EXPECT_FLOAT_EQ(square.area(), 1.0);
	
	// Displaced square test..
	math::linear::StaticVector<float, 2> dis = 2.971f * i + 5.813f * j;
	math::geometry2::SimplePolygon<float> displaced_square;
	displaced_square.addVertex(zero + dis);
	displaced_square.addVertex(i + dis);
	displaced_square.addVertex(i+j + dis);
	displaced_square.addVertex(j + dis);
	EXPECT_EQ(displaced_square.numberOfEdges(), 4);
	EXPECT_EQ(displaced_square.numberOfVertices(), 4);
	EXPECT_EQ(displaced_square.edge(0).displacement(), i);
	EXPECT_EQ(displaced_square.edge(1).displacement(), j);
	EXPECT_EQ(displaced_square.edge(2).displacement(), -i);
	EXPECT_EQ(displaced_square.edge(3).displacement(), -j);
	EXPECT_FLOAT_EQ(displaced_square.perimeter(), 4.0);
	EXPECT_FLOAT_EQ(displaced_square.area(), 1.0);
	
	// Rotated and displaced square test..
	math::geometry2::SimplePolygon<float> rotated_square;
	rotated_square.addVertex(i+dis);
	rotated_square.addVertex(j+dis);
	rotated_square.addVertex(-i+dis);
	rotated_square.addVertex(-j+dis);
	EXPECT_EQ(rotated_square.numberOfEdges(), 4);
	EXPECT_EQ(rotated_square.numberOfVertices(), 4);
	EXPECT_FLOAT_EQ(rotated_square.perimeter(), 4.0 * std::sqrt(2.0));
	EXPECT_FLOAT_EQ(rotated_square.area(), 2.0);
	
	// Triangle test.
	math::geometry2::SimplePolygon<float> triangle;
	triangle.addVertex(zero);
	triangle.addVertex(j);
	triangle.addVertex(i);
	EXPECT_EQ(triangle.numberOfEdges(), 3);
	EXPECT_EQ(triangle.numberOfVertices(), 3);
	EXPECT_FLOAT_EQ(triangle.perimeter(), 2.0 + std::sqrt(2.0));
	EXPECT_FLOAT_EQ(triangle.area(), 0.5);
	
	
	// Trapezoid test.
	math::geometry2::SimplePolygon<float> trapezoid;
	trapezoid.addVertex(zero);
	trapezoid.addVertex(j);
	trapezoid.addVertex(i+j);
	trapezoid.addVertex(2.0f * i);
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
	EXPECT_EQ(inter.numberOfHits(), 1);
	EXPECT_EQ(inter.location().vertex(), 3);
	EXPECT_FLOAT_EQ(inter.thisParameter(), 0.5);
	EXPECT_FLOAT_EQ(inter.otherParameter(), 0.5);
	
	EXPECT_FALSE(square.isInside(0.5f * one - i));
	EXPECT_TRUE(square.isInside(0.5f * one - 0.2f * j));
	EXPECT_TRUE(square.isInside(0.5f * one));
}


TEST(SimplePolygon2D, DegenerateIntersectionCases) {
	// Define few vectors.
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	math::linear::StaticVector<float, 2> one = i + j;
	
	// Define a square..
	math::geometry2::SimplePolygon<float> square;
	square.addVertex(zero);
	square.addVertex(i);
	square.addVertex(i+j);
	square.addVertex(j);
	
	// Define a ray that passes through 3rd vertex.
	math::geometry2::Ray<float> ray(0.5f * one, 1.5f * one);
	auto vertex_intersection = square.intersect(ray);
	EXPECT_TRUE(vertex_intersection.hasHit());
	EXPECT_EQ(vertex_intersection.numberOfHits(), 1);
	EXPECT_FLOAT_EQ(vertex_intersection.thisParameter(), 0.0);
	
	// Define a ray that coincides with the 3rd edge.
	math::geometry2::Ray<float> coincident(-i+j, i);
	auto coincidence_intersection = square.intersect(ray);
	EXPECT_TRUE(coincidence_intersection.hasHit());
	EXPECT_EQ(coincidence_intersection.numberOfHits(), 1);
	EXPECT_FLOAT_EQ(coincidence_intersection.thisParameter(), 0.0);
}
