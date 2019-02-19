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

SpringForceOnParticle::SpringForceOnParticle(Particle * other, float restLength, float elasticCoefficient)
{
	this->other = other;
	this->restLength = restLength;
	this->elasticCoefficient = elasticCoefficient;
}

void SpringForceOnParticle::UpdateForce(Particle * particle, float duration)
{
	Vector3 vDis = particle->position - other->position;
	float dis = vDis.Magnitude();
	float dx = fabsf(dis - restLength);
	float forceValue = elasticCoefficient * dx;
	Vector3 force = Vector3::Normalize(vDis);
	force *= -forceValue;
	particle->AddForce(force);
}

RubberBandElasticOnParticle::RubberBandElasticOnParticle(Particle * other, float restLength, float elasticCoefficient)
{
	this->other = other;
	this->restLength = restLength;
	this->elasticCoefficient = elasticCoefficient;
}

void RubberBandElasticOnParticle::UpdateForce(Particle * particle, float duration)
{
	Vector3 vDis = particle->position - other->position;
	float dis = vDis.Magnitude();
	if (dis <= restLength) return;
	float dx = restLength - dis;
	float forceValue = elasticCoefficient * dx;
	Vector3 force = Vector3::Normalize(vDis);
	force *= -forceValue;
	particle->AddForce(force);
}

void BuoyancyOnParticle::UpdateForce(Particle * particle, float duration)
{
	float height = particle->position.y;
	float objectInWaterMaxHeight = waterHeight + maxDepth;

	if (height > objectInWaterMaxHeight) return;

	Vector3 force = Vector3::zero;
	force.y = liquidDensity * volume * gravity;

	if (height >  waterHeight - maxDepth)
	{
		float objectBottomHeight = height - maxDepth;
		force.y *= (waterHeight - objectBottomHeight) / (maxDepth * 2);
	}

	particle->AddForce(force);
} 

//I can not understand the principle about the Solution of differential equation -- from ¡¶Game Physics Engine Development¡· p96
void FakesSpringForceOnParticle::UpdateForce(Particle * particle, float duration)
{
	if (!particle->hasFiniteMass()) return;

	Vector3 v = particle->velocity;
	Vector3 vDis = particle->position - other->position;

	float gmma = 0.5f * sqrtf(4 * elasticCoefficient - damping * damping);
	if (gmma == 0) return;
	Vector3 c = vDis * (damping / (2.0f * gmma)) + 1.0f / gmma * v;

	Vector3 target = vDis * cosf(gmma * duration) + c * sinf(gmma * duration);
	target *= expf(-0.5f * duration * damping);

	Vector3 acc = 1.f / (duration * duration) * (target - vDis) - v * duration;

	particle->AddForce(acc * particle->GetMass());
}
