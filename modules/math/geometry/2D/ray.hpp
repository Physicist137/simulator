#pragma once
#include <cmath>
#include <math/geometry/intersection.hpp>
#include <math/linear/static_vector.hpp>
#include <math/geometry/2D/ray_base.hpp>

namespace math {
namespace geometry2 {


template <typename T>
class Ray : public RayBase<T> {
public:
	explicit Ray(const math::linear::StaticVector<T, 2>& direction)
	: RayBase<T>(direction, true, false, T()) {}
	
	
	explicit Ray(const math::linear::StaticVector<T, 2>& start, const math::linear::StaticVector<T, 2>& direction)
	: RayBase<T>(start, direction, true, false, T()) {}
};


} // namespace geometry2
} // namespace math
