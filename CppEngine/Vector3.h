#ifndef _PhysicsEngineForFun_Vector3_
#define _PhysicsEngineForFun_Vector3_

struct Vector3
{
public:
	Vector3() : x(0), y(0), z(0) {}
	Vector3(const float x, const float y, const float z)
		: x(x), y(y), z(z) {}

	float operator[](unsigned i) const;
	float& operator[](unsigned i);
	void operator+=(const Vector3& v);
	Vector3 operator+(const Vector3& v) const;
	void operator-=(const Vector3& v);
	Vector3 operator-(const Vector3& v) const;
	void operator*=(const float value);
	Vector3 operator*(const float value) const;
	void operator*=(const Vector3& v);
	Vector3 operator*(const Vector3& v) const;
	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;

	float SquareMagnitude() const;
	float Magnitude() const;
	void AddScaledVector(const Vector3& vector, float scale);
	void Normalize();
	void Clear();

	static Vector3 Cross(const Vector3& lhsV, const Vector3& rhsV);
	static float Dot(const Vector3& lhsV, const Vector3& rhsV);
	static Vector3 Normalize(const Vector3& v);

	float x, y, z;

	const static Vector3 right;
	const static Vector3 up;
	const static Vector3 forward;
	const static Vector3 zero;
};
Vector3 operator*(float value, const Vector3& v);

#endif
