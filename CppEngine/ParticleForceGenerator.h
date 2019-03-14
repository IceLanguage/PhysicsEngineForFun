#ifndef _PhysicsEngineForFun_ParticleForceGenerator_
#define _PhysicsEngineForFun_ParticleForceGenerator_

#include "Particle.h"
#include <list>

class IParticleForceGenerator
{
public:
	virtual void UpdateForce(Particle *particle, float duration) {};
};

class GravityOnParticle : IParticleForceGenerator
{
public:
	GravityOnParticle(const Vector3 & gravity);
	virtual void UpdateForce(Particle *particle, float duration);
	Vector3 gravity = Vector3(0.f, -9.8f, 0.f);
};

class DragOnParticle :IParticleForceGenerator
{
protected:
	float k1, k2;
public:
	DragOnParticle(float k1, float k2);
	virtual void UpdateForce(Particle *particle, float duration);
};

class SpringForceOnParticle : public IParticleForceGenerator
{
protected:
	Particle* other;
	float restLength;
	float elasticCoefficient;
public:
	SpringForceOnParticle(Particle* other, float restLength, float elasticCoefficient);
	virtual void UpdateForce(Particle *particle, float duration);
};

class RubberBandElasticOnParticle : public IParticleForceGenerator
{
protected:
	Particle * other;
	float restLength;
	float elasticCoefficient;
public:
	RubberBandElasticOnParticle(Particle* other, float restLength, float elasticCoefficient);
	virtual void UpdateForce(Particle *particle, float duration);
};

class BuoyancyOnParticle : public IParticleForceGenerator
{
protected:
	float liquidDensity;
	float waterHeight;
	float maxDepth;
	float volume;
public:
	float gravity = 9.8f;
	virtual void UpdateForce(Particle *particle, float duration);
};

class FakesSpringForceOnParticle : public IParticleForceGenerator
{
protected:
	Particle * other;//is attached to a fixed point in space.
	float elasticCoefficient;
	float damping;
public:
	virtual void UpdateForce(Particle *particle, float duration);
};

class ParticleForceRegistry
{
protected:
	struct ParticleForceRegistration
	{
		Particle *particle;
		IParticleForceGenerator *fg;
	};
	std::list<ParticleForceRegistration> registrations;
public:
	void Add(Particle* particle, IParticleForceGenerator *fg);
	void Remove(Particle* particle, IParticleForceGenerator *fg);
	void Clear();
	void UpdateForces(float duration);
};
#endif