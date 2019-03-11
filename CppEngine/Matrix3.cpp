#include "Matrix3.h"



Matrix3::Matrix3()
{
	for (int i = 0; i < 9; ++i) data[i] = 0;
}

Matrix3::Matrix3(float c0, float c1, float c2, float c3, float c4, float c5, float c6, float c7, float c8)
{
	data[0] = c0; data[1] = c1; data[2] = c2;
	data[3] = c3; data[4] = c4; data[5] = c5;
	data[6] = c6; data[7] = c7; data[8] = c8;
}

Vector3 Matrix3::operator*(const Vector3 & vector) const
{
	return Vector3(
		vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
		vector.x * data[3] + vector.y * data[4] + vector.z * data[5],
		vector.x * data[6] + vector.y * data[7] + vector.z * data[8]
	);
}

Matrix3 Matrix3::operator*(const Matrix3 & o) const
{
	return Matrix3(
		data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6],
		data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7],
		data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8],

		data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6],
		data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7],
		data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8],

		data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6],
		data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7],
		data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8]
	);
}

void Matrix3::SetInverse(const Matrix3 & m)
{
	float t4 = m.data[0] * m.data[4];
	float t6 = m.data[0] * m.data[5];
	float t8 = m.data[1] * m.data[3];
	float t10 = m.data[2] * m.data[3];
	float t12 = m.data[1] * m.data[6];
	float t14 = m.data[2] * m.data[6];

	// Calculate the determinant
	float t16 = (t4 * m.data[8] - t6 * m.data[7] - t8 * m.data[8] +
		t10 * m.data[7] + t12 * m.data[5] - t14 * m.data[4]);

	// Make sure the determinant is non-zero.
	if (t16 == 0.0f) return;
	float t17 = 1 / t16;

	data[0] = (m.data[4] * m.data[8] - m.data[5] * m.data[7])*t17;
	data[1] = -(m.data[1] * m.data[8] - m.data[2] * m.data[7])*t17;
	data[2] = (m.data[1] * m.data[5] - m.data[2] * m.data[4])*t17;
	data[3] = -(m.data[3] * m.data[8] - m.data[5] * m.data[6])*t17;
	data[4] = (m.data[0] * m.data[8] - t14)*t17;
	data[5] = -(t6 - t10)*t17;
	data[6] = (m.data[3] * m.data[7] - m.data[4] * m.data[6])*t17;
	data[7] = -(m.data[0] * m.data[7] - t12)*t17;
	data[8] = (t4 - t8)*t17;
}

void Matrix3::Invert()
{
	SetInverse(*this);
}

Matrix3 Matrix3::Inverse() const
{
	Matrix3 result;
	result.SetInverse(*this);
	return result;
}

void Matrix3::SetTranspose(const Matrix3 & m)
{
	data[0] = m.data[0];
	data[1] = m.data[3];
	data[2] = m.data[6];
	data[3] = m.data[1];
	data[4] = m.data[4];
	data[5] = m.data[7];
	data[6] = m.data[2];
	data[7] = m.data[5];
	data[8] = m.data[8];
}

Matrix3 Matrix3::Transpose() const
{
	Matrix3 result;
	result.SetTranspose(*this);
	return result;
}

void Matrix3::SetOrientation(const Quaternion & q)
{
	float r = q.Get_r();
	float i = q.Get_i();
	float j = q.Get_j();
	float k = q.Get_k();

	data[0] = 1 - (2 * j * j + 2 * k * k);
	data[1] = 2 * i * j + 2 * k * r;
	data[2] = 2 * i * k - 2 * j * r;
	data[3] = 2 * i * j - 2 * k * r;
	data[4] = 1 - (2 * i * i + 2 * k * k);
	data[5] = 2 * j * k + 2 * i * r;
	data[6] = 2 * i * k + 2 * j * r;
	data[7] = 2 * j * k - 2 * i * r;
	data[8] = 1 - (2 * i * i + 2 * j * j);
}

