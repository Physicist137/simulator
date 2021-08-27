#include <gtest/gtest.h>
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

	EXPECT_EQ(square.perimeter(), 4.0);
	EXPECT_EQ(square.area(), 1.0);
}