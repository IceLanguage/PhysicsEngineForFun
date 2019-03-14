#ifndef _PhysicsEngineForFun_RigidBodyForceGenerator_
#define _PhysicsEngineForFun_RigidBodyForceGenerator_

#include "RigidBody.h"

class IRigidBodyForceGenerator
{
public:
	virtual void UpdateForce(RigidBody * body, float duration) {};
};

class GravityOnRigidBody : public IRigidBodyForceGenerator
{
public:
	GravityOnRigidBody(const Vector3 & gravity);
	virtual void UpdateForce(RigidBody * body, float duration);
	Vector3 gravity = Vector3(0.f, -9.8f, 0.f);
};

class SpringForceOnRigidBody :public IRigidBodyForceGenerator
{
protected:
	RigidBody * other;
	float restLength;
	float elasticCoefficient;
	Vector3 connectionPoint, otherConnectionPoint;
public:
	SpringForceOnRigidBody(RigidBody* other, float restLength, float elasticCoefficient, const Vector3 &localConnectionPt, const Vector3 &otherConnectionPt);
	virtual void UpdateForce(RigidBody *body, float duration);
};
#endif
