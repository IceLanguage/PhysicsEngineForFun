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

class AeroForceOnRigidBody :public IRigidBodyForceGenerator
{
protected:
	Matrix3 tensor;//Holds the aerodynamic tensor for the surface in body space.
	const Vector3* windspeed;
	Vector3 position;
public:
	AeroForceOnRigidBody(const Matrix3 &tensor, const Vector3 &position,
		const Vector3 *windspeed);
	virtual void UpdateForce(RigidBody *body, float duration);
};

class AeroForceControlOnRigidBody :public AeroForceOnRigidBody
{
private:
	void ReCalculateTensor();
	Matrix3 maxTensor, minTensor;
public:
	AeroForceControlOnRigidBody(const Matrix3 &base,
		const Matrix3 &min, const Matrix3 &max,
		const Vector3 &position, const Vector3 *windspeed);
	virtual void UpdateForce(RigidBody *body, float duration);
	float controlSetting;
};
#endif
