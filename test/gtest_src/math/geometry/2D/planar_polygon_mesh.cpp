#include <gtest/gtest.h>
#include <cmath>
#include <math/linear/static_vector.hpp>
#include <math/geometry/2D/simple_polygon.hpp>
#include <math/geometry/2D/planar_polygon_mesh.hpp>


TEST(PlanarPolygonMesh, ConstructorAndBasicFunctions) {
	// Define few vectors.
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	
	math::geometry2::PlanarPolygonMesh<float> grid;
	
	unsigned v00 = grid.addVertex(zero);
	unsigned v01 = grid.addVertex(i);
	unsigned v02 = grid.addVertex(i+i);
	
	unsigned v10 = grid.addVertex(j);
	unsigned v11 = grid.addVertex(j+i);
	unsigned v12 = grid.addVertex(j+i+i);
	
	unsigned v20 = grid.addVertex(j+j);
	unsigned v21 = grid.addVertex(j+j+i);
	unsigned v22 = grid.addVertex(j+j+i+i);
	
	grid.addFace({v00, v01, v11, v10});
	grid.addFace({v01, v02, v12, v11});
	grid.addFace({v10, v11, v21, v20});
	grid.addFace({v11, v12, v22, v21});
	
	EXPECT_EQ(grid.numberOfFaces(), 4);
	EXPECT_EQ(grid.numberOfVertices(), 9);
	EXPECT_FLOAT_EQ(grid.area(), 4.0);
	
	EXPECT_EQ(grid.face(0).numberOfVertices(), 4);
	EXPECT_EQ(grid.face(1).numberOfVertices(), 4);
	EXPECT_EQ(grid.face(2).numberOfVertices(), 4);
	EXPECT_EQ(grid.face(3).numberOfVertices(), 4);
	
	EXPECT_FLOAT_EQ(grid.face(0).area(), 1.0);
	EXPECT_FLOAT_EQ(grid.face(1).area(), 1.0);
	EXPECT_FLOAT_EQ(grid.face(2).area(), 1.0);
	EXPECT_FLOAT_EQ(grid.face(3).area(), 1.0);
	
	EXPECT_FLOAT_EQ(grid.face(0).perimeter(), 4.0);
	EXPECT_FLOAT_EQ(grid.face(1).perimeter(), 4.0);
	EXPECT_FLOAT_EQ(grid.face(2).perimeter(), 4.0);
	EXPECT_FLOAT_EQ(grid.face(3).perimeter(), 4.0);
}
