#pragma once
#include <math/linear/static_vector.hpp>
#include <array>
#include <cmath>
#include <initializer_list>
#include <algorithm>
#include <iostream>

/*
To think off:
Make: (i, j) == [i][j].
Same with the const values.
*/

namespace math {
namespace linear {

/// Echelon Reduction type enumeration
enum class ReductionType {
	UpperLeft, UpperRight,
	LowerLeft, LowerRight
};

template <typename T, unsigned Rows, unsigned Cols>
class StaticMatrix {
	// StaticMatrix formed by column-vectors:
	// std::array< math::linear::StaticVector<T, Rows>, Cols> _data; Is equivalent:
	
	// TODO: Delete this.
	// Make this a array of numbers, of Rows*Cols size.
	std::array<T, Rows*Cols> _data;
	static_assert(Cols*Rows > 0, "Can't make matrix with no cells");


public:
	static const unsigned size = Rows*Cols;

	StaticMatrix();
	StaticMatrix(std::initializer_list<math::linear::StaticVector<T, Cols>> list);
	StaticMatrix(std::initializer_list<T> list);

	StaticMatrix(const StaticMatrix<T, Rows, Cols>& other);
	StaticMatrix(StaticMatrix<T, Rows, Cols>& other);
	
	const T& operator[](unsigned i) const;
	T& operator[](unsigned i);

	const T& operator()(unsigned i, unsigned j) const;
	T& operator()(unsigned i, unsigned j);

	StaticMatrix operator+() const;
	StaticMatrix operator-() const;

	StaticMatrix& operator+=(const StaticMatrix& mat);
	StaticMatrix& operator-=(const StaticMatrix& mat);
	StaticMatrix& operator*=(const StaticMatrix& mat);

	StaticMatrix& operator*=(const T& value);
	StaticMatrix& operator/=(const T& value);

	static const StaticMatrix<T, Rows, Cols> zeros();
	static const StaticMatrix<T, Rows, Cols> ones();
	static const StaticMatrix<T, Rows, Cols> eye();

	math::linear::StaticVector<T, Cols> row(unsigned i) const;
	math::linear::StaticVector<T, Rows> col(unsigned j) const;

	StaticMatrix<T, Cols, Rows> transpost() const;
	
	// Reduced Row Echelon Form
	StaticMatrix<T, Rows, Cols> rref(ReductionType type) const;
	StaticMatrix<T, Rows, Cols> rref(T& value, ReductionType type) const;
	StaticMatrix<T, Rows, Cols> rref(math::linear::StaticVector<T, Rows>& value, ReductionType type) const;
	StaticMatrix<T, Rows, Cols> rref(StaticMatrix<T, Rows, Cols>& value, ReductionType type) const;

	template <typename ReductionHelper>
	StaticMatrix<T, Rows, Cols> rref(ReductionHelper&& helper, ReductionType type) const;

	// TODO: Decide!
	// 	Shall we specialize StaticMatrix class for square ones and put det and inverse?
	//	Shall we put det/inverse here?
	T det() const;
	StaticMatrix<T, Rows, Rows> inverse() const;

	// Other Functions. Swap, etc.
	void swapline(unsigned a, unsigned b);
};

// Definition of alias -----------------------------------------------------------
using StaticMatrix2 = StaticMatrix<double, 2, 2>;
using StaticMatrix3 = StaticMatrix<double, 3, 3>;
using StaticMatrix4 = StaticMatrix<double, 4, 4>;


// Declaration of functions: -----------------------------------------------------
template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator+(const StaticMatrix<T, Rows, Cols>& matrix, const StaticMatrix<T, Rows, Cols>& other);

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator-(const StaticMatrix<T, Rows, Cols>& matrix, const StaticMatrix<T, Rows, Cols>& other);

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator*(const StaticMatrix<T, Rows, Cols>& matrix, const T& value);

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator*(const T& value, const StaticMatrix<T, Rows, Cols>& matrix);

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator/(const StaticMatrix<T, Rows, Cols>& matrix, const T& value);

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator/(const T& value, const StaticMatrix<T, Rows, Cols>& matrix);

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator*(const StaticMatrix<T, Rows, Cols>& matrix, const StaticMatrix<T, Rows, Cols>& other);

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator/(const StaticMatrix<T, Rows, Cols>& matrix, const StaticMatrix<T, Rows, Cols>& other);

template <typename T, unsigned Rows, unsigned Inter, unsigned Cols>
StaticMatrix<T, Rows, Cols> matrixMultiplication(const StaticMatrix<T, Rows, Inter>& matrix, const StaticMatrix<T, Inter, Cols>& other);

template <typename T, unsigned Rows, unsigned Cols>
math::linear::StaticVector<T, Rows> matrixMultiplication(const StaticMatrix<T, Rows, Cols>& matrix, const math::linear::StaticVector<T, Cols>& vec);





// Definition of member-functions: -----------------------------------------------
template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols>::StaticMatrix() : _data{} {
	// All vectors will be constructed and initialized to T()
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols>::StaticMatrix(std::initializer_list<math::linear::StaticVector<T, Cols>> list) {
	unsigned i = 0;
	for (const math::linear::StaticVector<T, Cols>& vector : list) {
		for (unsigned j = 0; j < Cols; ++j) {
			this->operator()(i, j) = vector[j];
		}
		
		++i;
	}
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols>::StaticMatrix(std::initializer_list<T> list) {
	std::copy(list.begin(), list.end(), _data.begin());
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols>::StaticMatrix(const StaticMatrix<T, Rows, Cols>& other) : _data(other._data) {

}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols>::StaticMatrix(StaticMatrix<T, Rows, Cols>& other) : _data(other._data) {

}

template <typename T, unsigned Rows, unsigned Cols>
const T& StaticMatrix<T, Rows, Cols>::operator[](unsigned i) const {
	#ifdef DEBUG
	if (i >= size) throw std::logic_error("Invalid index");
	#endif

	return _data[i];
}

template <typename T, unsigned Rows, unsigned Cols>
T& StaticMatrix<T, Rows, Cols>::operator[](unsigned i) {
	#ifdef DEBUG
	if (i >= size) throw std::logic_error("Invalid index");
	#endif

	return _data[i];
}

template <typename T, unsigned Rows, unsigned Cols>
const T& StaticMatrix<T, Rows, Cols>::operator()(unsigned i, unsigned j) const {
	#ifdef DEBUG
	if (i >= Rows  || j >= Cols) throw std::logic_error("Invalid index");
	#endif

	return _data[i*Cols + j];
}

template <typename T, unsigned Rows, unsigned Cols>
T& StaticMatrix<T, Rows, Cols>::operator()(unsigned i, unsigned j) {
	#ifdef DEBUG
	if (i >= Rows  || j >= Cols) throw std::logic_error("Invalid index");
	#endif

	return _data[i*Cols + j];
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> StaticMatrix<T, Rows, Cols>::operator+() const {
	return *this;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> StaticMatrix<T, Rows, Cols>::operator-() const {
	StaticMatrix<T, Rows, Cols> result;
	for (unsigned i = 0; i < size; ++i) result[i] = -_data[i];
	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols>& StaticMatrix<T, Rows, Cols>::operator+=(const StaticMatrix& mat) {
	for (unsigned i = 0; i < size; ++i) _data[i] += mat[i];
	return *this;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols>& StaticMatrix<T, Rows, Cols>::operator-=(const StaticMatrix& mat) {
	for (unsigned i = 0; i < size; ++i) _data[i] -= mat[i];
	return *this;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols>& StaticMatrix<T, Rows, Cols>::operator*=(const StaticMatrix& mat) {
	for (unsigned i = 0; i < size; ++i) _data[i] *= mat[i];
	return *this;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols>& StaticMatrix<T, Rows, Cols>::operator*=(const T& value) {
	for (unsigned i = 0; i < size; ++i) _data[i] *= value;
	return *this;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols>& StaticMatrix<T, Rows, Cols>::operator/=(const T& value) {
	for (unsigned i = 0; i < size; ++i) _data[i] /= value;
	return *this;
}

template <typename T, unsigned Rows, unsigned Cols>
const StaticMatrix<T, Rows, Cols> StaticMatrix<T, Rows, Cols>::zeros() {
	return StaticMatrix<T, Rows, Cols>();
}

template <typename T, unsigned Rows, unsigned Cols>
const StaticMatrix<T, Rows, Cols> StaticMatrix<T, Rows, Cols>::ones() {
	StaticMatrix<T, Rows, Cols> result;
	for (unsigned i = 0; i < Rows; ++i) {
		for (unsigned j = 0; j < Cols; ++j) {
			result._data[j][i] = 1;
		}
	}

	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
const StaticMatrix<T, Rows, Cols> StaticMatrix<T, Rows, Cols>::eye() {
	StaticMatrix<T, Rows, Cols> result;
	for (unsigned i = 0; i < Rows; ++i) {
		for (unsigned j = 0; j < Cols; ++j) {
			if (i == j) result(i, j) = T(0);
			else result(i, j) = T(1);
		}
	}

	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
math::linear::StaticVector<T, Cols> StaticMatrix<T, Rows, Cols>::row(unsigned i) const {
	math::linear::StaticVector<T, Cols> result;
	for (unsigned j = 0; j < Cols; ++j) result(j) = this->operator()(i, j);
	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
math::linear::StaticVector<T, Rows> StaticMatrix<T, Rows, Cols>::col(unsigned j) const {
	math::linear::StaticVector<T, Rows> result;
	for (unsigned i = 0; j < Rows; ++i) result(i) = this->operator()(i, j);
	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Cols, Rows> StaticMatrix<T, Rows, Cols>::transpost() const {
	StaticMatrix<T, Cols, Rows> result;
	for (unsigned i = 0; i < Rows; ++i) {
		for (unsigned j = 0; j < Cols; ++j) {
			result(i, j) = this->operator()(j, i);
		}
	}

	return result;
}





// Definition of non-member-functions: -------------------------------------------
template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator+(const StaticMatrix<T, Rows, Cols>& matrix, const StaticMatrix<T, Rows, Cols>& other) {
	unsigned size = Rows * Cols;
	StaticMatrix<T, Rows, Cols> result;
	for (unsigned i = 0; i < size; ++i) result[i] = matrix[i] + other[i];
	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator-(const StaticMatrix<T, Rows, Cols>& matrix, const StaticMatrix<T, Rows, Cols>& other) {
	unsigned size = Rows * Cols;
	StaticMatrix<T, Rows, Cols> result;
	for (unsigned i = 0; i < size; ++i) result[i] = matrix[i] - other[i];
	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator*(const StaticMatrix<T, Rows, Cols>& matrix, const T& value) {
	unsigned size = Rows * Cols;
	StaticMatrix<T, Rows, Cols> result;
	for (unsigned i = 0; i < size; ++i) result[i] = matrix[i] * value;
	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator*(const T& value, const StaticMatrix<T, Rows, Cols>& matrix) {
	return matrix * value;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator/(const StaticMatrix<T, Rows, Cols>& matrix, const T& value) {
	unsigned size = Rows * Cols;
	StaticMatrix<T, Rows, Cols> result;
	for (unsigned i = 0; i < size; ++i) result[i] = matrix[i] / value;
	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator/(const T& value, const StaticMatrix<T, Rows, Cols>& matrix) {
	return matrix / value;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator*(const StaticMatrix<T, Rows, Cols>& matrix, const StaticMatrix<T, Rows, Cols>& other) {
	unsigned size = Rows * Cols;
	StaticMatrix<T, Rows, Cols> result;
	for (unsigned i = 0; i < size; ++i) result[i] = matrix[i] * other[i];
	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> operator/(const StaticMatrix<T, Rows, Cols>& matrix, const StaticMatrix<T, Rows, Cols>& other) {
	unsigned size = Rows * Cols;
	StaticMatrix<T, Rows, Cols> result;
	for (unsigned i = 0; i < size; ++i) result[i] = matrix[i] / other[i];
	return result;
}


template <typename T, unsigned Rows, unsigned Inter, unsigned Cols>
StaticMatrix<T, Rows, Cols> matrixMultiplication(const StaticMatrix<T, Rows, Inter>& matrix, const StaticMatrix<T, Inter, Cols>& other) {
	StaticMatrix<T, Rows, Cols> result;
	for (unsigned i = 0; i < Rows; ++i) {
		for (unsigned j = 0; j < Cols; ++j) {
			for (unsigned k = 0; k < Inter; ++k) {
				result(i, j) += matrix(i, k) * other(k, j);
			}
		}
	}

	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
math::linear::StaticVector<T, Rows> matrixMultiplication(const StaticMatrix<T, Rows, Cols>& matrix, const math::linear::StaticVector<T, Cols>& vec) {
	math::linear::StaticVector<T, Rows> result;

	for (unsigned i = 0; i < Rows; ++i) {
		for (unsigned j = 0; j < Cols; ++j) {
			result(i) += matrix(i, j) * vec(j);
		}
	}

	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
void StaticMatrix<T, Rows, Cols>::swapline(unsigned a, unsigned b) {
	// TODO: Fix me for direct accessment to _data, as soon as _data[i][j] is working.
	for (unsigned j = 0; j < Cols; ++j) std::swap(this->operator()(a, j), this->operator()(b, j));
}


template <typename T, unsigned Rows, unsigned Cols>
std::ostream& operator<<(std::ostream& os, const math::linear::StaticMatrix<T, Rows, Cols>& matrix) {
	os << "[";
	for (unsigned i = 0; i < Rows-1; ++i) {
		os << "[" << matrix(i, 0);
		for (unsigned j = 1; j < Cols; ++j) os << ", " << matrix(i, j);
		os << "], ";
	}
	
	os << "[" << matrix(Rows-1, 0);
	for (unsigned j = 1; j < Cols; ++j) os << ", " << matrix(Rows-1, j);
	os << "]";
	
	os << "]";
	return os;
}






// Internal namespace for reduction helpers
namespace internal {

template <typename T, unsigned Rows, unsigned Cols>
struct ReductionHelperWithStaticMatrix {
	StaticMatrix<T, Rows, Cols>& mat;

	ReductionHelperWithStaticMatrix(StaticMatrix<T, Rows, Cols>& s) : mat(s) {}
	inline void applyLineSwap(unsigned a, unsigned b) {
		mat.swapline(a, b);
		//for (unsigned j = 0; j < Cols; ++j) std::swap(mat(a, j), mat(b, j));
	}

	inline void applySubtractLines(unsigned a, unsigned b) {
		for (unsigned j = 0; j < Cols; ++j) mat(a, j) -= mat(b, j);
	}

	inline void applyScalar(unsigned line, const T& scalar) {
		for (unsigned j = 0; j < Cols; ++j) mat(line, j) *= scalar;
	}
};


template <typename T, unsigned D>
struct ReductionHelperWithVector {
	math::linear::StaticVector<T, D>& vec;

	ReductionHelperWithVector(math::linear::StaticVector<T, D>& s) : vec(s) {}
	inline void applyLineSwap(unsigned a, unsigned b) {std::swap(vec[a], vec[b]);}
	inline void applySubtractLines(unsigned a, unsigned b) {vec[a] -= vec[b];}
	inline void applyScalar(unsigned line, const T& scalar) {vec[line] *= scalar;}
};


template <typename T>
struct ReductionHelperWithScalar {
	T& scalar;

	ReductionHelperWithScalar(T& s) : scalar(s) {}
	inline void applyLineSwap(unsigned, unsigned) {scalar *= static_cast<T>(-1);}
	inline void applySubtractLines(unsigned, unsigned) {}
	inline void applyScalar(unsigned, const T& s) {scalar *= s;}
};

template <typename T>
struct ReductionHelperWithNothing {
	inline void applyLineSwap(unsigned, unsigned) {}
	inline void applySubtractLines(unsigned, unsigned) {}
	inline void applyScalar(unsigned, const T&) {}
};
} // internal namespace

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> StaticMatrix<T, Rows, Cols>::rref(ReductionType type) const {
	return rref(internal::ReductionHelperWithNothing<T>(), type);
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> StaticMatrix<T, Rows, Cols>::rref(T& value, ReductionType type) const {
	return rref(internal::ReductionHelperWithScalar<T>(value), type);
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> StaticMatrix<T, Rows, Cols>::rref(math::linear::StaticVector<T, Rows>& value, ReductionType type) const {
	return rref(internal::ReductionHelperWithVector<T, Rows>(value), type);
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Cols> StaticMatrix<T, Rows, Cols>::rref(StaticMatrix<T, Rows, Cols>& value, ReductionType type) const {
	return rref(internal::ReductionHelperWithStaticMatrix<T, Rows, Cols>(value), type);
}


template <typename T, unsigned Rows, unsigned Cols>
template <typename ReductionHelper>
StaticMatrix<T, Rows, Cols> StaticMatrix<T, Rows, Cols>::rref(ReductionHelper&& helper, ReductionType type) const {
	StaticMatrix<T, Rows, Cols> result(*this);
	internal::ReductionHelperWithStaticMatrix<T, Rows, Cols> mhelper(result);

	for (unsigned k = 0; k < Rows-1  or  k < Cols-1; ++k) {
		// Null pivot indicator with no chance of swap
		bool nullpivot = false;

		// Pivot matrix indexes. Row, Col.
		unsigned ip, jp;

		// Selecting pivot indexes.
		switch (type) {
			case ReductionType::LowerRight: ip=Cols-1-k;  jp=k;		 break;	// Pivot on Right-superior corner
			case ReductionType::LowerLeft:  ip=k;		 jp=k;		 break;  // Pivot on Left-superior corner
			case ReductionType::UpperLeft:  ip=k;		 jp=Rows-1-k;  break;	// Pivot on Left-inferior corner
			case ReductionType::UpperRight: ip=Cols-1-k;  jp=Rows-1-k;	break;	// Pivot on Right-inferior corner
		}

		// The pivot value
		T pivot = result(ip, jp);

		// If pivot is zero, iterate until find non zero, and swap.
		if (pivot == 0) {
			nullpivot = true;
			for (unsigned i = k+1; i < Rows; ++i) {
				// Cell matrix indexes. Row, Col.
				unsigned ic = ip;
				unsigned jc = jp;

				// Selecting cell indexes.
				switch (type) {
					case ReductionType::LowerRight: ic = i; break;
					case ReductionType::LowerLeft:  ic = i; break;
					case ReductionType::UpperLeft:  ic = Rows-i-1; break;
					case ReductionType::UpperRight: ic = Rows-i-1; break;
				}

				// The cell value
				T cell = result(ic, jc);

				// If cell is nonzero, swap.
				if (cell != 0) {
					helper.applyLineSwap(ip, ic);
					mhelper.applyLineSwap(ip, ic);
					nullpivot = false;
					break;
				}
			}
		}

		// If entire pivot line is null, move on...
		if (nullpivot) continue;
		// FIXME. (just increase the pivot-j without passing to other line).
		// But then, it won't work for determinants, and inverse, in case this happen... =(

		// Reduce the matrix
		for (unsigned i = k+1; i < Rows; ++i) {
			// Cell matrix indexes. Row, Col.
			unsigned ic = ip;
			unsigned jc = jp;

			// Select the cell indexes.
			switch (type) {
				case ReductionType::LowerRight: ic = i; break;
				case ReductionType::LowerLeft:  ic = i; break;
				case ReductionType::UpperLeft:  ic = Rows-i-1; break;
				case ReductionType::UpperRight: ic = Rows-i-1; break;
			}

			// Cell value
			T cell = result(ic, jc);

			// Already reduced. Move on...
			if (cell == 0) continue;

			// Apply reduction
			T value = pivot / cell;
			helper.applyScalar(ic, value);
			helper.applySubtractLines(ic, ip);
			mhelper.applyScalar(ic, value);
			mhelper.applySubtractLines(ic, ip);
		}
	}

	return result;
}

template <typename T, unsigned Rows, unsigned Cols>
T StaticMatrix<T, Rows, Cols>::det() const {
	static_assert(Rows == Cols, "StaticMatrix must be squared for det() to work");

	T result = 1;
	StaticMatrix reduced = this->rref(result, ReductionType::LowerLeft);

	T diag = 1;
	for (unsigned i = 0; i < Rows; ++i) diag *= reduced(i, i);
	return diag / result;
}

template <typename T, unsigned Rows, unsigned Cols>
StaticMatrix<T, Rows, Rows> StaticMatrix<T, Rows, Cols>::inverse() const {
	static_assert(Rows == Cols, "StaticMatrix must be squared for det() to work");

	StaticMatrix identity = StaticMatrix<T, Rows, Cols>::eye();
	StaticMatrix firstReduced = this->rref(identity, ReductionType::LowerLeft);
	StaticMatrix secondReduced = firstReduced.rref(identity, ReductionType::UpperRight);

	double d = det();
	if (d == 0) throw std::logic_error("This matrix is non-invertible");

	for (unsigned i = 0; i < Rows; ++i) {
		for (unsigned j = 0; j < Cols; ++j) identity(i, j) /= secondReduced(i, i);
	}

	return identity;
}

} // linear namespace
} // math namespace
