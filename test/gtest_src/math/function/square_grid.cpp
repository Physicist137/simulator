#include <gtest/gtest.h>
#include <math/function/square_grid.hpp>


TEST(SquareGridTest, ConstructorAndBasics) {
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	SquareGridFunction<float, float> grid(2, 2, 1.0, {0.0, 0.0});
	
	EXPECT_EQ(grid.sizex(), 2);
	EXPECT_EQ(grid.sizey(), 2);
	EXPECT_FLOAT_EQ(grid.spacing(), 1.0);
	
	EXPECT_EQ(grid.start(), zero);
	EXPECT_EQ(grid.domainfromij(0,0), zero);
	EXPECT_EQ(grid.domainfromij(0,1), j);
	EXPECT_EQ(grid.domainfromij(1,0), i);
	EXPECT_EQ(grid.domainfromij(1,1), i+j);
	
	EXPECT_FLOAT_EQ(grid.dataEvaluation(0,0), 0.0);
	EXPECT_FLOAT_EQ(grid.dataEvaluation(0,1), 0.0);
	EXPECT_FLOAT_EQ(grid.dataEvaluation(1,0), 0.0);
	EXPECT_FLOAT_EQ(grid.dataEvaluation(1,1), 0.0);
	
	EXPECT_FLOAT_EQ(grid.evaluate(0.0,0.0), 0.0);
	EXPECT_FLOAT_EQ(grid.evaluate(0.0,1.0), 0.0);
	EXPECT_FLOAT_EQ(grid.evaluate(1.0,0.0), 0.0);
	EXPECT_FLOAT_EQ(grid.evaluate(1.0,1.0), 0.0);
	EXPECT_FLOAT_EQ(grid.evaluate(0.5,0.5), 0.0);
	
	grid.dataEvaluation(1,1) = 4;
	EXPECT_FLOAT_EQ(grid.dataEvaluation(0,0), 0.0);
	EXPECT_FLOAT_EQ(grid.dataEvaluation(0,1), 0.0);
	EXPECT_FLOAT_EQ(grid.dataEvaluation(1,0), 0.0);
	EXPECT_FLOAT_EQ(grid.dataEvaluation(1,1), 4.0);
	
	EXPECT_FLOAT_EQ(grid.evaluate(0.0,0.0), 0.0);
	EXPECT_FLOAT_EQ(grid.evaluate(0.0,1.0), 0.0);
	EXPECT_FLOAT_EQ(grid.evaluate(1.0,0.0), 0.0);
	EXPECT_FLOAT_EQ(grid.evaluate(1.0,1.0), 4.0);
	EXPECT_FLOAT_EQ(grid.evaluate(0.5,0.5), 1.0);
}