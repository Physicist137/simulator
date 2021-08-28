#pragma once
#include <cmath>
#include <math/geometry/intersection.hpp>
#include <math/geometry/2D/line_segment.hpp>
#include <math/linear/static_vector.hpp>

namespace math {
namespace geometry2 {

template <typename T>
class LineSegment;


template <typename T>
class Ray {
	math::linear::StaticVector<T, 2> _start;
	math::linear::StaticVector<T, 2> _direction;
	bool _directed;

public:
	explicit Ray(const math::linear::StaticVector<T, 2>& direction, bool directed = true) : _start(), _direction(direction), _directed(directed) {}
	explicit Ray(const math::linear::StaticVector<T, 2>& start, const math::linear::StaticVector<T, 2>& direction, bool directed = true)
	: _start(start), _direction(direction), _directed(directed) {}

	inline bool directed() const {return _directed;}
	inline bool isDirectedRay() const {return _directed;}
	inline const math::linear::StaticVector<T, 2>& start() const {return _start;}
	inline const math::linear::StaticVector<T, 2>& direction() const {return _direction;}
	
	inline void setDirected(bool value) {_directed = value;}
	
	math::geometry::IntersectionData<T,2> intersect(const Ray& other) const;
	math::geometry::IntersectionData<T,2> intersect(const math::geometry2::LineSegment<T>& other) const;
};


template <typename T>
typename math::geometry::IntersectionData<T,2> Ray<T>::intersect(const Ray& other) const {
	// Get differences.
	math::linear::StaticVector<T, 2> diff = other._start - _start;

	// Get matrix elements.
	T a11 = this->direction().x();
	T a12 = other.direction().x();
	T a21 = this->direction().y();
	T a22 = other.direction().y();
	T b1 = diff.x();
	T b2 = diff.y();

	// Compute determinant.
	T det = a11 * a22  - a12 * a21;
	if (det == 0.0) return math::geometry::IntersectionData<T,2>();

	// Compute solution.
	T t = (a22*b1 - a12*b2) / det;
	T s = (a21*b1 - a11*b2) / det;

	// Rule out no intersection scenarios from ray directness.
	if (this->directed() and  t < 0) return math::geometry::IntersectionData<T,2>();
	if (other.directed() and  s < 0) return math::geometry::IntersectionData<T,2>();

	// Return intersection.
	math::linear::StaticVector<T, 2> pos = _start + this->direction() * t;
	return math::geometry::IntersectionData<T,2>(t, s, pos);
}

template <typename T>
typename math::geometry::IntersectionData<T,2> Ray<T>::intersect(const math::geometry2::LineSegment<T>& other) const {
	Ray<T> other_ray(other.start(), other.end() - other.start());
	auto data = this->intersect(other_ray);
	if (data.hasHit()  and  data.otherParameter() > 1) return math::geometry::IntersectionData<T,2>();
	else return data;
}


} // namespace geometry2
} // namespace math
