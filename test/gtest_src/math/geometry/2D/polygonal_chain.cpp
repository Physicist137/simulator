#include <gtest/gtest.h>

#include <math/linear/static_vector.hpp>
#include <math/geometry/2D/polygonal_chain.hpp>

TEST(PolygonalChain2D, ConstructorAndBasicFunctions) {
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	std::vector<math::linear::StaticVector<float, 2>> vec({zero, i, i+j});
	
	math::geometry2::PolygonalChain<float> chain1;
	math::geometry2::PolygonalChain<float> chain2(vec);
	
	// Solve this.
	//math::geometry2::PolygonalChain<float> chain3({zero, i, i+j});
	//math::geometry2::PolygonalChain<float> chain3({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}});
	
	EXPECT_EQ(chain2.start().x(), 0.0);
	EXPECT_EQ(chain2.start().y(), 0.0);
	EXPECT_EQ(chain2.end().x(), 1.0);
	EXPECT_EQ(chain2.end().y(), 1.0);
	EXPECT_EQ(chain2.vertex(1).x(), 1.0);
	EXPECT_EQ(chain2.vertex(1).y(), 0.0);
	EXPECT_EQ(chain2.length(), 2.0);
}