#pragma once
#include <cmath>
#include <math/geometry/intersection.hpp>
#include <math/linear/static_vector.hpp>
#include <math/geometry/2D/ray_base.hpp>

namespace math {
namespace geometry2 {


template <typename T>
class LineSegment : public RayBase<T> {
public:
	// Constructor.
	explicit LineSegment(const math::linear::StaticVector<T, 2>& end)
	: RayBase<T>(end, true, true, T(1.0)) {}
	
	explicit LineSegment(const math::linear::StaticVector<T, 2>& start, const math::linear::StaticVector<T, 2>& end)
	: RayBase<T>(start, end - start, true, true, T(1.0)) {}
	
	// End vector.
	math::linear::StaticVector<T, 2> end() const;
	
	// Length and displacement.
	math::linear::StaticVector<T, 2> displacement() const;
	T length_squared() const;
	T length(const std::function<T(T)>& sqrt = [](auto const& x) {return std::sqrt(x);}) const;
};


template <typename T>
math::linear::StaticVector<T, 2> LineSegment<T>::end() const {
	return this->start() + this->terminationParameter() * this->direction();
}

template <typename T>
inline math::linear::StaticVector<T, 2> LineSegment<T>::displacement() const {
	return this->direction();
}

template <typename T>
inline T LineSegment<T>::length_squared() const {
	return this->displacement().dot();
}

template <typename T>
inline T LineSegment<T>::length(const std::function<T(T)>& sqrt) const {
	return sqrt(this->length_squared());
}


} // namespace geometry2
} // namespace math
