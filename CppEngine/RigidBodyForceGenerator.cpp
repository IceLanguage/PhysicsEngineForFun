#include "RigidBodyForceGenerator.h"
#include <math.h>
GravityOnRigidBody::GravityOnRigidBody(const Vector3 & gravity) :gravity(gravity)
{
}

void GravityOnRigidBody::UpdateForce(RigidBody * body, float duration)
{
	if (body->hasFiniteMass())
	{
		body->AddForce(gravity * body->GetMass());
	}
}

SpringForceOnRigidBody::SpringForceOnRigidBody(RigidBody* other, float restLength, float elasticCoefficient, const Vector3 &localConnectionPt, const Vector3 &otherConnectionPt)
	:other(other), restLength(restLength), elasticCoefficient(elasticCoefficient), connectionPoint(localConnectionPt),otherConnectionPoint(otherConnectionPt)
{
}

void SpringForceOnRigidBody::UpdateForce(RigidBody * body, float duration)
{
	Vector3 v1 = body->GetPointInWorldSpace(connectionPoint);

	Vector3 vDis = v1 - other->GetPointInWorldSpace(otherConnectionPoint);
	float dis = vDis.Magnitude();
	float dx = fabsf(dis - restLength);
	float forceValue = elasticCoefficient * dx;
	Vector3 force = Vector3::Normalize(vDis);
	force *= -forceValue;
	body->AddForceAtPoint(force, v1);
}

AeroForceOnRigidBody::AeroForceOnRigidBody(const Matrix3 & tensor, const Vector3 & position, const Vector3 * windspeed)
	:tensor(tensor),position(position),windspeed(windspeed)
{
}

void AeroForceOnRigidBody::UpdateForce(RigidBody * body, float duration)
{
	Vector3 velocity = body->velocity;
	velocity += *windspeed;

	// Calculate the velocity in body coordinates
	Vector3 bodyVel = body->transformMatrix.TransformInverseDirection(velocity);

	// Calculate the force in body coordinates
	Vector3 bodyForce = tensor.Transform(bodyVel);
	Vector3 force = body->transformMatrix.TransformDirection(bodyForce);

	// Apply the force
	body->AddForceAtBodyPoint(force, position);
}

void AeroForceControlOnRigidBody::ReCalculateTensor()
{
	if (controlSetting <= -1.0f)
	{
		tensor = minTensor;
		return;
	}
	else if (controlSetting >= 1.0f)
	{
		tensor = maxTensor;
		return;
	}
	else if (controlSetting == 0)
	{
		return;
	}
	else if (controlSetting < 0)
	{
		tensor = Matrix3::linearInterpolate(minTensor, tensor, controlSetting + 1.0f);
		return;
	}
	else if(controlSetting > 0)
	{
		tensor = Matrix3::linearInterpolate(tensor, maxTensor, controlSetting);
		return;
	}
}

AeroForceControlOnRigidBody::AeroForceControlOnRigidBody(const Matrix3 & base, const Matrix3 & min, const Matrix3 & max, const Vector3 & position, const Vector3 * windspeed)
	:AeroForceOnRigidBody(base, position, windspeed), maxTensor(max), minTensor(min)
{
}

void AeroForceControlOnRigidBody::UpdateForce(RigidBody * body, float duration)
{
	ReCalculateTensor();
	AeroForceOnRigidBody::UpdateForce(body, duration);
}

void RigidBodyForceRegistry::Add(RigidBody * body, IRigidBodyForceGenerator * fg)
{
	ForceRegistration registration;
	registration.body = body;
	registration.fg = fg;
	registrations.push_back(registration);
}

void RigidBodyForceRegistry::Remove(RigidBody * body, IRigidBodyForceGenerator * fg)
{
	auto it = registrations.begin();
	for (; it != registrations.end(); it++)
	{
		if (it->body == body && it->fg == fg)
		{
			registrations.erase(it);
			break;
		}
	}
}

void RigidBodyForceRegistry::Clear()
{
	registrations.clear();
}

void RigidBodyForceRegistry::UpdateForces(float duration)
{
	auto i = registrations.begin();
	for (; i != registrations.end(); i++)
	{
		i->fg->UpdateForce(i->body, duration);
	}
}
