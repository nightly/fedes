#pragma once

#include <ostream>

namespace fedes {

	template <typename T>
	struct Vector3 {
		T x, y, z;
		Vector3(T x, T y, T z);
		Vector3();

		Vector3 operator/(const T& scalar) const;
		Vector3 operator*(const T& scalar) const;
		Vector3& operator+=(const Vector3<T>& other);
		Vector3& operator/=(const T& scalar);
		bool operator==(const Vector3<T>& other) const;
	};

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Vector3<T>& v);
}