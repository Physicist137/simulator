#include <gtest/gtest.h>
#include <math/function/square_grid.hpp>


TEST(SquareGridTest, ConstructorAndBasics) {
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> i({1.0, 0.0});
	math::linear::StaticVector<float, 2> j({0.0, 1.0});
	math::function::SquareGridFunction<float, float> grid(2, 2, 1.0);
	
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
	math::function::SquareGridFunction<float, float> larger_grid(size, size, spacing, zero);
	for (unsigned i = 0; i < size; ++i) {
		for (unsigned j = 0; j < size; ++j) {
			EXPECT_FLOAT_EQ(larger_grid.dataEvaluation(i,j), 0.0);
		}
	}
	
	EXPECT_FLOAT_EQ(larger_grid.evaluate(0.128, 0.132), 0.0);
	EXPECT_FLOAT_EQ(larger_grid.evaluate(0.849, 0.153), 0.0);
	EXPECT_FLOAT_EQ(larger_grid.evaluate(0.328, 0.703), 0.0);
	EXPECT_FLOAT_EQ(larger_grid.evaluate(0.287, 0.540), 0.0);
	
	larger_grid.setValueAllSquares(2.0);
	EXPECT_TRUE(std::abs(larger_grid.evaluate(0.128, 0.132) - 2.0) < 1e-3);
	EXPECT_TRUE(std::abs(larger_grid.evaluate(0.849, 0.153) - 2.0) < 1e-3);
	EXPECT_TRUE(std::abs(larger_grid.evaluate(0.328, 0.703) - 2.0) < 1e-3);
	EXPECT_TRUE(std::abs(larger_grid.evaluate(0.287, 0.540) - 2.0) < 1e-3);
	
	for (unsigned i = 0; i < size; ++i) {
		for (unsigned j = 0; j < size; ++j) {
			EXPECT_FLOAT_EQ(larger_grid.dataEvaluation(i,j), 2.0);
		}
	}
}


template <typename T, typename E>
void display_grid(const math::function::SquareGridFunction<T,E>& grid) {
	std::cout << grid.domainfromij(0,0) << ", " << grid.domainfromij(1,1) << std::endl;
	for (int j = grid.sizey()-1; j >= 0; --j) {
		for (int i = 0; i < grid.sizex(); ++i) {
			std::cout << grid.dataEvaluation(i,j) << "  ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}



TEST(SquareGridTest, GradientTests) {
	math::linear::StaticVector<float, 2> zero({0.0, 0.0});
	math::linear::StaticVector<float, 2> ui({1.0, 0.0});
	math::linear::StaticVector<float, 2> uj({0.0, 1.0});
	math::linear::StaticVector<float, 2> start = zero;
	
	// Identity function.
	unsigned size = 6;	// 0,  0.2,  0.4,  0.6,  0.8,  1.0.
	float step = 1 / (static_cast<float>(size-1));
	
	// Quadratic function.
	math::function::SquareGridFunction<float, float> small(size, size, step);
	for (unsigned i = 0; i < size; ++i) {
		for (unsigned j = 0; j < size; ++j) {
			float fi = static_cast<unsigned>(i);
			float fj = static_cast<unsigned>(j);
			
			float r = step*fi + step*fj;
			float x = step*fi;
			float y = step*fj;
			
			small.dataEvaluation(i, j) = x*x / 2.0 + y*y / 2.0;
		}
	} 
	
	// Verify correct values in partial x.
	auto small_partial_x = small.partial_x();
	for (unsigned i = 0; i < small_partial_x.sizex(); ++i) {
		for (unsigned j = 0; j < small_partial_x.sizey(); ++j) {
			EXPECT_FLOAT_EQ(small_partial_x.dataEvaluation(i,j), small_partial_x.domainfromij(i,j).x());
		}
	}
	
	// Verify correct values in partial y.
	auto small_partial_y = small.partial_y();
	for (unsigned i = 0; i < small_partial_y.sizex(); ++i) {
		for (unsigned j = 0; j < small_partial_y.sizey(); ++j) {
			EXPECT_FLOAT_EQ(small_partial_y.dataEvaluation(i,j), small_partial_y.domainfromij(i,j).y());
		}
	}
	
	// Verify correct values in the gradient function.
	auto small_gradient = small.gradient();
	for (unsigned i = 0; i < small_gradient.sizex(); ++i) {
		for (unsigned j = 0; j < small_gradient.sizey(); ++j) {
			EXPECT_FLOAT_EQ(small_gradient.dataEvaluation(i,j).x(), small_gradient.domainfromij(i,j).x());
			EXPECT_FLOAT_EQ(small_gradient.dataEvaluation(i,j).y(), small_gradient.domainfromij(i,j).y());
		}
	}
	
	// Display grids.
	// display_grid<float, float>(small);
	// display_grid<float, float>(small_partial_x);
	// display_grid<float, float>(small_partial_y);
	// display_grid<float, math::linear::StaticVector<float,2>>(small_gradient);
}