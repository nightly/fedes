#include "fedes/maths/vector3.h"

#include <ostream>
#include <concepts>

namespace fedes {
	/*
	 * @brief Empty constructor for Vector3 which will return a Vector3 of (0.00, 0.00, 0.00)
	 * @tparam T: the numeric type of the Vector3, i.e. double or float
	 */
	template <std::floating_point T>
	Vector3<T>::Vector3()
		: x(0.0), y(0.0), z(0.0) {}

	/*
	 * @brief Constructor for Vector3 which will take the given coordinates in the order of x, y, and z
	 * @tparam T: the numeric type of the Vector3, i.e. double or float
	 * @params x, y, z: the respective coordinates for each dimension, assigned in the listed order
	 */
	template <std::floating_point T>
	Vector3<T>::Vector3(T x, T y, T z)
		: x(x), y(y), z(z) {}

	/*
	 * @brief Scalar constructor overload for Vector3, setting an uniform value for x, y, and z.
	 * @tparam T: the numeric type of the Vector3, i.e. double or float
	 * @param scalar: the uniform value to set for each dimension, x, y, and z.
	 */
	template <std::floating_point T>
	Vector3<T>::Vector3(T scalar) 
		: x(scalar), y(scalar), z(scalar) {}

	/*
	 * @brier operator/ for scalar value
	 */
	template <std::floating_point T>
	Vector3<T> Vector3<T>::operator/(const T& scalar) const {
		return Vector3<T>(x / scalar, y / scalar, z / scalar);
	}

	/*
	 * @brief operator * for scalar value
	 */
	template <std::floating_point T>
	Vector3<T> Vector3<T>::operator*(const T& scalar) const {
		return Vector3<T>(x * scalar, y * scalar, z * scalar);
	}

	/*
	 * @brief operator /=, for scalar value
	 */
	template <std::floating_point T>
	Vector3<T>& Vector3<T>::operator/=(const T& scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	/*
	 * @brief operator + for another Vector3 
	 */
	template <std::floating_point T>
	Vector3<T> Vector3<T>::operator+(const Vector3<T>& other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	/*
     * @brief operator +=, adding the other Vector3 to the current one
	 */
	template <std::floating_point T>
	Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	/*
     * @brief operator - for another Vector3
	 */
	template <std::floating_point T>
	Vector3<T> Vector3<T>::operator-(const Vector3<T>& other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	/*
	 * @brief operator * for another Vector3 - also known as dot product
	 */
	template <std::floating_point T>
	Vector3<T> Vector3<T>::operator*(const Vector3<T>& other) const {
		return Vector3(x * other.x, y * other.y, z * other.z);
	}

	/*
	 * @brief operator ==, checking two Vector3's for equality
	 */
	template <std::floating_point T>
	bool Vector3<T>::operator==(const Vector3<T>& other) const {
		return ((x == other.x) && (y == other.y) && (z == other.z));
	}

	/*
	 * @brief Cross product 
	 */
	template <std::floating_point T>
	Vector3<T> Vector3<T>::cross(const Vector3<T>& other) const {
		return Vector3<T>(
			y * other.z - other.y * z, 
			z * other.x - other.z * x,
			x * other.y - other.x * y);
	}


	// =============================
	// Output and instantiations
	// =============================

	/*
	 * @brief Overloaded output operator 
	 */
	template <std::floating_point T>
	std::ostream& operator<<(std::ostream& os, const Vector3<T>& v) {
		return os << v.x << ", " << v.y << ", " << v.z;
	}

	/* Explicit template instantiations*/
	template struct Vector3<double>;
	template struct Vector3<float>;

	template std::ostream& operator<< <double>(std::ostream& os, const Vector3<double>& v);
	template std::ostream& operator<< <float>(std::ostream& os, const Vector3<float>& v);

}
