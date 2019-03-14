#ifndef _PhysicsEngineForFun_RigidBody_
#define _PhysicsEngineForFun_RigidBody_

#include "Matrix4.h"
#include "Matrix3.h"

class RigidBody
{
private:
	Vector3 forceAccum;
	Vector3 torqueAccum;
	float inverseMass;
	Matrix3 inverseInertiaTensorWorld;
	Matrix3 inverseInertiaTensor;
	Quaternion orientation;
public:
	void CalculateDerivedData();
	void SetInertiaTensor(const Matrix3 &inertiaTensor);
	void AddForce(const Vector3 &force);
	void AddForceAtBodyPoint(const Vector3 &force, const Vector3 &point);
	void AddForceAtPoint(const Vector3 &force, const Vector3 &point);
	void ClearAccumulator();
	void Integrate(float duration);
	Vector3 GetPointInWorldSpace(const Vector3 &point) const;
	bool hasFiniteMass() const;
	float GetMass() const;
	Vector3 position;
	Vector3 velocity;
	Vector3 rotation;
	Vector3 acceleration;
	Matrix4 transformMatrix;
	float angularDamping;
	float linearDamping;
};

#endif