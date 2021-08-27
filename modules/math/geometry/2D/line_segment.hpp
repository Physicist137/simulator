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

} // namespace geometry2
} // namespace math
