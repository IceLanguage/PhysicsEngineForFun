#ifndef _PhysicsEngineForFun_Vector3_
#define _PhysicsEngineForFun_Vector3_

#include "Precision.h"

namespace PhysicsEngineForFun
{
	struct Vector3
	{
	public:
		Vector3() : x(0), y(0), z(0) {}
		Vector3(const real x, const real y, const real z)
			: x(x), y(y), z(z) {}

		real operator[](unsigned i) const;
		real& operator[](unsigned i); 
		void operator+=(const Vector3& v);
		Vector3 operator+(const Vector3& v) const;
		void operator-=(const Vector3& v);
		Vector3 operator-(const Vector3& v) const;
		void operator*=(const real value);
		Vector3 operator*(const real value) const;
		void operator*=(const Vector3& v);
		Vector3 operator*(const Vector3& v) const;
		bool operator==(const Vector3& other) const;
		bool operator!=(const Vector3& other) const;

		real SquareMagnitude() const;
		real Magnitude() const;
		void AddScaledVector(const Vector3& vector, real scale);
		void Normalize();
		void Clear();

		static Vector3 Cross(const Vector3& lhsV, const Vector3& rhsV);
		static real Dot(const Vector3& lhsV, const Vector3& rhsV);
		static Vector3 Normalize(const Vector3& v);

		real x, y, z;

		const static Vector3 right;
		const static Vector3 up;
		const static Vector3 forward;
		const static Vector3 zero;
	};
	Vector3 operator*(real value, const Vector3& v);
}

#endif
