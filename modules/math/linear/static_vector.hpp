#pragma once
#include <array>
#include <stdexcept>
#include <cmath>
#include <initializer_list>
#include <algorithm>
#include <ostream>

namespace math {
namespace linear {
template <typename T, unsigned D>
class StaticVector {
	std::array<T, D> _data;
	static_assert(D > 0, "Can't make a vector with no dimentions");

public:
	StaticVector();
	StaticVector(std::initializer_list<T> list);

	StaticVector(const std::array<T, D>& array);
	StaticVector(const StaticVector<T, D>& other);
	StaticVector(StaticVector<T, D>& other);

	const T& operator[](unsigned i) const;
	T& operator[](unsigned i);

	const T& operator()(unsigned i) const;
	T& operator()(unsigned i);

	T x() const;
	T y() const;
	T z() const;
	T w() const;

	StaticVector operator+() const;
	StaticVector operator-() const;

	StaticVector& operator+=(const StaticVector& vec);
	StaticVector& operator-=(const StaticVector& vec);
	StaticVector& operator*=(const StaticVector& vec);
	StaticVector& operator/=(const StaticVector& vec);

	StaticVector& operator*=(const T& value);
	StaticVector& operator/=(const T& value);

	bool operator==(const StaticVector& vec);
	bool operator!=(const StaticVector& vec);

	StaticVector cross(const StaticVector& value) const;
	StaticVector unit() const;

	T dot(const StaticVector& vec) const;
	T dot() const;

	template <typename E=T>
	E length() const;
};

// Definition of alias -----------------------------------------------------------
using StaticVector2 = StaticVector<double, 2>;
using StaticVector3 = StaticVector<double, 3>;
using StaticVector4 = StaticVector<double, 4>;
using Vector2 = StaticVector<double, 2>;
using Vector3 = StaticVector<double, 3>;
using Vector4 = StaticVector<double, 4>;

// Declaration of functions: -----------------------------------------------------
template <typename T, unsigned D> StaticVector<T, D> operator+(const StaticVector<T, D>& vec, const StaticVector<T, D>& other);
template <typename T, unsigned D> StaticVector<T, D> operator-(const StaticVector<T, D>& vec, const StaticVector<T, D>& other);
template <typename T, unsigned D> StaticVector<T, D> operator*(const StaticVector<T, D>& vec, const StaticVector<T, D>& other);
template <typename T, unsigned D> StaticVector<T, D> operator/(const StaticVector<T, D>& vec, const StaticVector<T, D>& other);

template <typename T, unsigned D> StaticVector<T, D> operator*(const StaticVector<T, D>& vec, const T& value);
template <typename T, unsigned D> StaticVector<T, D> operator*(const T& value, const StaticVector<T, D>& vec);
template <typename T, unsigned D> StaticVector<T, D> operator/(const StaticVector<T, D>& vec, const T& value);
template <typename T, unsigned D> StaticVector<T, D> operator/(const T& value, const StaticVector<T, D>& vec);

template <typename T, unsigned D> std::ostream& operator<<(std::ostream& os, const math::linear::StaticVector<T, D>& vec);

// Definition of memberfunctions: ------------------------------------------------
template <typename T, unsigned D>
inline StaticVector<T, D>::StaticVector() {
	for (unsigned i = 0; i < D; ++i) _data[i] = T(0);
}

template <typename T, unsigned D>
inline StaticVector<T, D>::StaticVector(std::initializer_list<T> list) {
	std::copy(list.begin(), list.end(), _data.begin());
}


template <typename T, unsigned D>
inline StaticVector<T, D>::StaticVector(const std::array<T, D>& array) : _data(array) {

}

template <typename T, unsigned D>
inline StaticVector<T, D>::StaticVector(const StaticVector<T, D>& other) : _data(other._data) {

}

template <typename T, unsigned D>
inline StaticVector<T, D>::StaticVector(StaticVector<T, D>& other) : _data(other._data) {

}

template <typename T, unsigned D>
T StaticVector<T, D>::x() const {
	static_assert(D>=1, "The x-component of this vector does not exist");
	return _data[0];
}

template <typename T, unsigned D>
T StaticVector<T, D>::y() const {
	static_assert(D>=2, "The y-component of this vector does not exist");
	return _data[1];
}

template <typename T, unsigned D>
T StaticVector<T, D>::z() const {
	static_assert(D>=3, "The z-component of this vector does not exist");
	return _data[2];
}

template <typename T, unsigned D>
T StaticVector<T, D>::w() const {
	static_assert(D>=4, "The w-component of this vector does not exist");
	return _data[3];
}

template <typename T, unsigned D>
inline const T& StaticVector<T, D>::operator[](unsigned i) const {
	#ifdef DEBUG
		if (i >= D) throw std::logic_error("Invalid index");
	#endif

	return _data[i];
}

template <typename T, unsigned D>
inline T& StaticVector<T, D>::operator[](unsigned i) {
	#ifdef DEBUG
		if (i >= D) throw std::logic_error("Invalid index");
	#endif

	return _data[i];
}

template <typename T, unsigned D>
inline const T& StaticVector<T, D>::operator()(unsigned i) const {
	#ifdef DEBUG
		if (i >= D) throw std::logic_error("Invalid index");
	#endif
	return this->operator[](i);
}

template <typename T, unsigned D>
inline T& StaticVector<T, D>::operator()(unsigned i) {
	#ifdef DEBUG
		if (i >= D) throw std::logic_error("Invalid index");
	#endif
	return this->operator[](i);
}

template <typename T, unsigned D>
inline StaticVector<T, D> StaticVector<T, D>::operator+() const {
	return *this;
}

template <typename T, unsigned D>
inline StaticVector<T, D> StaticVector<T, D>::operator-() const {
	StaticVector vec;
	for (unsigned i = 0; i < D; ++i) vec[i] = -_data[i];
	return vec;
}

template <typename T, unsigned D>
inline StaticVector<T, D>& StaticVector<T, D>::operator+=(const StaticVector<T, D>& vec) {
	for (unsigned i = 0; i < D; ++i) _data[i] += vec[i];
	return *this;
}

template <typename T, unsigned D>
inline StaticVector<T, D>& StaticVector<T, D>::operator-=(const StaticVector<T, D>& vec) {
	for (unsigned i = 0; i < D; ++i) _data[i] -= vec[i];
	return *this;
}

template <typename T, unsigned D>
inline StaticVector<T, D>& StaticVector<T, D>::operator*=(const StaticVector<T, D>& vec) {
	for (unsigned i = 0; i < D; ++i) _data[i] *= vec[i];
	return *this;
}

template <typename T, unsigned D>
inline StaticVector<T, D>& StaticVector<T, D>::operator/=(const StaticVector<T, D>& vec) {
	#ifdef DEBUG
		for (unsigned i = 0; i < D; ++i)
			if (vec._data[i] == 0)
				throw std::logic_error("Can't divide by zero");
	#endif

	for (unsigned i = 0; i < D; ++i) _data[i] /= vec[i];
	return *this;
}

template <typename T, unsigned D>
inline StaticVector<T, D>& StaticVector<T, D>::operator*=(const T& value) {
	for (unsigned i = 0; i < D; ++i) _data[i] *= value;
	return *this;
}

template <typename T, unsigned D>
inline StaticVector<T, D>& StaticVector<T, D>::operator/=(const T& value) {
	#ifdef DEBUG
		if (value == 0) throw std::logic_error("Can't divide by zero");
	#endif

	for (unsigned i = 0; i < D; ++i) _data[i] /= value;
	return *this;
}

template <typename T, unsigned D>
bool StaticVector<T, D>::operator==(const StaticVector<T, D>& vec) {
	for (unsigned i = 0; i < D; ++i) {
		if (_data[i] != vec[i]) return false;
	}

	return true;
}

template <typename T, unsigned D>
bool StaticVector<T, D>::operator!=(const StaticVector<T, D>& vec) {
	return !(*this == vec);
}

template <typename T, unsigned D>
inline StaticVector<T, D> StaticVector<T, D>::cross(const StaticVector<T, D>& value) const {
	static_assert(D >= 3, "Cross product only for dimension 3 (for now anyway..)");

	StaticVector<T, D> result;
	result(0) = _data[1] * value._data[2] - _data[2] * value._data[1];
	result(1) = _data[2] * value._data[0] - _data[0] * value._data[2];
	result(2) = _data[0] * value._data[1] - _data[1] * value._data[0];
	
	return result;
}

template <typename T, unsigned D>
inline StaticVector<T, D> StaticVector<T, D>::unit() const {
	return *this / std::sqrt(this->dot());
}

template <typename T, unsigned D>
inline T StaticVector<T, D>::dot(const StaticVector<T, D>& vec) const {
	T result = T(0);
	for (unsigned i = 0; i < D; ++i) result += _data[i] * vec[i];
	return result;
}

template <typename T, unsigned D>
inline T StaticVector<T, D>::dot() const {
	T result = T(0);
	for (unsigned i = 0; i < D; ++i) result += _data[i] * _data[i];
	return result;
}

template <typename T, unsigned D>
template <typename E>
inline E StaticVector<T, D>::length() const {
	return std::sqrt(static_cast<E>(dot()));
}


// Definition of non-memberfunctions: --------------------------------------------
template <typename T, unsigned D>
StaticVector<T, D> operator+(const StaticVector<T, D>& vec, const StaticVector<T, D>& other) {
	StaticVector<T, D> result;
	for (unsigned i = 0; i < D; ++i) result[i] = vec[i] + other[i];
	return result;
}

template <typename T, unsigned D>
StaticVector<T, D> operator-(const StaticVector<T, D>& vec, const StaticVector<T, D>& other) {
	StaticVector<T, D> result;
	for (unsigned i = 0; i < D; ++i) result[i] = vec[i] - other[i];
	return result;
}

template <typename T, unsigned D>
StaticVector<T, D> operator*(const StaticVector<T, D>& vec, const StaticVector<T, D>& other) {
	StaticVector<T, D> result;
	for (unsigned i = 0; i < D; ++i) result[i] = vec[i] * other[i];
	return result;
}

template <typename T, unsigned D>
StaticVector<T, D> operator/(const StaticVector<T, D>& vec, const StaticVector<T, D>& other) {
	#ifdef DEBUG
		for (unsigned i = 0; i < D; ++i)
			if (other._data[i] == 0)
				throw std::logic_error("Can't divide by zero");
	#endif

	StaticVector<T, D> result;
	for (unsigned i = 0; i < D; ++i) result[i] = vec[i] / other[i];
	return result;
}

template <typename T, unsigned D>
inline StaticVector<T, D> operator*(const StaticVector<T, D>& vec, const T& value) {
	StaticVector<T, D> result;
	for (unsigned i = 0; i < D; ++i) result[i] = vec[i] * value;
	return result;
}

template <typename T, unsigned D>
inline StaticVector<T, D> operator*(const T& value, const StaticVector<T, D>& vec) {
	return vec * value;
}

template <typename T, unsigned D>
inline StaticVector<T, D> operator/(const StaticVector<T, D>& vec, const T& value) {
	StaticVector<T, D> result;
	for (unsigned i = 0; i < D; ++i) result[i] = vec[i] / value;
	return result;
}

template <typename T, unsigned D>
inline StaticVector<T, D> operator/(const T& value, const StaticVector<T, D>& vec) {
	return vec / value;
}

template <typename T, unsigned D>
std::ostream& operator<<(std::ostream& os, const math::linear::StaticVector<T, D>& vec) {
	os << "[" << vec.x();
	for (unsigned i = 1; i < D; ++i) os << ", " << vec[i];
	os << "]";

	return os;
}

} // linear namespace
} // math namespace
