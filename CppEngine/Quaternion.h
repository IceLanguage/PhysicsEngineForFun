#ifndef _PhysicsEngineForFun_Quaternion_
#define _PhysicsEngineForFun_Quaternion_

#include "Vector3.h"

struct Quaternion
{
private:
	float data[4];
public:
	Quaternion();
	Quaternion(const float r, const float i, const float j, const float k);
	float Get_r() const;
	float Get_i() const;
	float Get_j() const;
	float Get_k() const;
	void Set_r(float value);
	void Set_i(float value);
	void Set_j(float value);
	void Set_k(float value);
	void Normalize();
	void operator *=(const Quaternion &multiplier);
	void RotateByVector(const Vector3& vector);
	void AddScaledVector(const Vector3& vector, float scale);
	
};

#endif