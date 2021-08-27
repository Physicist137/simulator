#pragma once
#include <vector>
#include <functional>
#include <cmath>
#include <initializer_list>
#include <math/linear/static_vector.hpp>
#include <math/geometry/intersection.hpp>
#include <math/geometry/2D/line_segment.hpp>

namespace math {
namespace geometry2 {


template <typename T>
class SimplePolygon {
	std::vector<math::linear::StaticVector<T, 2>> _vertices;

public:
	// Constructor.
	SimplePolygon();
	SimplePolygon(std::initializer_list<math::linear::StaticVector<T, 2>> list);
	SimplePolygon(const std::vector<math::linear::StaticVector<T, 2>>& vec);
	SimplePolygon(const SimplePolygon<T>& other);
	SimplePolygon(SimplePolygon<T>& other);

	// Return special vertices.
	inline const math::linear::StaticVector<T, 2>& start() const {return _vertices.front();}
	inline const math::linear::StaticVector<T, 2>& end() const {return _vertices.back();}
	inline const math::linear::StaticVector<T, 2>& vertex(unsigned i) const {return _vertices[i];}
	inline const math::linear::StaticVector<T, 2>& operator[](unsigned i) const {return _vertices[i % _vertices.size()];}
	
	// Polygon information
	inline unsigned faces() const {return _vertices.size();}
	T perimeter(const std::function<T(T)>& sqrt = [](auto const& x) {return std::sqrt(x);}) const;
	T signedArea() const;
	T area() const;
	
	// Add vertices.
	const SimplePolygon& addVertex(const math::linear::StaticVector<T, 2>& vertex);
};


// Constructor functions.
template <typename T> SimplePolygon<T>::SimplePolygon() : _vertices() {}
template <typename T> SimplePolygon<T>::SimplePolygon(const std::vector<math::linear::StaticVector<T, 2>>& vec) : _vertices(vec) {}
template <typename T> SimplePolygon<T>::SimplePolygon(const SimplePolygon<T>& other) : _vertices(other._vertices) {}
template <typename T> SimplePolygon<T>::SimplePolygon(SimplePolygon<T>& other) : _vertices(other._vertices) {}
template <typename T>
SimplePolygon<T>::SimplePolygon(std::initializer_list<math::linear::StaticVector<T, 2>> list) {
	std::copy(list.begin(), list.end(), _vertices.begin());
}



template <typename T>
const SimplePolygon<T>& SimplePolygon<T>::addVertex(const math::linear::StaticVector<T, 2>& vertex) {
	_vertices.push_back(vertex);
	return *this;
}

template <typename T>
T SimplePolygon<T>::perimeter(const std::function<T(T)>& sqrt) const {
	unsigned size = _vertices.size();
	T result = T();
	
	for (unsigned i = 1; i < size; ++i) {
		auto displacement = _vertices[i] - _vertices[i-1];
		result += sqrt(displacement.dot());
	}
	
	auto displacement = _vertices[0] - _vertices[size-1];
	result += sqrt(displacement.dot());
	
	return result;
}

template <typename T>
T SimplePolygon<T>::signedArea() const {
	unsigned size = _vertices.size();
	T result = T();
	
	for (unsigned i = 0; i <= size; ++i) {
		T sum = (*this)[i].x() * (*this)[i+1].y();
		T sub = (*this)[i+1].x() * (*this)[i].y();
		result += sum - sub;
	}
	
	return result / 2.0;
}

template <typename T>
T SimplePolygon<T>::area() const {
	T signed_area = signedArea();
	if (signed_area >= 0) return signed_area;
	else return -signed_area;
}



}
}