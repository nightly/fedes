#include "fedes/maths/vector3.hpp"

#include <ostream>

namespace fedes {
	/*
	 * @brief Empty constructor for Vector3 which will return a Vector3 of (0.00, 0.00, 0.00)
	 * @tparam T: the numeric type of the Vector3, i.e. double or float
	 */
	template<typename T>
	Vector3<T>::Vector3()
		: x(0.0), y(0.0), z(0.0) {}

	/*
	 * @brief Constructor for Vector3 which will take the given coordinates in the order of x, y, and z
	 * @tparam T: the numeric type of the Vector3, i.e. double or float
	 * @params x, y, z: the respective coordinates for each dimension, assigned in the listed order
	 */
	template<typename T>
	Vector3<T>::Vector3(T x, T y, T z)
		: x(x), y(y), z(z) {}

	/*
	 * @brief Scalar constructor overload for Vector3, setting an uniform value for x, y, and z.
	 * @tparam T: the numeric type of the Vector3, i.e. double or float
	 * @param scalar: the uniform value to set for each dimension, x, y, and z.
	 */
	template <typename T>
	Vector3<T>::Vector3(T scalar) 
		: x(scalar), y(scalar), z(scalar) {
	}

	/*
	 * @brief Overloaded division operator with a scalar value 
	 */
	template<typename T>
	Vector3<T> Vector3<T>::operator/(const T& scalar) const {
		return Vector3<T>(x / scalar, y / scalar, z / scalar);
	}

	/*
	 * @brief Overloaded multiplication operator with a scalar value 
	 */
	template<typename T>
	Vector3<T> Vector3<T>::operator*(const T& scalar) const {
		return Vector3<T>(x * scalar, y * scalar, z * scalar);
	}

	/*
	 * @brief Overloaded += operator, adding the other Vector3 to the current one
	 */
	template<typename T>
	Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	/*
     * @brief Overloaded /= operator, for scalar value
     */
	template<typename T>
	Vector3<T>& Vector3<T>::operator/=(const T& scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	/*
	 * @brief Overloaded == operator, checking two Vector3's for equality
	 */
	template<typename T>
	bool Vector3<T>::operator==(const Vector3<T>& other) const {
		return ((x == other.x) && (y == other.y) && (z == other.z)) ? true : false;
	}

	/*
	 * @brief Overloaded output operator 
	 */
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Vector3<T>& v) {
		return os << v.x << ", " << v.y << ", " << v.z << std::endl;
	}

	/* Explicit template instantiations*/
	template struct Vector3<double>;
	template struct Vector3<float>;

	template std::ostream& operator<< <double>(std::ostream& os, const Vector3<double>& v);
	template std::ostream& operator<< <float>(std::ostream& os, const Vector3<float>& v);

}
