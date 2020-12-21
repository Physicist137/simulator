#pragma once
#include <math/linear/static_vector.hpp>

namespace math {
namespace geometry2 {

template <typename T>
class LineSegment {
	math::linear::StaticVector<T, 2> _start;
	math::linear::StaticVector<T, 2> _end;

public:
	explicit LineSegment(const math::linear::StaticVector<T, 2>& start, math::linear::StaticVector<T, 2>& end) : _start(start), _end(end) {}
	explicit LineSegment(const math::linear::StaticVector<T, 2>& end) : _start(), _end(end) {}

	math::linear::StaticVector<T, 2> displacement() const;
};

template <typename T>
inline math::linear::StaticVector<T, 2> LineSegment<T>::displacement() const {
	return _end - _start;
}


} // namespace geometry2
} // namespace math
