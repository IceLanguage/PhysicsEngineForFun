#ifndef _PhysicsEngineForFun_RigidBodyForceGenerator_
#define _PhysicsEngineForFun_RigidBodyForceGenerator_

#include "RigidBody.h"
#include <vector>

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

class BuoyancyOnRigidBody :public IRigidBodyForceGenerator
{
private:
	float maxDepth;
	float volume;
	float waterSurfaceHeight;
	float liquidDensity;
	Vector3 centerOfBuoyancy;//The center of buoyancy of the rigid body, in body coordinates.
public:
	BuoyancyOnRigidBody(const Vector3 &centerOfBuoyancy,
		float maxDepth, float volume, float waterBootomHeight,
		float liquidDensity = 1000.0f);
	virtual void UpdateForce(RigidBody *body, float duration);
};

class RigidBodyForceRegistry
{
protected:
	struct ForceRegistration
	{
		RigidBody *body;
		IRigidBodyForceGenerator *fg;
	};
	std::vector<ForceRegistration> registrations;

public:
	void Add(RigidBody* body, IRigidBodyForceGenerator *fg);
	void Remove(RigidBody* body, IRigidBodyForceGenerator *fg);
	void Clear();
	void UpdateForces(float duration);
};
#endif
