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
	void SetInertiaTensorCoeffs(float ix, float iy, float iz,
		float ixy = 0, float ixz = 0, float iyz = 0);//Sets the value of the matrix from inertia tensor values.
	void SetBlockInertiaTensor(const Vector3 &halfSizes, float mass);
	static Matrix3 linearInterpolate(const Matrix3& a, const Matrix3& b, float prop);
	
	float data[9];
};

#endif