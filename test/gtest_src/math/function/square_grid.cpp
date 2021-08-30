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
	
	
	unsigned size = 10;
	float spacing = 1.0 / static_cast<float>(size);
	SquareGridFunction<float, float> larger_grid(size, size, spacing, zero);
	for (unsigned i = 0; i < size; ++i) {
		for (unsigned j = 0; j < size; ++j) {
			EXPECT_FLOAT_EQ(larger_grid.dataEvaluation(i,j), 0.0);
		}
	}
	
	larger_grid.setValueAllSquares(2.0);
	for (unsigned i = 0; i < size; ++i) {
		for (unsigned j = 0; j < size; ++j) {
			EXPECT_FLOAT_EQ(larger_grid.dataEvaluation(i,j), 2.0);
		}
	}
}

TEST(SquareGridTest, GradientTests) {
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	
	SquareGridFunction<float, float> grid(10, 10, 0.1, {0.0, 0.0});
	grid.setValueAllSquares(2.0);
	auto partialx = grid.partial_x();
	
	EXPECT_FLOAT_EQ(grid.dataEvaluation(5, 5), 2.0);
	EXPECT_FLOAT_EQ(partialx.evaluate(0.4, 0.4), 0.0);
}