#include "RigidBody.h"
#include <float.h>
#include <math.h>
#include <assert.h>

void RigidBody::CalculateDerivedData()
{
	transformMatrix.SetOrientationAndPos(orientation, position);
	inverseInertiaTensorWorld = transformMatrix * inverseInertiaTensor * transformMatrix.GetTransposeMatrix3();
}

void RigidBody::SetInertiaTensor(const Matrix3 & inertiaTensor)
{
	inverseInertiaTensor.SetInverse(inertiaTensor);
}

void RigidBody::AddForce(const Vector3 & force)
{
	forceAccum += force;
}

void RigidBody::AddForceAtBodyPoint(const Vector3 & force, const Vector3 & point)
{
	Vector3 pt = GetPointInWorldSpace(point);
	AddForceAtPoint(force, pt);
}

void RigidBody::AddForceAtPoint(const Vector3 & force, const Vector3 & point)
{
	Vector3 pt = point - position;

	forceAccum += force;
	torqueAccum += Vector3::Cross(force,pt);
}

void RigidBody::ClearAccumulator()
{
	forceAccum.Clear();
	torqueAccum.Clear();
}

void RigidBody::Integrate(float duration)
{
	acceleration.AddScaledVector(forceAccum, inverseMass);

	Vector3 angularAcceleration = inverseInertiaTensorWorld.Transform(torqueAccum);

	velocity.AddScaledVector(acceleration, duration);
	rotation.AddScaledVector(angularAcceleration, duration);

	velocity *= powf(linearDamping, duration);
	rotation *= powf(angularDamping, duration);

	position.AddScaledVector(velocity, duration);
	orientation.AddScaledVector(rotation, duration);

	CalculateDerivedData();

	ClearAccumulator();
}

Vector3 RigidBody::GetPointInWorldSpace(const Vector3 & point) const
{
	return transformMatrix.Transform(point);
}

bool RigidBody::hasFiniteMass() const
{
	return inverseMass >= 0.0f;
}

float RigidBody::GetMass() const
{
	if (inverseMass == 0) {
		return FLT_MAX;
	}
	else {
		return ((float)1.0) / inverseMass;
	}
}

void RigidBody::SetMass(float mass)
{
	assert(mass != 0);
	RigidBody::inverseMass = 1.f / mass;
}
