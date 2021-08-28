#pragma once
#include <vector>
#include <functional>
#include <cmath>
#include <initializer_list>
#include <math/linear/static_vector.hpp>
#include <math/geometry/intersection.hpp>
#include <math/geometry/2D/ray.hpp>
#include <math/geometry/2D/line_segment.hpp>

namespace math {
namespace geometry2 {


template <typename T>
class SimplePolygon {
	std::vector<math::linear::StaticVector<T, 2>> _vertices;

public:
	// Constructor.
	SimplePolygon();
	SimplePolygon(std::initializer_list<math::linear::StaticVector<T, 2>> list);
	SimplePolygon(const std::vector<math::linear::StaticVector<T, 2>>& vec);
	SimplePolygon(const SimplePolygon<T>& other);
	SimplePolygon(SimplePolygon<T>& other);

	// Return special vertices.
	inline const math::linear::StaticVector<T, 2>& start() const {return _vertices.front();}
	inline const math::linear::StaticVector<T, 2>& end() const {return _vertices.back();}
	
	// Return polygon featurs.
	const math::linear::StaticVector<T, 2>& vertex(unsigned i) const {return _vertices[i % _vertices.size()];}
	inline const math::linear::StaticVector<T, 2>& operator[](unsigned i) const {return vertex(i);}
	math::geometry2::LineSegment<T> edge(unsigned i) const;
	
	// Polygon information
	inline unsigned numberOfEdges() const {return _vertices.size();}
	inline unsigned numberOfVertices() const {return _vertices.size();}
	
	// Position information.
	math::linear::StaticVector<T, 2> center() const;
	
	// Geometrical information.
	T perimeter(const std::function<T(T)>& sqrt = [](auto const& x) {return std::sqrt(x);}) const;
	T signedArea() const;
	T area() const;
	
	// Intersection operations.
	math::geometry::IntersectionData<T,2> intersect(const math::geometry2::Ray<T>& ray) const;
	math::geometry::IntersectionData<T,2> intersect(const math::geometry2::LineSegment<T>& line) const;
	bool isInside(const math::linear::StaticVector<T, 2>& point) const;
	
	// Add vertices.
	const SimplePolygon& addVertex(const math::linear::StaticVector<T, 2>& vertex);
};


// Constructor functions.
template <typename T> SimplePolygon<T>::SimplePolygon() : _vertices() {}
template <typename T> SimplePolygon<T>::SimplePolygon(const std::vector<math::linear::StaticVector<T, 2>>& vec) : _vertices(vec) {}
template <typename T> SimplePolygon<T>::SimplePolygon(const SimplePolygon<T>& other) : _vertices(other._vertices) {}
template <typename T> SimplePolygon<T>::SimplePolygon(SimplePolygon<T>& other) : _vertices(other._vertices) {}
template <typename T>
SimplePolygon<T>::SimplePolygon(std::initializer_list<math::linear::StaticVector<T, 2>> list) {
	std::copy(list.begin(), list.end(), _vertices.begin());
}



template <typename T>
const SimplePolygon<T>& SimplePolygon<T>::addVertex(const math::linear::StaticVector<T, 2>& vertex) {
	_vertices.push_back(vertex);
	return *this;
}

template <typename T>
math::geometry2::LineSegment<T> SimplePolygon<T>::edge(unsigned i) const {
	return math::geometry2::LineSegment<T>(this->vertex(i), this->vertex(i+1));
}

template <typename T>
math::linear::StaticVector<T, 2> SimplePolygon<T>::center() const {
	math::linear::StaticVector<T, 2> result;
	T num = static_cast<T>(numberOfVertices());
	for (const auto& vertex : _vertices) result += vertex / num;
	return result;
}

template <typename T>
T SimplePolygon<T>::perimeter(const std::function<T(T)>& sqrt) const {
	unsigned size = _vertices.size();
	T result = T();
	
	for (unsigned i = 1; i < size; ++i) {
		auto displacement = _vertices[i] - _vertices[i-1];
		result += sqrt(displacement.dot());
	}
	
	auto displacement = _vertices[0] - _vertices[size-1];
	result += sqrt(displacement.dot());
	
	return result;
}

template <typename T>
T SimplePolygon<T>::signedArea() const {
	unsigned size = _vertices.size();
	T result = T();
	
	for (unsigned i = 0; i <= size; ++i) {
		T sum = (*this)[i].x() * (*this)[i+1].y();
		T sub = (*this)[i+1].x() * (*this)[i].y();
		result += sum - sub;
	}
	
	return result / 2.0;
}

template <typename T>
T SimplePolygon<T>::area() const {
	T signed_area = signedArea();
	if (signed_area >= 0) return signed_area;
	else return -signed_area;
}

template <typename T>
math::geometry::IntersectionData<T,2> SimplePolygon<T>::intersect(const math::geometry2::Ray<T>& ray) const {
	unsigned size = _vertices.size();
	math::geometry::IntersectionData<T,2> data;
	for (unsigned i = 0; i < size; ++i) {
		auto inter = ray.intersect(edge(i));
		if (inter.hasHit()) data.addIntersection(inter);
	}
	
	return data;
}

template <typename T>
math::geometry::IntersectionData<T,2> SimplePolygon<T>::intersect(const math::geometry2::LineSegment<T>& line) const {
	unsigned size = _vertices.size();
	math::geometry::IntersectionData<T,2> data;
	for (unsigned i = 0; i < size; ++i) {
		auto inter = line.intersect(edge(i));
		if (inter.hasHit()) data.addIntersection(inter);
	}
	
	return data;
}

template <typename T>
bool SimplePolygon<T>::isInside(const math::linear::StaticVector<T, 2>& point) const {
	// Get some vectors.
	math::linear::StaticVector<T, 2> center = this->center();
	math::linear::StaticVector<T, 2> diff = point - center;
	
	// If point in question happens to be too near the center, just choose a random direction.
	if (diff.dot() < 1e-4) diff = math::linear::StaticVector<T, 2>({0.6570, 0.5480});
		
	// Directed ray connecting point in question to the center of the polygon.
	math::geometry2::Ray<T> ray(point, diff);
	
	// Run intersections.
	auto inter = this->intersect(ray);
	
	// Decide if inside or outside.
	if (inter.numberOfHits() % 2) return true;
	else return false;
}



}
}