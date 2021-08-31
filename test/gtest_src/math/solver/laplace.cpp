#include <gtest/gtest.h>
#include <math/solver/laplace.hpp>

template <typename T, typename E>
void display_grid(const math::solver::laplace2::FDM<T,E>& grid) {
	std::cout << "start=" << grid.domainfromij(0,0) << ", step=" << grid.domainfromij(1,1) << std::endl;
	for (int j = grid.sizey()-1; j >= 0; --j) {
		for (int i = 0; i < grid.sizex(); ++i) {
			std::cout << grid.dataEvaluation(i,j).value() << "  ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


TEST(LaplaceFDM, ConstructorAndBasics) {
	unsigned size = 5;
	float step = 1.0;
	math::solver::laplace2::FDM<float, float> fdm(size, size, step);
	for (unsigned i = 0; i < size; ++i) {
		for (unsigned j = 0; j < size; ++j) {
			EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 0.0);
		}
	}
	
	fdm.setBoundary(math::solver::laplace2::GridEdge::LeftEdge, 1.0);
	for (unsigned i = 0; i < size; ++i) {
		for (unsigned j = 0; j < size; ++j) {
			if (i == 0) EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 1.0);
			else EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 0.0);
		}
	}
	
	fdm.setBoundary(math::solver::laplace2::GridEdge::RightEdge, 2.0);
	for (unsigned i = 0; i < size; ++i) {
		for (unsigned j = 0; j < size; ++j) {
			if (i == size-1) EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 2.0);
			else if (i == 0) EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 1.0);
			else EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 0.0);
		}
	}
	
	fdm.setBoundary(math::solver::laplace2::GridEdge::UpperEdge, 3.0);
	for (unsigned i = 0; i < size; ++i) {
		for (unsigned j = 0; j < size; ++j) {
			if (j == size-1) EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 3.0);
			else if (i == size-1) EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 2.0);
			else if (i == 0) EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 1.0);
			else EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 0.0);
		}
	}
	
	fdm.setBoundary(math::solver::laplace2::GridEdge::LowerEdge, 4.0);
	for (unsigned i = 0; i < size; ++i) {
		for (unsigned j = 0; j < size; ++j) {
			if (j == 0) EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 4.0);
			else if (j == size-1) EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 3.0);
			else if (i == size-1) EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 2.0);
			else if (i == 0) EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 1.0);
			else EXPECT_FLOAT_EQ(fdm.dataEvaluation(i,j).value(), 0.0);
		}
	}
	
	/*
	display_grid<float,float>(fdm);
	
	fdm.setBoundary(math::solver::laplace2::GridEdge::LeftEdge, 1.0);
	fdm.setBoundary(math::solver::laplace2::GridEdge::RightEdge, 1.0);
	fdm.setBoundary(math::solver::laplace2::GridEdge::UpperEdge, 1.0);
	fdm.setBoundary(math::solver::laplace2::GridEdge::LowerEdge, 1.0);
	
	display_grid<float,float>(fdm);
	fdm.naiveIteration();
	display_grid<float,float>(fdm);
	fdm.naiveIteration();
	display_grid<float,float>(fdm);
	*/
}