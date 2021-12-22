#include "fedes/data/vector3.hpp"
namespace fedes {
	template<typename T>
	Vector3<T>::Vector3(T x, T y, T z)
		: x(x), y(y), z(z) {}

	template<typename T>
	Vector3<T>::Vector3()
		: x(0.0), y(0.0), z(0.0) {}

	template<typename T>
	Vector3<T> Vector3<T>::operator/(const T& scalar) const {
		return Vector3<T>(x / scalar, y / scalar, z / scalar);
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator*(const T& scalar) const {
		return Vector3<T>(x * scalar, y * scalar, z * scalar);
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	template<typename T>
	bool Vector3<T>::operator==(const Vector3<T>& other) const {
		return ((x == other.x) && (y == other.y) && (z == other.z)) ? true : false;
	}

	template struct Vector3<double>;
	template struct Vector3<float>;
}