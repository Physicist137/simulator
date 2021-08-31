#pragma once
#include <vector>
#include <math/linear/static_vector.hpp>


template <typename T, typename E=T>
class SquareGridFunction {
	// Domain information.
	unsigned _sizex;
	unsigned _sizey;
	T _spacing;
	math::linear::StaticVector<T,2> _start;
	
	// Image information.
	std::vector<E> _data;

public:
	// Transfer from ij-coordinates to the image coordinates.
	unsigned datafromij(unsigned i, unsigned j) const;
	math::linear::StaticVector<T,2> domainfromij(unsigned i, unsigned j) const;
	
	// Linear interpolation.
	T linearBasisFunction(const math::linear::StaticVector<T,2>& coord) const;
	E linearInterpolationEvaluation(const math::linear::StaticVector<T,2>& coord) const;

public:
	// Constructor functions
	SquareGridFunction(unsigned sizex, unsigned sizey, const T& spacing, math::linear::StaticVector<T,2> start = math::linear::StaticVector<T,2>())
	: _sizex(sizex), _sizey(sizey), _spacing(spacing), _start(start), _data(sizex * sizey) {}
	
	// Accessor functions
	inline unsigned sizex() const {return _sizex;}
	inline unsigned sizey() const {return _sizey;}
	inline const T& spacing() const {return _spacing;}
	inline const math::linear::StaticVector<T,2>& start() const {return _start;}
	inline math::linear::StaticVector<T,2> end() const {return _start + _spacing * math::linear::StaticVector<T,2>({static_cast<T>(_sizex-1), static_cast<T>(_sizey-1)});}
	
	// Some other functions.
	const SquareGridFunction<T,E>& setValueAllSquares(const T& value);
	
	// Evaluation at grid points.
	const E& dataEvaluation(unsigned i, unsigned j) const;
	E& dataEvaluation(unsigned i, unsigned j);

	// Interpolated evaluation.
	E evaluate(const math::linear::StaticVector<T,2>& coord) const;
	E operator()(const math::linear::StaticVector<T,2>& coord) const;
	E evaluate(const T& x, const T& y) const;
	E operator()(const T& x, const T& y) const;
	
	// Partial derivative operators.
	E evaluate_partial_x(const math::linear::StaticVector<T,2>& coord) const;
	E evaluate_partial_y(const math::linear::StaticVector<T,2>& coord) const;
	SquareGridFunction<T,E> partial_x() const;
	SquareGridFunction<T,E> partial_y() const;
	
	// Gradient operators.
	math::linear::StaticVector<E,2> evaluate_gradient(const math::linear::StaticVector<T,2>& coord) const;
	SquareGridFunction<T,math::linear::StaticVector<E,2>> gradient() const;
};

template <typename T, typename E>
unsigned SquareGridFunction<T,E>::datafromij(unsigned i, unsigned j) const {
	// This assumes both i and j are aligned with xhat and yhat unit vectors.
	return j * _sizex + i;
}

template <typename T, typename E>
math::linear::StaticVector<T,2> SquareGridFunction<T,E>::domainfromij(unsigned i, unsigned j) const {
	return math::linear::StaticVector<T,2>({
		_start.x() + static_cast<T>(i) * _spacing,
		_start.y() + static_cast<T>(j) * _spacing
	});
}

template <typename T, typename E>
const E& SquareGridFunction<T,E>::dataEvaluation(unsigned i, unsigned j) const {
	return _data[datafromij(i,j)];
}

template <typename T, typename E>
E& SquareGridFunction<T,E>::dataEvaluation(unsigned i, unsigned j) {
	return _data[datafromij(i,j)];
}

template <typename T, typename E>
const SquareGridFunction<T,E>& SquareGridFunction<T,E>::setValueAllSquares(const T& value) {
	unsigned size = _data.size();
	for (unsigned k = 0; k < size; ++k) _data[k] = value;
	return *this;
}


template <typename T, typename E>
T SquareGridFunction<T,E>::linearBasisFunction(const math::linear::StaticVector<T,2>& coord) const {
	if (coord.x() > 1.0) return T();
	if (coord.y() > 1.0) return T();
	if (coord.x() < -1.0) return T();
	if (coord.y() < -1.0) return T();
	
	// Compute x-value.
	T xvalue;
	if (coord.x() > 0.0) xvalue = T(1.0) - coord.x();
	else xvalue = T(1.0) + coord.x();
	
	// Compute y-value.
	T yvalue;
	if (coord.y() > 0.0) yvalue = T(1.0) - coord.y();
	else yvalue = T(1.0) + coord.y();
	
	// Tensor product.
	return xvalue * yvalue;
}

template <typename T, typename E>
E SquareGridFunction<T,E>::linearInterpolationEvaluation(const math::linear::StaticVector<T,2>& coord) const {
	// Get left down corner point of the grid.
	unsigned i = static_cast<unsigned>(std::floor((coord.x() - _start.x()) / _spacing));
	unsigned j = static_cast<unsigned>(std::floor((coord.y() - _start.y()) / _spacing));
	
	// Check limits to prevent segmentation fault errors.
	if (i > _sizex-1) throw std::invalid_argument("Outside of domain of the function.");
	if (j > _sizey-1) throw std::invalid_argument("Outside of domain of the function.");
	
	// Check limits to verify if we are inside domain.
	if (coord.x() < start().x()) throw std::invalid_argument("Outside of domain of the function.");
	if (coord.y() < start().y()) throw std::invalid_argument("Outside of domain of the function.");
	if (coord.x() > end().x()) throw std::invalid_argument("Outside of domain of the function.");
	if (coord.y() > end().y()) throw std::invalid_argument("Outside of domain of the function.");
	
	// Detect if coordinates are in the edges.
	if (i == _sizex-1  or  j == _sizey-1) return dataEvaluation(i,j);

	// Interpolate.
	// E start = dataEvaluation(i,j);
	// E xdiff = (coord.x() - _start.x()) * (dataEvaluation(i+1,j) - dataEvaluation(i,j));
	// E ydiff = (coord.x() - _start.x()) * (dataEvaluation(i,j+1) - dataEvaluation(i,j));
	// return start + xdiff + ydiff;
	
	// Interpolate.
	return
		+ dataEvaluation(i,j) * linearBasisFunction((coord - domainfromij(i,j)) / _spacing)
		+ dataEvaluation(i,j+1) * linearBasisFunction((coord - domainfromij(i,j+1)) / _spacing)
		+ dataEvaluation(i+1,j) * linearBasisFunction((coord - domainfromij(i+1,j)) / _spacing)
		+ dataEvaluation(i+1,j+1) * linearBasisFunction((coord - domainfromij(i+1,j+1)) / _spacing)
	;
}

template <typename T, typename E>
E SquareGridFunction<T,E>::evaluate(const math::linear::StaticVector<T,2>& coord) const {
	return linearInterpolationEvaluation(coord);
}

template <typename T, typename E>
E SquareGridFunction<T,E>::operator()(const math::linear::StaticVector<T,2>& coord) const {
	return linearInterpolationEvaluation(coord);
}

template <typename T, typename E>
E SquareGridFunction<T,E>::evaluate(const T& x, const T& y) const {
	return linearInterpolationEvaluation(math::linear::StaticVector<T,2>({x, y}));
}

template <typename T, typename E>
E SquareGridFunction<T,E>::operator()(const T& x, const T& y) const {
	return linearInterpolationEvaluation(math::linear::StaticVector<T,2>({x, y}));
}

template <typename T, typename E>
E SquareGridFunction<T,E>::evaluate_partial_x(const math::linear::StaticVector<T,2>& coord) const {
	
}

template <typename T, typename E>
E SquareGridFunction<T,E>::evaluate_partial_y(const math::linear::StaticVector<T,2>& coord) const {
	
}

template <typename T, typename E>
math::linear::StaticVector<E,2> SquareGridFunction<T,E>::evaluate_gradient(const math::linear::StaticVector<T,2>& coord) const {
	
}

template <typename T, typename E>
SquareGridFunction<T,E> SquareGridFunction<T,E>::partial_x() const {
	// Define the grid new parameters.
	unsigned newsizex = _sizex - 2;
	unsigned newsizey = _sizey;
	T newspacing = _spacing;
	math::linear::StaticVector<T,2> one({static_cast<T>(1.0), 0.0});
	math::linear::StaticVector<T,2> newstart = _start + _spacing * one;
	
	// Declare and initialize the grid.
	SquareGridFunction<T,E> grid(newsizex, newsizey, newspacing, newstart);
	
	// Calculate central differences.
	for (unsigned i = 1; i < _sizex-1; ++i) {
		for (unsigned j = 0; j < _sizey; ++j) {
			E diff = dataEvaluation(i+1,j) - dataEvaluation(i-1,j);
			grid.dataEvaluation(i-1,j) = diff / newspacing / 2.0;
		}
	}
	
	// Return the partial_x grid.
	return grid;
}

template <typename T, typename E>
SquareGridFunction<T,E> SquareGridFunction<T,E>::partial_y() const {
	// Define the grid new parameters.
	unsigned newsizex = _sizex;
	unsigned newsizey = _sizey-2;
	T newspacing = _spacing;
	math::linear::StaticVector<T,2> one({0.0, static_cast<T>(1.0)});
	math::linear::StaticVector<T,2> newstart = _start + _spacing * one;
	
	// Declare and initialize the grid.
	SquareGridFunction<T,E> grid(newsizex, newsizey, newspacing, newstart);
	
	// Calculate central differences.
	for (unsigned i = 0; i < _sizex; ++i) {
		for (unsigned j = 1; j < _sizey-1; ++j) {
			E diff = dataEvaluation(i,j+1) - dataEvaluation(i,j-1);
			grid.dataEvaluation(i,j-1) = diff / newspacing / 2.0;
		}
	}
	
	// Return the partial_y grid.
	return grid;
}

template <typename T, typename E>
SquareGridFunction<T,math::linear::StaticVector<E,2>> SquareGridFunction<T,E>::gradient() const {
	// Define the grid new parameters.
	unsigned newsizex = _sizex-2;
	unsigned newsizey = _sizey-2;
	T newspacing = _spacing;
	math::linear::StaticVector<T,2> one({static_cast<T>(1.0), static_cast<T>(1.0)});
	math::linear::StaticVector<T,2> newstart = _start + _spacing * one;
	
	// Declare and initialize the grid.
	SquareGridFunction<T,math::linear::StaticVector<E,2>> grid(newsizex, newsizey, newspacing, newstart);
	
	// Calculate central differences.
	for (unsigned i = 1; i < _sizex-1; ++i) {
		for (unsigned j = 1; j < _sizey-1; ++j) {
			E xdiff = dataEvaluation(i+1,j) - dataEvaluation(i-1,j);
			E ydiff = dataEvaluation(i,j+1) - dataEvaluation(i,j-1);
			
			E xpartial = xdiff / newspacing / 2.0;
			E ypartial = ydiff / newspacing / 2.0;
			
			grid.dataEvaluation(i-1,j-1) = math::linear::StaticVector<E,2>({xpartial, ypartial});
		}
	}
	
	// Return the gradient.
	return grid;
}
