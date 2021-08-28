#pragma once
#include <vector>
#include <functional>
#include <cmath>
#include <initializer_list>
#include <math/linear/static_vector.hpp>
#include <math/geometry/intersection.hpp>


namespace math {
namespace geometry2 {
	
template <typename T>
class PlanarPolygonMesh {
	std::vector<math::linear::StaticVector<T, 2>> _vertices;
	std::vector<std::vector<unsigned>> _faces;

public:
	explicit PlanarPolygonMesh();
	explicit PlanarPolygonMesh(const PlanarPolygonMesh& mesh);
	explicit PlanarPolygonMesh(const std::vector<math::linear::StaticVector<T, 2>>& vertices);
	explicit PlanarPolygonMesh(const std::vector<math::linear::StaticVector<T, 2>>& vertices, const std::vector<std::vector<unsigned>>& faces);
	
	// Assembling a mesh. Returns ID of added thing.
	unsigned addVertex(const math::linear::StaticVector<T, 2>& position);
	unsigned addFace(std::vector<unsigned> face);
	
	// Information on the mesh.
	inline unsigned numberOfVertices() const {return _vertices.size();}
	inline unsigned numberOfFaces() const {return _faces.size();}
	
	// Elements of the mesh.
	const math::linear::StaticVector<T, 2>& vertex(unsigned i) const;
	SimplePolygon<T> face(unsigned i) const;
	T area() const;
};

// Constructor definitions.
template <typename T> PlanarPolygonMesh<T>::PlanarPolygonMesh() : _vertices(), _faces() {}
template <typename T> PlanarPolygonMesh<T>::PlanarPolygonMesh(const PlanarPolygonMesh& mesh) : _vertices(mesh._vertices), _faces(mesh._faces) {}
template <typename T> PlanarPolygonMesh<T>::PlanarPolygonMesh(const std::vector<math::linear::StaticVector<T, 2>>& vertices) : _vertices(vertices), _faces() {}
template <typename T> PlanarPolygonMesh<T>::PlanarPolygonMesh(const std::vector<math::linear::StaticVector<T, 2>>& vertices, const std::vector<std::vector<unsigned>>& faces) : _vertices(vertices), _faces(faces) {}

template <typename T>
unsigned PlanarPolygonMesh<T>::addVertex(const math::linear::StaticVector<T, 2>& position) {
	_vertices.push_back(position);
	return _vertices.size() - 1;
}

template <typename T>
unsigned PlanarPolygonMesh<T>::addFace(std::vector<unsigned> face) {
	_faces.push_back(face);
	return _faces.size() - 1;
}

template <typename T>
const math::linear::StaticVector<T, 2>& PlanarPolygonMesh<T>::vertex(unsigned i) const {
	return _vertices[i];
}

template <typename T>
SimplePolygon<T> PlanarPolygonMesh<T>::face(unsigned i) const {
	unsigned size = _faces[i].size();
	std::vector<math::linear::StaticVector<T, 2>> face_vertices;
	for (unsigned k = 0; k < size; ++k) face_vertices.push_back(_vertices[_faces[i][k]]);
	return SimplePolygon<T>(face_vertices);
}

template <typename T>
T PlanarPolygonMesh<T>::area() const {
	T result = T();
	unsigned size = numberOfFaces();
	for (unsigned i = 0; i < size; ++i) result += face(i).area();
	return result;
}

}
}