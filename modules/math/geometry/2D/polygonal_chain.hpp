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
class PolygonalChain {
	std::vector<math::linear::StaticVector<T, 2>> _vertices;
	math::geometry::IntersectionData<T,2> _self_intersection_data;

protected:
	void clear_self_intersection_data();
	void load_self_intersection_data();

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
	
	// Self intersection data.
	inline const math::geometry::IntersectionData<T,2>& selfIntersectionData() const {return _self_intersection_data;}
	const PolygonalChain& calculateSelfIntersectionData();
	const PolygonalChain& clearSelfIntersectionData();
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


// Self Intersection
template <typename T>
void PolygonalChain<T>::load_self_intersection_data() {
	unsigned size = _vertices.size();
	for (unsigned i = 1; i < size; ++i) {
		for (unsigned j = i+2; j < size; ++j) {
			math::geometry2::LineSegment<T> first(_vertices[i-1], _vertices[i]);
			math::geometry2::LineSegment<T> second(_vertices[j-1], _vertices[j]);
			math::geometry::IntersectionData<T,2> inter = first.intersect(second);
			if (inter.hasHit()) _self_intersection_data.addIntersection(inter);
		}
	}
}

template <typename T>
void PolygonalChain<T>::clear_self_intersection_data() {
	_self_intersection_data.clear();
}

template <typename T>
const PolygonalChain<T>& PolygonalChain<T>::calculateSelfIntersectionData() {
	this->clear_self_intersection_data();
	this->load_self_intersection_data();
	return *this;
}

template <typename T>
const PolygonalChain<T>& PolygonalChain<T>::clearSelfIntersectionData() {
	this->clear_self_intersection_data();
	return *this;
}

}	// geometry2 namespace.
}	// math namespace.