#ifndef _PhysicsEngineForFun_ParticleContact_
#define _PhysicsEngineForFun_ParticleContact_

#include "Particle.h"

class ParticleContact
{
public:
	ParticleContact(Particle* a, Particle* b, float restitutionCoefficient);
	float restitutionCoefficient;
	float penetrationDepth;
	Particle * particles[2];
	Vector3 contactNormal;
private:
	float CalculateSeparatingVelocity() const;
	void ResolveVelocity(float duration);
	void ResolveInterpenetration(float duration);
};

#endif