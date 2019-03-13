#ifndef _PhysicsEngineForFun_RigidBody_
#define _PhysicsEngineForFun_RigidBody_

#include "Matrix4.h"
#include "Matrix3.h"

class RigidBody
{
private:
	float inverseMass;
	Matrix3 inverseInertiaTensorWorld;
	Matrix3 inverseInertiaTensor;
	Quaternion orientation;
public:
	void CalculateDerivedData();
	void SetInertiaTensor(const Matrix3 &inertiaTensor);
	Vector3 position;
	Vector3 velocity;
	Vector3 rotation;
	Matrix4 transformMatrix;
};

#endif