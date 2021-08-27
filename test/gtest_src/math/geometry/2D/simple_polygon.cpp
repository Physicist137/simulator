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

	EXPECT_FLOAT_EQ(square.perimeter(), 4.0);
	EXPECT_FLOAT_EQ(square.area(), 1.0);
	EXPECT_FLOAT_EQ(triangle.perimeter(), 2.0 + std::sqrt(2.0));
	EXPECT_FLOAT_EQ(triangle.area(), 0.5);
	EXPECT_FLOAT_EQ(trapezoid.perimeter(), 4.0 + std::sqrt(2.0));
	EXPECT_FLOAT_EQ(trapezoid.area(), 3.0 / 2.0);
}
