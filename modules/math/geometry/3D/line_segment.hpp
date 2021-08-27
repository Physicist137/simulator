#pragma once
#include <iostream>
#include <math/linear/static_vector.hpp>
#include <math/linear/static_matrix.hpp>
#include <math/geometry/intersection.hpp>

namespace math {
namespace geometry3 {

template <typename T>
class LineSegment {
	math::linear::StaticVector<T, 3> _start;
	math::linear::StaticVector<T, 3> _end;

public:
	explicit LineSegment(const math::linear::StaticVector<T, 3>& start, const math::linear::StaticVector<T, 3>& end) : _start(start), _end(end) {}
	explicit LineSegment(const math::linear::StaticVector<T, 3>& end) : _start(), _end(end) {}

	inline const math::linear::StaticVector<T, 3>& start() const {return _start;}
	inline const math::linear::StaticVector<T, 3>& end() const {return _end;}
	
	math::linear::StaticVector<T, 3> displacement() const;
	T length_squared() const;

	math::geometry::IntersectionData<T,3> intersect(const LineSegment& other) const;
};


template <typename T>
inline math::linear::StaticVector<T, 3> LineSegment<T>::displacement() const {
	return _end - _start;
}

template <typename T>
inline T LineSegment<T>::length_squared() const {
	return this->displacement().dot();
}

template <typename T>
typename math::geometry::IntersectionData<T,3> LineSegment<T>::intersect(const LineSegment& other) const {
	// Get differences.
	math::linear::StaticVector<T, 3> diff_this = _end - _start;
	math::linear::StaticVector<T, 3> diff_other = other._end - other._start;
	math::linear::StaticVector<T, 3> diff_cross = other._start - _start;

	// Get matrix elements.
	T a11 = diff_this.x();
	T a21 = diff_this.y();
	T a31 = diff_this.z();
	T a12 = diff_other.x();
	T a22 = diff_other.y();
	T a32 = diff_other.z();
	T b1 = diff_cross.x();
	T b2 = diff_cross.y();
	T b3 = diff_cross.z();
	
	// Ignore third dimension. Calculate solution.
	// Compute determinant and rule out parallel lines.
	T det = a11 * a22  - a12 * a21;
	if (det == 0.0) return math::geometry::IntersectionData<T,3>();

	// Compute solution.
	T t = (a22*b1 - a12*b2) / det;
	T s = (a21*b1 - a11*b2) / det;

	// Rule out no intersection scenarios.
	if (t < 0) return math::geometry::IntersectionData<T,3>();
	if (s < 0) return math::geometry::IntersectionData<T,3>();
	if (t > 1) return math::geometry::IntersectionData<T,3>();
	if (s > 1) return math::geometry::IntersectionData<T,3>();
	
	
	// Proceed and analyze third dimension
	// Partial solution by echelon form. Compare with new solution.
	if (a31 != 0) {
		T num = b3 * a21 / a31 - b2;
		T den = a32 * a21 / a31 - a22;
		if (den != 0  and  s != num / den) return math::geometry::IntersectionData<T,3>();
	}
	
	else if (a32 != 0) {
		T num = b3 * a22 / a32 - b2;
		T den = a31 * a22 / a32 - a21;
		if (den != 0  and  t != num / den) return math::geometry::IntersectionData<T,3>();
	}
	
	// Return intersection.
	math::linear::StaticVector<T, 3> pos = _start + diff_this * t;
	return math::geometry::IntersectionData<T,3>(t, s, pos);
}


} // namespace geometry3
} // namespace math
