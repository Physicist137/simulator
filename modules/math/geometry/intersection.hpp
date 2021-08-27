#pragma once
#include <vector>

namespace math {
namespace geometry {

template <typename T, unsigned D>
class SingleIntersectionData {
	// Internal parameter calculation data.
	T _this_parameter;
	T _other_parameter;
	
	// Intersection position.
	math::linear::StaticVector<T, D> _position;

public:
	explicit SingleIntersectionData(
		const T& this_position,
		const T& other_position,
		const math::linear::StaticVector<T, D>& position
	) : _this_parameter(this_position), _other_parameter(other_position), _position(position) {}

	inline const T& thisParameter() const {return _this_parameter;}
	inline const T& otherParameter() const {return _other_parameter;}

	inline const T& t() const {return _this_parameter;}
	inline const T& s() const {return _other_parameter;}

	inline const math::linear::StaticVector<T, D>& position() const {return _position;}
};


template <typename T, unsigned D>
class IntersectionData {
	std::vector<SingleIntersectionData<T,D>> _data;
	
public:
	explicit IntersectionData() : _data() {}
	explicit IntersectionData(const std::vector<SingleIntersectionData<T,D>>& data) : _data(data) {}
	explicit IntersectionData(
		const T& this_position,
		const T& other_position,
		const math::linear::StaticVector<T, D>& position
	) : _data({SingleIntersectionData<T,D>(this_position, other_position, position)}) {}

	// Verify if there's intersection.
	inline const bool intersects() const {return not _data.empty();}
	inline const bool doesIntersect() const {return not _data.empty();}
	inline const bool hasHit() const {return not _data.empty();}
	
	// Verify how many intersections.
	inline unsigned hits() const {return _data.size();}
	inline unsigned numberOfHits() const {return _data.size();}
	inline unsigned numberOfIntersections() const {return _data.size();}
	
	// Get a specific intersection data.
	inline const SingleIntersectionData<T,D>& intersection(unsigned i) const {return _data[i];}
	inline const SingleIntersectionData<T,D>& operator[](unsigned i) const {return _data[i];}
	
	// Compatibility with SingleIntersectionData public memberfunctions. Get the first.
	inline const T& thisParameter() const {return _data[0].thisParameter();}
	inline const T& otherParameter() const {return _data[0].otherParameter();}
	inline const T& t() const {return thisParameter();}
	inline const T& s() const {return otherParameter();}
	inline const math::linear::StaticVector<T, D>& position() const {return _data[0].position();}
	
	// Add an intersection.
	const IntersectionData& addIntersection(const SingleIntersectionData<T,D>& data);
	const IntersectionData& addIntersection(const IntersectionData<T,D>& data);
	
	// Clear.
	const IntersectionData& clear();
};


template <typename T, unsigned D>
const IntersectionData<T,D>& IntersectionData<T,D>::addIntersection(const SingleIntersectionData<T,D>& data) {
	_data.push_back(data);
	return *this;
}

template <typename T, unsigned D>
const IntersectionData<T,D>& IntersectionData<T,D>::addIntersection(const IntersectionData<T,D>& data) {
	for (const SingleIntersectionData<T,D>& single : data._data) _data.push_back(single);
	return *this;
}

template <typename T, unsigned D>
const IntersectionData<T,D>& IntersectionData<T,D>::clear() {
	_data.clear();
	return *this;
}

}	// geometry namespace
}	// math namespace