#pragma once
#include <cmath>
#include <functional>
#include <math/geometry/intersection.hpp>
#include <math/geometry/2D/ray.hpp>
#include <math/linear/static_vector.hpp>

namespace math {
namespace geometry2 {

template <typename T>
class Ray;

template <typename T>
class LineSegment {
	math::linear::StaticVector<T, 2> _start;
	math::linear::StaticVector<T, 2> _end;

public:
	explicit LineSegment(const math::linear::StaticVector<T, 2>& start, const math::linear::StaticVector<T, 2>& end) : _start(start), _end(end) {}
	explicit LineSegment(const math::linear::StaticVector<T, 2>& end) : _start(), _end(end) {}

	inline const math::linear::StaticVector<T, 2>& start() const {return _start;}
	inline const math::linear::StaticVector<T, 2>& end() const {return _end;}
	
	math::linear::StaticVector<T, 2> displacement() const;
	T length_squared() const;
	T length(const std::function<T(T)>& sqrt = [](auto const& x) {return std::sqrt(x);}) const;

	math::geometry::IntersectionData<T,2> intersect(const LineSegment& other) const;
};


template <typename T>
inline math::linear::StaticVector<T, 2> LineSegment<T>::displacement() const {
	return _end - _start;
}

template <typename T>
inline T LineSegment<T>::length_squared() const {
	return this->displacement().dot();
}

template <typename T>
inline T LineSegment<T>::length(const std::function<T(T)>& sqrt) const {
	return sqrt(this->length_squared());
}


template <typename T>
typename math::geometry::IntersectionData<T,2> LineSegment<T>::intersect(const LineSegment& other) const {
	// Build rays from the line segments.
	Ray<T> ray_this(_start, _end - _start);
	Ray<T> ray_other(other.start(), other.end() - other.start());
	
	// Intersect the rays.
	auto inter = ray_this.intersect(ray_other);
	
	if (inter.hasHit()) {
		T t = inter.thisParameter();
		T s = inter.otherParameter();
		if (t > 1) return math::geometry::IntersectionData<T,2>();
		if (s > 1) return math::geometry::IntersectionData<T,2>();
	}

	return inter;
}


/*
template <typename T>
typename math::geometry::IntersectionData<T,2> LineSegment<T>::intersect(const LineSegment& other) const {
	// Get differences.
	math::linear::StaticVector<T, 2> diff_this = _end - _start;
	math::linear::StaticVector<T, 2> diff_other = other._end - other._start;
	math::linear::StaticVector<T, 2> diff_cross = other._start - _start;

	// Get matrix elements.
	T a11 = diff_this.x();
	T a12 = diff_other.x();
	T a21 = diff_this.y();
	T a22 = diff_other.y();
	T b1 = diff_cross.x();
	T b2 = diff_cross.y();

	// Compute determinant and rule out parallel lines.
	T det = a11 * a22  - a12 * a21;
	if (det == 0.0) return math::geometry::IntersectionData<T,2>();

	// Compute solution.
	T t = (a22*b1 - a12*b2) / det;
	T s = (a21*b1 - a11*b2) / det;

	// Rule out no intersection scenarios.
	if (t < 0) return math::geometry::IntersectionData<T,2>();
	if (s < 0) return math::geometry::IntersectionData<T,2>();
	if (t > 1) return math::geometry::IntersectionData<T,2>();
	if (s > 1) return math::geometry::IntersectionData<T,2>();

	// Return intersection.
	math::linear::StaticVector<T, 2> pos = _start + diff_this * t;
	return math::geometry::IntersectionData<T,2>(t, s, pos);
}*/


} // namespace geometry2
} // namespace math
