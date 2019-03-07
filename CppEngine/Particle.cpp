#include "Particle.h"
#include <assert.h>
#include <float.h>
#include <math.h>

void Particle::SetMass(float mass)
{
	assert(mass != 0);
	inverseMass = ((float)1.0) / mass;
}

float Particle::GetMass() const
{
	if (inverseMass == 0) {
		return FLT_MAX;
	}
	else {
		return ((float)1.0) / inverseMass;
	}
}

void Particle::AddForce(const Vector3 & force)
{
	this->force += force;
}

bool Particle::hasFiniteMass() const
{
	return inverseMass >= 0.f;
}

float Particle::GetInverseMass() const
{
	return inverseMass;
}

void Particle::ClearAccumulator()
{
	force.Clear();
}

void Particle::Integrate(float duration)
{
	if (inverseMass <= 0.0f) return;
	assert(duration > 0.0);
	position.AddScaledVector(velocity, duration);
	Vector3 resultingAcc = acceleration;
	resultingAcc.AddScaledVector(force, inverseMass);
	velocity.AddScaledVector(resultingAcc, duration);
	velocity *= powf(damping, duration);
	force.Clear();
}
