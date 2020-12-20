#pragma once
#include <vector>
#include <stdexcept>
#include <cmath>
#include <initializer_list>
#include <algorithm>
#include <ostream>

namespace math {
namespace linear {
	

template <typename T>
class DynamicVector {
	std::vector<T> _data;

public:
	DynamicVector();
	DynamicVector(unsigned size);
	
	DynamicVector(std::initializer_list<T> list);

	DynamicVector(const std::vector<T>& array);
	DynamicVector(const DynamicVector<T>& other);
	DynamicVector(DynamicVector<T>& other);
	
	unsigned size() const;
	
	DynamicVector& resize(unsigned size);

	const T& operator[](unsigned i) const;
	T& operator[](unsigned i);

	const T& operator()(unsigned i) const;
	T& operator()(unsigned i);

	DynamicVector operator+() const;
	DynamicVector operator-() const;

	DynamicVector& operator+=(const DynamicVector& vec);
	DynamicVector& operator-=(const DynamicVector& vec);
	DynamicVector& operator*=(const DynamicVector& vec);
	DynamicVector& operator/=(const DynamicVector& vec);

	DynamicVector& operator*=(const T& value);
	DynamicVector& operator/=(const T& value);

	bool operator==(const DynamicVector& vec);
	bool operator!=(const DynamicVector& vec);

	DynamicVector unit() const;

	T dot(const DynamicVector& vec) const;
	T dot() const;

	template <typename E=T>
	E length() const;
};


// Declaration of functions: -----------------------------------------------------
template <typename T> DynamicVector<T> operator+(const DynamicVector<T>& vec, const DynamicVector<T>& other);
template <typename T> DynamicVector<T> operator-(const DynamicVector<T>& vec, const DynamicVector<T>& other);
template <typename T> DynamicVector<T> operator*(const DynamicVector<T>& vec, const DynamicVector<T>& other);
template <typename T> DynamicVector<T> operator/(const DynamicVector<T>& vec, const DynamicVector<T>& other);

template <typename T> DynamicVector<T> operator*(const DynamicVector<T>& vec, const T& value);
template <typename T> DynamicVector<T> operator*(const T& value, const DynamicVector<T>& vec);
template <typename T> DynamicVector<T> operator/(const DynamicVector<T>& vec, const T& value);
template <typename T> DynamicVector<T> operator/(const T& value, const DynamicVector<T>& vec);

template <typename T> std::ostream& operator<<(std::ostream& os, const math::linear::DynamicVector<T>& vec);

// Definition of memberfunctions: ------------------------------------------------
template <typename T>
inline DynamicVector<T>::DynamicVector() : _data{} {

}

template <typename T>
inline DynamicVector<T>::DynamicVector(unsigned size) : _data(size) {
	// _data.resize(size);
}

template <typename T>
inline DynamicVector<T>::DynamicVector(std::initializer_list<T> list) {
	_data.resize(list.size());
	std::copy(list.begin(), list.end(), _data.begin());
}


template <typename T>
inline DynamicVector<T>::DynamicVector(const std::vector<T>& array) : _data(array) {

}

template <typename T>
inline DynamicVector<T>::DynamicVector(const DynamicVector<T>& other) : _data(other._data) {

}

template <typename T>
inline DynamicVector<T>::DynamicVector(DynamicVector<T>& other) : _data(other._data) {

}

template <typename T>
unsigned DynamicVector<T>::size() const {
	return _data.size();
}

template <typename T>
DynamicVector<T>& DynamicVector<T>::resize(unsigned size) {
	_data.resize(size);
	return *this;
}

template <typename T>
inline const T& DynamicVector<T>::operator[](unsigned i) const {
	#ifdef DEBUG
		if (i >= _data.size()) throw std::logic_error("Invalid index");
	#endif

	return _data[i];
}

template <typename T>
inline T& DynamicVector<T>::operator[](unsigned i) {
	#ifdef DEBUG
		if (i >= _data.size()) throw std::logic_error("Invalid index");
	#endif

	return _data[i];
}

template <typename T>
inline const T& DynamicVector<T>::operator()(unsigned i) const {
	#ifdef DEBUG
		if (i >= _data.size()) throw std::logic_error("Invalid index");
	#endif
	return this->operator[](i);
}

template <typename T>
inline T& DynamicVector<T>::operator()(unsigned i) {
	#ifdef DEBUG
		if (i >= _data.size()) throw std::logic_error("Invalid index");
	#endif
	return this->operator[](i);
}

template <typename T>
inline DynamicVector<T> DynamicVector<T>::operator+() const {
	return *this;
}

template <typename T>
inline DynamicVector<T> DynamicVector<T>::operator-() const {
	unsigned size = _data.size();
	DynamicVector vec(_data);
	for (unsigned i = 0; i < size; ++i) vec[i] = -_data[i];
	return vec;
}

template <typename T>
inline DynamicVector<T>& DynamicVector<T>::operator+=(const DynamicVector<T>& vec) {
	unsigned size = _data.size();
	if (vec.size() != size) throw std::logic_error("Vectors have to be of the same shape");
	for (unsigned i = 0; i < size; ++i) _data[i] += vec[i];
	return *this;
}

template <typename T>
inline DynamicVector<T>& DynamicVector<T>::operator-=(const DynamicVector<T>& vec) {
	unsigned size = _data.size();
	if (vec.size() != size) throw std::logic_error("Vectors have to be of the same shape");
	for (unsigned i = 0; i < size; ++i) _data[i] -= vec[i];
	return *this;
}

template <typename T>
inline DynamicVector<T>& DynamicVector<T>::operator*=(const DynamicVector<T>& vec) {
	unsigned size = _data.size();
	if (vec.size() != size) throw std::logic_error("Vectors have to be of the same shape");
	for (unsigned i = 0; i < size; ++i) _data[i] *= vec[i];
	return *this;
}

template <typename T>
inline DynamicVector<T>& DynamicVector<T>::operator/=(const DynamicVector<T>& vec) {
	unsigned size = _data.size();
	if (vec.size() != size) throw std::logic_error("Vectors have to be of the same shape");
	
	#ifdef DEBUG
	for (unsigned i = 0; i < size; ++i)
		if (vec._data[i] == 0)
			throw std::logic_error("Can't divide by zero");
	#endif

	for (unsigned i = 0; i < size; ++i) _data[i] /= vec[i];
	return *this;
}

template <typename T>
inline DynamicVector<T>& DynamicVector<T>::operator*=(const T& value) {
	unsigned size = _data.size();
	for (unsigned i = 0; i < size; ++i) _data[i] *= value;
	return *this;
}

template <typename T>
inline DynamicVector<T>& DynamicVector<T>::operator/=(const T& value) {
	#ifdef DEBUG
		if (value == 0) throw std::logic_error("Can't divide by zero");
	#endif

	unsigned size = _data.size();
	for (unsigned i = 0; i < size; ++i) _data[i] /= value;
	return *this;
}

template <typename T>
bool DynamicVector<T>::operator==(const DynamicVector<T>& vec) {
	unsigned size = _data.size();
	for (unsigned i = 0; i < size; ++i) {
		if (_data[i] != vec[i]) return false;
	}

	return true;
}

template <typename T>
bool DynamicVector<T>::operator!=(const DynamicVector<T>& vec) {
	return !(*this == vec);
}

template <typename T>
inline DynamicVector<T> DynamicVector<T>::unit() const {
	return *this / std::sqrt(this->dot());
}

template <typename T>
inline T DynamicVector<T>::dot(const DynamicVector<T>& vec) const {
	T result = T(0);
	unsigned size = _data.size();
	if (vec.size() != size) throw std::logic_error("Vectors have to be of the same shape");
	for (unsigned i = 0; i < size; ++i) result += _data[i] * vec[i];
	return result;
}

template <typename T>
inline T DynamicVector<T>::dot() const {
	unsigned size = _data.size();
	T result = T(0);
	for (unsigned i = 0; i < size; ++i) result += _data[i] * _data[i];
	return result;
}

template <typename T>
template <typename E>
inline E DynamicVector<T>::length() const {
	return std::sqrt(static_cast<E>(dot()));
}


// Definition of non-memberfunctions: --------------------------------------------
template <typename T>
DynamicVector<T> operator+(const DynamicVector<T>& vec, const DynamicVector<T>& other) {
	unsigned size = vec.size();
	if (other.size() != vec.size()) throw std::logic_error("Vectors have to be of the same shape");
	
	DynamicVector<T> result(size);
	for (unsigned i = 0; i < size; ++i) result[i] = vec[i] + other[i];
	return result;
}

template <typename T>
DynamicVector<T> operator-(const DynamicVector<T>& vec, const DynamicVector<T>& other) {
	unsigned size = vec.size();
	if (other.size() != vec.size()) throw std::logic_error("Vectors have to be of the same shape");
	
	DynamicVector<T> result(size);
	for (unsigned i = 0; i < size; ++i) result[i] = vec[i] - other[i];
	return result;
}

template <typename T>
DynamicVector<T> operator*(const DynamicVector<T>& vec, const DynamicVector<T>& other) {
	unsigned size = vec.size();
	if (other.size() != vec.size()) throw std::logic_error("Vectors have to be of the same shape");
	
	DynamicVector<T> result(size);
	for (unsigned i = 0; i < size; ++i) result[i] = vec[i] * other[i];
	return result;
}

template <typename T>
DynamicVector<T> operator/(const DynamicVector<T>& vec, const DynamicVector<T>& other) {
	#ifdef DEBUG
		for (unsigned i = 0; i < D; ++i)
			if (other._data[i] == 0)
				throw std::logic_error("Can't divide by zero");
	#endif

	unsigned size = vec.size();
	if (other.size() != vec.size()) throw std::logic_error("Vectors have to be of the same shape");
	
	DynamicVector<T> result(size);
	for (unsigned i = 0; i < size; ++i) result[i] = vec[i] / other[i];
	return result;
}

template <typename T>
inline DynamicVector<T> operator*(const DynamicVector<T>& vec, const T& value) {
	unsigned size = vec.size();
	DynamicVector<T> result(size);
	for (unsigned i = 0; i < size; ++i) result[i] = vec[i] * value;
	return result;
}

template <typename T>
inline DynamicVector<T> operator*(const T& value, const DynamicVector<T>& vec) {
	return vec * value;
}

template <typename T>
inline DynamicVector<T> operator/(const DynamicVector<T>& vec, const T& value) {
	unsigned size = vec.size();
	DynamicVector<T> result(size);
	for (unsigned i = 0; i < size; ++i) result[i] = vec[i] / value;
	return result;
}

template <typename T>
inline DynamicVector<T> operator/(const T& value, const DynamicVector<T>& vec) {
	return vec / value;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const math::linear::DynamicVector<T>& vec) {
	os << "(" << vec[0];
	unsigned size = vec.size();
	for (unsigned i = 1; i < size; ++i) os << ", " << vec[i];
	os << ")";

	return os;
}

} // linear namespace
} // math namespace
