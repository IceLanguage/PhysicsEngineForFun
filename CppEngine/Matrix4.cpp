#include "Matrix4.h"

Matrix4::Matrix4()
{
	for (int i = 0; i < 12; ++i) data[i] = 0;
	data[0] = data[5] = data[10] = 1;
}

Matrix4 Matrix4::operator*(const Matrix4 & o) const
{
	Matrix4 result;
	result.data[0] = (o.data[0] * data[0]) + (o.data[4] * data[1]) + (o.data[8] * data[2]);
	result.data[4] = (o.data[0] * data[4]) + (o.data[4] * data[5]) + (o.data[8] * data[6]);
	result.data[8] = (o.data[0] * data[8]) + (o.data[4] * data[9]) + (o.data[8] * data[10]);

	result.data[1] = (o.data[1] * data[0]) + (o.data[5] * data[1]) + (o.data[9] * data[2]);
	result.data[5] = (o.data[1] * data[4]) + (o.data[5] * data[5]) + (o.data[9] * data[6]);
	result.data[9] = (o.data[1] * data[8]) + (o.data[5] * data[9]) + (o.data[9] * data[10]);

	result.data[2] = (o.data[2] * data[0]) + (o.data[6] * data[1]) + (o.data[10] * data[2]);
	result.data[6] = (o.data[2] * data[4]) + (o.data[6] * data[5]) + (o.data[10] * data[6]);
	result.data[10] = (o.data[2] * data[8]) + (o.data[6] * data[9]) + (o.data[10] * data[10]);

	result.data[3] = (o.data[3] * data[0]) + (o.data[7] * data[1]) + (o.data[11] * data[2]) + data[3];
	result.data[7] = (o.data[3] * data[4]) + (o.data[7] * data[5]) + (o.data[11] * data[6]) + data[7];
	result.data[11] = (o.data[3] * data[8]) + (o.data[7] * data[9]) + (o.data[11] * data[10]) + data[11];

	return result;
}

Vector3 Matrix4::operator*(const Vector3 & vector) const
{
	return Vector3(
		vector.x * data[0] +
		vector.y * data[1] +
		vector.z * data[2] + data[3],

		vector.x * data[4] +
		vector.y * data[5] +
		vector.z * data[6] + data[7],

		vector.x * data[8] +
		vector.y * data[9] +
		vector.z * data[10] + data[11]
	);
}

float Matrix4::GetDeterminant() const
{
	return 
	   -data[8] * data[5] * data[2]  +
		data[4] * data[9] * data[2]  +
		data[8] * data[1] * data[6]  -
		data[0] * data[9] * data[6]  -
		data[4] * data[1] * data[10] +
		data[0] * data[5] * data[10];
}

void Matrix4::SetOrientationAndPos(const Quaternion & q, const Vector3 & pos)
{
	float r = q.Get_r();
	float i = q.Get_i();
	float j = q.Get_j();
	float k = q.Get_k();

	data[0] = 1 - (2 * j * j + 2 * k * k);
	data[1] = 2 * i * j + 2 * k * r;
	data[2] = 2 * i * k - 2 * j * r;
	data[3] = pos.x;

	data[4] = 2 * i * j - 2 * k * r;
	data[5] = 1 - (2 * i * i + 2 * k * k);
	data[6] = 2 * j * k + 2 * i * r;
	data[7] = pos.y;

	data[8] = 2 * i * k + 2 * j * r;
	data[9] = 2 * j * k - 2 * i * r;
	data[10] = 1 - (2 * i * i + 2 * j * j);
	data[11] = pos.z;
}

Vector3 Matrix4::Transform(const Vector3 & vector) const
{
	return (*this) * vector;
}

Vector3 Matrix4::TransformInverse(const Vector3 & vector) const
{
	Vector3 tmp = vector;
	tmp.x -= data[3];
	tmp.y -= data[7];
	tmp.z -= data[11];
	return Vector3(
		tmp.x * data[0] +
		tmp.y * data[4] +
		tmp.z * data[8],

		tmp.x * data[1] +
		tmp.y * data[5] +
		tmp.z * data[9],

		tmp.x * data[2] +
		tmp.y * data[6] +
		tmp.z * data[10]
	);
}

Vector3 Matrix4::TransformDirection(const Vector3 & vector) const
{
	return Vector3(
		vector.x * data[0] +
		vector.y * data[1] +
		vector.z * data[2],

		vector.x * data[4] +
		vector.y * data[5] +
		vector.z * data[6],

		vector.x * data[8] +
		vector.y * data[9] +
		vector.z * data[10]
	);
}

Vector3 Matrix4::TransformInverseDirection(const Vector3 & vector) const
{
	return Vector3(
		vector.x * data[0] +
		vector.y * data[4] +
		vector.z * data[8],

		vector.x * data[1] +
		vector.y * data[5] +
		vector.z * data[9],

		vector.x * data[2] +
		vector.y * data[6] +
		vector.z * data[10]
	);
}

Matrix3 Matrix4::operator*(const Matrix3 & o) const
{
	return Matrix3(
		data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6],
		data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7],
		data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8],

		data[4] * o.data[0] + data[5] * o.data[3] + data[6] * o.data[6],
		data[4] * o.data[1] + data[5] * o.data[4] + data[6] * o.data[7],
		data[4] * o.data[2] + data[5] * o.data[5] + data[6] * o.data[8],

		data[8] * o.data[0] + data[9] * o.data[3] + data[10] * o.data[6],
		data[8] * o.data[1] + data[9] * o.data[4] + data[10] * o.data[7],
		data[8] * o.data[2] + data[9] * o.data[5] + data[10] * o.data[8]
	);
}

Matrix3 Matrix4::GetTransposeMatrix3() const
{
	Matrix3 result;
	result.data[0] = data[0];
	result.data[1] = data[4];
	result.data[2] = data[8];
	result.data[3] = data[1];
	result.data[4] = data[5];
	result.data[5] = data[9];
	result.data[6] = data[2];
	result.data[7] = data[6];
	result.data[8] = data[10];
	return result;
}

Vector3 Matrix4::GetColumn(int i) const
{
	return Vector3(data[i], data[i + 4], data[i + 8]);
}
