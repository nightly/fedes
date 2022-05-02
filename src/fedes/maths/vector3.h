#pragma once

#include <ostream>
#include <concepts>

namespace fedes {

	template <std::floating_point T>
	struct Vector3 {
		T x, y, z;
		Vector3(T x, T y, T z);
		Vector3(T scalar);
		Vector3();

		Vector3 operator/(const T& scalar) const;
		Vector3 operator*(const T& scalar) const;
		Vector3& operator/=(const T& scalar);

		Vector3 operator+(const Vector3<T>& other) const;
		Vector3& operator+=(const Vector3<T>& other);
		Vector3 operator-(const Vector3<T>& other) const;
		Vector3 operator*(const Vector3<T>& other) const;
		bool operator==(const Vector3<T>& other) const;

		Vector3 cross(const Vector3<T>& other) const;
	};

	template <std::floating_point T>
	std::ostream& operator<<(std::ostream& os, const Vector3<T>& v);
}