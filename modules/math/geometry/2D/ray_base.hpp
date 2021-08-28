#pragma once
#include <cmath>
#include <math/geometry/intersection.hpp>
#include <math/linear/static_vector.hpp>

namespace math {
namespace geometry2 {

template <typename T>
class RayBase {
	math::linear::StaticVector<T, 2> _start;
	math::linear::StaticVector<T, 2> _direction;
	bool _directed;
	bool _terminated;
	T _termination_parameter;

public:
	explicit RayBase(const math::linear::StaticVector<T, 2>& direction, bool directed, bool terminated, const T& parameter)
	: _start(), _direction(direction), _directed(directed), _terminated(terminated), _termination_parameter(parameter) {}
	
	explicit RayBase(const math::linear::StaticVector<T, 2>& start, const math::linear::StaticVector<T, 2>& direction, bool directed, bool terminated, const T& parameter)
	: _start(start), _direction(direction), _directed(directed), _terminated(terminated), _termination_parameter(parameter) {}

	inline bool directed() const {return _directed;}
	inline bool isDirectedRay() const {return _directed;}
	
	inline bool terminated() const {return _terminated;}
	inline bool isTerminatedRay() const {return _terminated;}
	inline const T& terminationParameter() const {return _termination_parameter;}
	
	inline const math::linear::StaticVector<T, 2>& start() const {return _start;}
	inline const math::linear::StaticVector<T, 2>& direction() const {return _direction;}
	
	inline void setDirected(bool value) {_directed = value;}
	inline void setTerminated(bool value) {_terminated = value;}
	inline void setTerminationParameter(const T& value) {_termination_parameter = value;}
	
	math::geometry::IntersectionData<T,2> intersect(const RayBase& other) const;
};


template <typename T>
typename math::geometry::IntersectionData<T,2> RayBase<T>::intersect(const RayBase& other) const {
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
	

	// Rule out no intersection scenarios from this ray directness and termination.
	if (this->directed()) {
		if (t < 0) return math::geometry::IntersectionData<T,2>();
		if (this->terminated() and  t > this->terminationParameter()) return math::geometry::IntersectionData<T,2>();
	}
	
	else {
		if (this->terminated()) {
			if (t < -this->terminationParameter()) return math::geometry::IntersectionData<T,2>();
			if (t > this->terminationParameter()) return math::geometry::IntersectionData<T,2>();
		}
	}


	// Rule out no intersection scenarios from other ray directness and termination.
	if (other.directed()) {
		if (s < 0) return math::geometry::IntersectionData<T,2>();
		if (other.terminated() and  s > other.terminationParameter()) return math::geometry::IntersectionData<T,2>();
	}
	
	else {
		if (other.terminated()) {
			if (s < -other.terminationParameter()) return math::geometry::IntersectionData<T,2>();
			if (s > other.terminationParameter()) return math::geometry::IntersectionData<T,2>();
		}
	}
	
	// Return intersection.
	math::linear::StaticVector<T, 2> pos = _start + this->direction() * t;
	return math::geometry::IntersectionData<T,2>(t, s, pos);
}


} // namespace geometry2
} // namespace math
