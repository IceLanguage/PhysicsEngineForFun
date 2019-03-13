#ifndef _PhysicsEngineForFun_Matrix4_
#define _PhysicsEngineForFun_Matrix4_

#include "Matrix3.h"

struct Matrix4
{
public:
	Matrix4();
	Matrix4 operator*(const Matrix4 &o) const;
	Vector3 operator*(const Vector3 &vector) const;
	float GetDeterminant() const;
	void SetOrientationAndPos(const Quaternion &q, const Vector3 &pos);
	Vector3 Transform(const Vector3 &vector) const;
	Vector3 TransformInverse(const Vector3 &vector) const;
	Vector3 TransformDirection(const Vector3 &vector) const;
	Vector3 TransformInverseDirection(const Vector3 &vector) const;
	Matrix3 operator*(const Matrix3 &o) const;
	Matrix3 GetTransposeMatrix3() const;
	float data[12];
};

#endif
