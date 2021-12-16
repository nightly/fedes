#pragma once
namespace fedes {

	template<typename T>
	struct Vector3 {
		T x, y, z;
		Vector3(T x, T y, T z);
		Vector3();

		Vector3 operator/(const T& scalar) const;
		Vector3 operator*(const T& scalar) const;
		Vector3& operator+=(const Vector3<T>& other);
	};
}