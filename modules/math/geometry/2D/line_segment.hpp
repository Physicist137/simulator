#pragma once
#include <math/linear/static_vector.hpp>

namespace math {
namespace geometry2 {

template <typename T>
class LineSegment {
	math::linear::StaticVector<T, 2> _start;
	math::linear::StaticVector<T, 2> _end;

protected:
	class IntersectionData {
		bool _intersects;
		T _this;
		T _other;
		math::linear::StaticVector<T, 2> _position;

	public:
		explicit IntersectionData() : _intersects(false), _this(), _other(), _position() {}

		IntersectionData(
			const T& this_position,
			const T& other_position,
			const math::linear::StaticVector<T, 2>& position
		) : _intersects(true),  _this(this_position), _other(other_position), _position(position) {}

		inline const bool intersects() const {return _intersects;}
		inline const bool hits() const {return _intersects;}
		inline const bool doesIntersect() const {return _intersects;}
		inline const bool hasHit() const {return _intersects;}

		inline const T& thisLine() const {return _this;}
		inline const T& otherLine() const {return _other;}

		inline const T& t() const {return _this;}
		inline const T& s() const {return _other;}

		inline const math::linear::StaticVector<T, 2>& position() const {return _position;}
	};

public:
	explicit LineSegment(const math::linear::StaticVector<T, 2>& start, const math::linear::StaticVector<T, 2>& end) : _start(start), _end(end) {}
	explicit LineSegment(const math::linear::StaticVector<T, 2>& end) : _start(), _end(end) {}

	math::linear::StaticVector<T, 2> displacement() const;

	IntersectionData intersect(const LineSegment& other) const;
};

template <typename T>
inline math::linear::StaticVector<T, 2> LineSegment<T>::displacement() const {
	return _end - _start;
}

template <typename T>
typename LineSegment<T>::IntersectionData LineSegment<T>::intersect(const LineSegment& other) const {
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
	if (det == 0.0) return LineSegment<T>::IntersectionData();

	// Compute solution.
	T t = (a22*b1 - a12*b2) / det;
	T s = (a21*b1 - a11*b2) / det;

	// Rule out no intersection scenarios.
	if (t < 0) return LineSegment<T>::IntersectionData();
	if (s < 0) return LineSegment<T>::IntersectionData();
	if (t > 1) return LineSegment<T>::IntersectionData();
	if (s > 1) return LineSegment<T>::IntersectionData();

	// Return intersection.
	math::linear::StaticVector<T, 2> pos = _start + diff_this * t;
	return LineSegment<T>::IntersectionData(t, s, pos);
}


} // namespace geometry2
} // namespace math
