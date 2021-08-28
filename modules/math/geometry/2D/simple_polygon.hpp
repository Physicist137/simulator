#pragma once
#include <vector>
#include <functional>
#include <cmath>
#include <initializer_list>
#include <math/linear/static_vector.hpp>
#include <math/geometry/intersection.hpp>
#include <math/geometry/2D/ray_base.hpp>
#include <math/geometry/2D/line_segment.hpp>
#include <math/geometry/2D/ray.hpp>

namespace math {
namespace geometry2 {


template <typename T>
class SimplePolygon {
	std::vector<math::linear::StaticVector<T, 2>> _vertices;


protected:
	void cleanDegenerateVertexIntersection(math::geometry::IntersectionData<T,2>& data) const;

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
	math::geometry::IntersectionData<T,2> intersect(const math::geometry2::RayBase<T>& ray) const;
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
	math::linear::StaticVector<T, 2> c = center();
	unsigned size = _vertices.size();
	T result = T();
	
	for (unsigned i = 0; i < size; ++i) {
		// Centralize the coordinates.
		T cx = vertex(i).x() - c.x();
		T cy = vertex(i).y() - c.y();
		T nx = vertex(i+1).x() - c.x();
		T ny = vertex(i+1).y() - c.y();
		
		// Calculate the area contribution.
		result += cx * ny - cy * nx;
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
void SimplePolygon<T>::cleanDegenerateVertexIntersection(math::geometry::IntersectionData<T,2>& data) const {
	// Verify adjacent intersections, and check if it happens at the same location.
	std::vector<unsigned> to_erase;
	unsigned size = data.numberOfHits();
	for (unsigned i = 1; i < size; ++i) {
		if (data[i].location().vertex() - data[i-1].location().vertex() == 1) {
			if (data[i].thisParameter() < 1e-6) to_erase.push_back(i-1);
			else to_erase.push_back(i);
		}
	}
	
	// Verify first and last hits.
	if (size > 1) {
		if (data[0].location().vertex() - data[size-1].location().vertex() == 1) {
			if (data[0].thisParameter() < 1e-6) to_erase.push_back(size-1);
			else to_erase.push_back(0);
		}
	}
	
	// Sort, reverse, and delete entries.
	if (not to_erase.empty()) {
		std::sort(to_erase.begin(), to_erase.end(), std::greater<unsigned>());
		for (unsigned index : to_erase) data.deleteByIndex(index);
	}
}

template <typename T>
math::geometry::IntersectionData<T,2> SimplePolygon<T>::intersect(const math::geometry2::RayBase<T>& ray) const {
	unsigned size = _vertices.size();
	math::geometry::IntersectionData<T,2> data;
	
	// Calculate intersections.
	for (unsigned i = 0; i < size; ++i) {
		math::geometry::IntersectionLocation location(i);
		auto inter = ray.intersect(edge(i), location);
		inter.swap();
		if (inter.hasHit()) data.addIntersection(inter);
	}
	
	// Degenerate vertex intersection.
	this->cleanDegenerateVertexIntersection(data);

	// Return data.
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
