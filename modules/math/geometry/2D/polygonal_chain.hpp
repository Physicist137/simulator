#pragma once
#include <vector>
#include <functional>
#include <cmath>
#include <initializer_list>
#include <math/linear/static_vector.hpp>

namespace math {
namespace geometry2 {

template <typename T>
class PolygonalChain {
	std::vector<math::linear::StaticVector<T, 2>> _vertices;

public:
	// Constructor.
	PolygonalChain();
	PolygonalChain(std::initializer_list<math::linear::StaticVector<T, 2>> list);
	PolygonalChain(const std::vector<math::linear::StaticVector<T, 2>>& vec);
	PolygonalChain(const PolygonalChain<T>& other);
	PolygonalChain(PolygonalChain<T>& other);

	// Return special vertices.
	inline const math::linear::StaticVector<T, 2>& start() const {return _vertices.front();}
	inline const math::linear::StaticVector<T, 2>& end() const {return _vertices.back();}
	inline const math::linear::StaticVector<T, 2>& vertex(unsigned i) const {return _vertices[i];}
	inline const math::linear::StaticVector<T, 2>& operator[](unsigned i) const {return _vertices[i];}
	
	// Displacement and length.
	math::linear::StaticVector<T, 2> displacement() const;
	T length(const std::function<T(T)>& sqrt = [](auto const& x) {return std::sqrt(x);}) const;
};


template <typename T>
inline PolygonalChain<T>::PolygonalChain() {}

template <typename T>
inline PolygonalChain<T>::PolygonalChain(std::initializer_list<math::linear::StaticVector<T, 2>> list) {
	std::copy(list.begin(), list.end(), _vertices.begin());
}


template <typename T>
inline PolygonalChain<T>::PolygonalChain(const std::vector<math::linear::StaticVector<T, 2>>& vec) : _vertices(vec) {

}

template <typename T>
inline PolygonalChain<T>::PolygonalChain(const PolygonalChain<T>& other) : _vertices(other._vertices) {

}

template <typename T>
inline PolygonalChain<T>::PolygonalChain(PolygonalChain<T>& other) : _vertices(other._vertices) {

}

template <typename T>
math::linear::StaticVector<T, 2> PolygonalChain<T>::displacement() const {
	return end() - start();
}

template <typename T>
T PolygonalChain<T>::length(const std::function<T(T)>& sqrt) const {
	unsigned size = _vertices.size();
	T result = T();
	for (unsigned i = 1; i < size; ++i) {
		auto displacement = _vertices[i] - _vertices[i-1];
		result += sqrt(displacement.dot());
	}
	
	return result;
}

}	// geometry2 namespace.
}	// math namespace.