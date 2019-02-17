#include "ParticleForceGenerator.h"

GravityOnParticle::GravityOnParticle(const Vector3 & gravity)
{
}

void GravityOnParticle::UpdateForce(Particle * particle, float duration)
{
	if (particle->hasFiniteMass())
	{
		particle->AddForce(gravity * particle->GetMass());
	}
}

DragOnParticle::DragOnParticle(float k1, float k2)
{
	this->k1 = k1;
	this->k2 = k2;
}

void DragOnParticle::UpdateForce(Particle * particle, float duration)
{
	Vector3 v = particle->velocity;
	float p = v.Magnitude();
	float dragForce = k1 * p + k2 * p * p;
	Vector3 force = Vector3::Normalize(v);
	force *= -dragForce;
	particle->AddForce(force);
}

void ParticleForceRegistry::Add(Particle * particle, IParticleForceGenerator * fg)
{
	ParticleForceRegistration registration;
	registration.particle = particle;
	registration.fg = fg;
	registrations.push_back(registration);
}

void ParticleForceRegistry::Remove(Particle * particle, IParticleForceGenerator * fg)
{
	auto it = registrations.begin();
	for (; it != registrations.end(); it++)
	{
		if (it->particle == particle && it->fg == fg)
		{
			registrations.erase(it);
			break;
		}
	}
}

void ParticleForceRegistry::Clear()
{
	registrations.clear();
}

void ParticleForceRegistry::UpdateForces(float duration)
{
	auto it = registrations.begin();
	for (; it != registrations.end(); it++)
	{
		it->fg->UpdateForce(it->particle, duration);
	}
}


