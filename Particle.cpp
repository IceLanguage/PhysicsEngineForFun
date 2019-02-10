#include "Particle.h"
#include <assert.h>

using namespace PhysicsEngineForFun;

void PhysicsEngineForFun::Particle::SetMass(real mass)
{
	assert(mass != 0);
	inverseMass = ((real)1.0) / mass;
}

real PhysicsEngineForFun::Particle::GetMass() const
{
	if (inverseMass == 0) {
		return REAL_MAX;
	}
	else {
		return ((real)1.0) / inverseMass;
	}
}

void PhysicsEngineForFun::Particle::AddForce(const Vector3 & force)
{
	this->force += force;
}

void PhysicsEngineForFun::Particle::Integrate(real duration)
{
	if (inverseMass <= 0.0f) return;
	assert(duration > 0.0);
	position.AddScaledVector(velocity, duration);
	Vector3 resultingAcc = acceleration;
	resultingAcc.AddScaledVector(force, inverseMass);
	velocity.AddScaledVector(resultingAcc, duration);
	velocity *= real_pow(damping, duration);
	force.Clear();
}
