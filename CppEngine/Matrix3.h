#ifndef _PhysicsEngineForFun_Matrix3_
#define _PhysicsEngineForFun_Matrix3_

#include "Vector3.h"
#include "Quaternion.h"

struct Matrix3
{
public:
	Matrix3();
	Matrix3(float c0, float c1, float c2, float c3, float c4, float c5,
		float c6, float c7, float c8);
	
	Vector3 operator*(const Vector3 &vector) const;
	Matrix3 operator*(const Matrix3 &o) const;
	void SetInverse(const Matrix3 &m);
	void Invert();
	Matrix3 Inverse() const;
	void SetTranspose(const Matrix3 &m);
	Matrix3 Transpose() const;
	void SetOrientation(const Quaternion &q);
	Vector3 Transform(const Vector3 &vector) const;
	static Matrix3 linearInterpolate(const Matrix3& a, const Matrix3& b, float prop);
	
	float data[9];
};

#endif