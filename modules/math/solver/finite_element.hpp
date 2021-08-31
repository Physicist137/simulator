#pragma once


namespace math {
namespace solver {

template <typename T>
class FiniteElement {
	// Value of the FiniteElement.
	T _value;
	
	// Other data.
	bool _frozen;
	bool _boundary;

public:
	FiniteElement(const T& value = T(), bool frozen = false) : _value(value), _frozen(false) {}
	
	// Accessor functions.
	inline const T& value() const {return _value;}
	inline bool frozen() const {return _frozen;}
	inline bool boundary() const {return _boundary;}
	
	// FiniteElement operators.
	FiniteElement& operator=(const FiniteElement& other);
	FiniteElement& operator+=(const FiniteElement& other);
	FiniteElement& operator-=(const FiniteElement& other);
	FiniteElement& operator*=(const FiniteElement& other);
	FiniteElement& operator/=(const FiniteElement& other);
	
	FiniteElement& operator=(const T& other);
	FiniteElement& operator+=(const T& other);
	FiniteElement& operator-=(const T& other);
	FiniteElement& operator*=(const T& other);
	FiniteElement& operator/=(const T& other);
};

// Declaration of operations.
template <typename T> FiniteElement<T> operator+(const FiniteElement<T>& a);
template <typename T> FiniteElement<T> operator-(const FiniteElement<T>& a);
template <typename T> bool operator==(const FiniteElement<T>& a, const FiniteElement<T>& b);
template <typename T> bool operator!=(const FiniteElement<T>& a, const FiniteElement<T>& b);
template <typename T> FiniteElement<T> operator+(const FiniteElement<T>& a, const FiniteElement<T>& b);
template <typename T> FiniteElement<T> operator-(const FiniteElement<T>& a, const FiniteElement<T>& b);
template <typename T> FiniteElement<T> operator*(const FiniteElement<T>& a, const FiniteElement<T>& b);
template <typename T> FiniteElement<T> operator/(const FiniteElement<T>& a, const FiniteElement<T>& b);



// Definition of operator member functions.
template <typename T>
FiniteElement<T>& FiniteElement<T>::operator=(const FiniteElement<T>& other) {
	this->_value = other._value;
	return *this;
}

template <typename T>
FiniteElement<T>& FiniteElement<T>::operator+=(const FiniteElement<T>& other) {
	this->_value += other._value;
	return *this;
}

template <typename T>
FiniteElement<T>& FiniteElement<T>::operator-=(const FiniteElement<T>& other) {
	this->_value -= other._value;
	return *this;
}

template <typename T>
FiniteElement<T>& FiniteElement<T>::operator*=(const FiniteElement<T>& other) {
	this->_value *= other._value;
	return *this;
}

template <typename T>
FiniteElement<T>& FiniteElement<T>::operator/=(const FiniteElement<T>& other) {
	this->_value /= other._value;
	return *this;
}

template <typename T>
FiniteElement<T>& FiniteElement<T>::operator=(const T& other) {
	this->_value = other;
	return *this;
}

template <typename T>
FiniteElement<T>& FiniteElement<T>::operator+=(const T& other) {
	this->_value += other;
	return *this;
}

template <typename T>
FiniteElement<T>& FiniteElement<T>::operator-=(const T& other) {
	this->_value -= other;
	return *this;
}

template <typename T>
FiniteElement<T>& FiniteElement<T>::operator*=(const T& other) {
	this->_value *= other;
	return *this;
}

template <typename T>
FiniteElement<T>& FiniteElement<T>::operator/=(const T& other) {
	this->_value /= other;
	return *this;
}





// Definition of non-member operators.
template <typename T>
bool operator==(const FiniteElement<T>& a, const FiniteElement<T>& b) {
	return (a.value() == b.value());
}

template <typename T>
bool operator!=(const FiniteElement<T>& a, const FiniteElement<T>& b) {
	return (a.value() != b.value());
}

template <typename T>
FiniteElement<T> operator+(const FiniteElement<T>& a) {
	return FiniteElement<T>(+a.value(), a.frozen(), a.boundary());
}

template <typename T>
FiniteElement<T> operator-(const FiniteElement<T>& a) {
	return FiniteElement<T>(-a.value(), a.frozen(), a.boundary());
}

template <typename T>
FiniteElement<T> operator+(const FiniteElement<T>& a, const FiniteElement<T>& b) {
	return FiniteElement<T>(
		a.value() + b.value(),
		a.frozen() and b.frozen(),
		a.boundary() and b.boundary()
	);
}

template <typename T>
FiniteElement<T> operator-(const FiniteElement<T>& a, const FiniteElement<T>& b) {
	return FiniteElement<T>(
		a.value() - b.value(),
		a.frozen() and b.frozen(),
		a.boundary() and b.boundary()
	);
}

template <typename T>
FiniteElement<T> operator*(const FiniteElement<T>& a, const FiniteElement<T>& b) {
	return FiniteElement<T>(
		a.value() * b.value(),
		a.frozen() and b.frozen(),
		a.boundary() and b.boundary()
	);
}

template <typename T>
FiniteElement<T> operator/(const FiniteElement<T>& a, const FiniteElement<T>& b) {
	return FiniteElement<T>(
		a.value() / b.value(),
		a.frozen() and b.frozen(),
		a.boundary() and b.boundary()
	);
}

}	// Namespace solver.
}	// Namespace math.
