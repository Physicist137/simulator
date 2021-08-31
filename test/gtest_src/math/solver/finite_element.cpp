#include <gtest/gtest.h>
#include <math/solver/finite_element.hpp>


TEST(FiniteElement, ConstructorAndBasics) {
	math::solver::FiniteElement<float> element;
	EXPECT_FLOAT_EQ(element.value(), 0.0);
	EXPECT_FALSE(element.frozen());
	EXPECT_FALSE(element.boundary());
	
	element = 2;
	EXPECT_FLOAT_EQ(element.value(), 2.0);
	EXPECT_FALSE(element.frozen());
	EXPECT_FALSE(element.boundary());
	
	element *= element;
	EXPECT_FLOAT_EQ(element.value(), 4.0);
	EXPECT_FALSE(element.frozen());
	EXPECT_FALSE(element.boundary());
	
	element += element;
	EXPECT_FLOAT_EQ(element.value(), 8.0);
	EXPECT_FALSE(element.frozen());
	EXPECT_FALSE(element.boundary());
	
	element -= element;
	EXPECT_FLOAT_EQ(element.value(), 0.0);
	EXPECT_FALSE(element.frozen());
	EXPECT_FALSE(element.boundary());
}