#ifndef _PhysicsEngineForFun_ParticleForceGenerator_
#define _PhysicsEngineForFun_ParticleForceGenerator_

#include "Particle.h"
#include <list>

class IParticleForceGenerator
{
public:
	virtual void UpdateForce(Particle *particle, float duration) = 0;
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