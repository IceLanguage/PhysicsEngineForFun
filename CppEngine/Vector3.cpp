#include "Vector3.h"
#include <float.h>
#include <math.h>

const Vector3 Vector3::right = Vector3(1, 0, 0);
const Vector3 Vector3::up = Vector3(0, 1, 0);
const Vector3 Vector3::forward = Vector3(0, 0, 1);
const Vector3 Vector3::zero = Vector3(0, 0, 0);

float Vector3::operator[](unsigned i) const
{
	if (i == 0) return x;
	if (i == 1) return y;
	return z;
}

float & Vector3::operator[](unsigned i)
{
	if (i == 0) return x;
	if (i == 1) return y;
	return z;
}

void Vector3::operator+=(const Vector3 & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

Vector3 Vector3::operator+(const Vector3 & v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

void Vector3::operator-=(const Vector3 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

Vector3 Vector3::operator-(const Vector3 & v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

void Vector3::operator*=(const float value)
{
	x *= value;
	y *= value;
	z *= value;
}

Vector3 Vector3::operator*(const float value) const
{
	return Vector3(x * value, y * value, z * value);
}

void Vector3::operator*=(const Vector3 & v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

Vector3 Vector3::operator*(const Vector3 & v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

bool Vector3::operator==(const Vector3 & other) const
{
	return
		x == other.x &&
		y == other.y &&
		z == other.z;
}

bool Vector3::operator!=(const Vector3 & other) const
{
	return
		x != other.x ||
		y != other.y ||
		z != other.z;
}

float Vector3::SquareMagnitude() const
{
	return x * x + y * y + z * z;
}

float Vector3::Magnitude() const
{
	return sqrtf(SquareMagnitude());
}

void Vector3::AddScaledVector(const Vector3 & vector, float scale)
{
	x += vector.x * scale;
	y += vector.y * scale;
	z += vector.z * scale;
}

void Vector3::Normalize()
{
	float l = Magnitude();
	if (l > 0)
	{
		(*this) *= ((float)1) / l;
	}
}

void Vector3::Clear()
{
	x = y = z = 0;
}

Vector3 Vector3::Cross(const Vector3 & lhsV, const Vector3 & rhsV)
{
	return Vector3(
		lhsV.y * rhsV.z - lhsV.z * rhsV.y,
		lhsV.z * rhsV.x - lhsV.x * rhsV.z,
		lhsV.x * rhsV.y - lhsV.y * rhsV.x);
}

float Vector3::Dot(const Vector3 & lhsV, const Vector3 & rhsV)
{
	return lhsV.x * rhsV.x + lhsV.y * rhsV.y + lhsV.z * rhsV.z;
}

Vector3 Vector3::Normalize(const Vector3 & v)
{
	float l = v.Magnitude();
	Vector3 res = zero;
	if (l > 0)
	{
		float reciprocal = ((float)1) / l;
		res = reciprocal * v;
	}
	return res;
}

Vector3 operator*(float value, const Vector3 & v)
{
	return v * value;
}
