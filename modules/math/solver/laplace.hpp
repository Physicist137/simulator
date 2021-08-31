#pragma once
#include <math/function/square_grid.hpp>
#include <math/solver/finite_element.hpp>
#include <math/geometry/2D/simple_polygon.hpp>

namespace math {
namespace solver {
namespace laplace2 {
	
enum class GridEdge {
	RightEdge, LeftEdge, UpperEdge, LowerEdge
};


template <typename T, typename E>
class FDM : public math::function::SquareGrid<T, math::solver::FiniteElement<E>> {
	// Copy auxiliary.
	math::function::SquareGrid<T,E> _copy;
	
public:
	// Set up constructor alinged with SquareGrid.
	FDM(unsigned sizex, unsigned sizey, const T& spacing, math::linear::StaticVector<T,2> start = math::linear::StaticVector<T,2>())
	: math::function::SquareGrid<T, math::solver::FiniteElement<E>>(sizex, sizey, spacing, start), _copy(sizex, sizey, spacing, start) {}

	// Set up boundary terms.
	FDM& setBoundary(GridEdge edge, const E& value = E());
	FDM& setBoundary(unsigned i, unsigned j, const E& value = E());
	FDM& setBoundary(const math::geometry2::SimplePolygon<T>& polygon, const E& value = E());

public:
	void naiveIteration();
};


template <typename T, typename E>
FDM<T,E>& FDM<T,E>::setBoundary(GridEdge edge, const E& value) {
	if (edge == GridEdge::RightEdge) {
		unsigned i = this->sizex() - 1;
		unsigned sy = this->sizey();
		for (unsigned j = 0; j < sy; ++j) this->setBoundary(i,j,value);
		return *this;
	}
	
	if (edge == GridEdge::LeftEdge) {
		unsigned i = 0;
		unsigned sy = this->sizey();
		for (unsigned j = 0; j < sy; ++j) this->setBoundary(i,j,value);
		return *this;
	}
	
	if (edge == GridEdge::UpperEdge) {
		unsigned j = this->sizey() - 1;
		unsigned sx = this->sizex();
		for (unsigned i = 0; i < sx; ++i) this->setBoundary(i,j,value);
		return *this;
	}
	
	if (edge == GridEdge::LowerEdge) {
		unsigned j = 0;
		unsigned sx = this->sizex();
		for (unsigned i = 0; i < sx; ++i) this->setBoundary(i,j,value);
		return *this;
	}
	
	return *this;
}

template <typename T, typename E>
FDM<T,E>& FDM<T,E>::setBoundary(unsigned i, unsigned j, const E& value) {
	this->dataEvaluation(i, j) = value;
	this->dataEvaluation(i, j).setFrozen(true);
	return *this;
}

template <typename T, typename E>
FDM<T,E>& FDM<T,E>::setBoundary(const math::geometry2::SimplePolygon<T>& polygon, const E& value) {
	// Get the grid size.
	unsigned sx = this->sizex();
	unsigned sy = this->sizey();
	
	// For all points of the grid, check if it is inside polygon.
	// If inside, classify point as frozen.
	for (unsigned i = 0; i < sx; ++i) {
		for (unsigned j = 0; j < sy; ++j) {
			auto point = this->domainfromij(i, j);
			if (polygon.isInside(point)) {
				this->dataEvaluation(i, j) = value;
				this->dataEvaluation(i, j).setFrozen(true);
			}
		}
	}
	
	return *this;
}

template <typename T, typename E>
void FDM<T,E>::naiveIteration() {
	// Set up sizes.
	unsigned sx = this->sizex();
	unsigned sy = this->sizey();
	
	// Copy data.
	for (unsigned i = 0; i < sx; ++i) {
		for (unsigned j = 0; j < sy; ++j) {
			_copy.dataEvaluation(i,j) = this->dataEvaluation(i,j).value();
		}
	}
	
	
	for (unsigned i = 1; i < sx-1; ++i) {
		for (unsigned j = 1; j < sy-1; ++j) {
			if (this->dataEvaluation(i, j).frozen()) continue;
			
			E sum = 
				+ _copy.dataEvaluation(i+1, j)
				+ _copy.dataEvaluation(i-1, j)
				+ _copy.dataEvaluation(i, j+1)
				+ _copy.dataEvaluation(i, j-1)
			;
		
			this->dataEvaluation(i, j) = sum / 4.0;
		}
	}
}


}
}
}