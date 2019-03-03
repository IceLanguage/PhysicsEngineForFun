#ifndef _PhysicsEngineForFun_ParticleContact_
#define _PhysicsEngineForFun_ParticleContact_

#include "Particle.h"

class ParticleContact
{
public:
	ParticleContact(Particle* a, Particle* b);
	void Resolve(float duration);
	float CalculateSeparatingVelocity() const;
	float restitutionCoefficient;
	float penetrationDepth;
	Vector3 contactNormal;
	Particle * particles[2];
private:
	void ResolveVelocity(float duration);
	void ResolveInterpenetration(float duration);
};

class ParticleContactsResolver
{
private:
	unsigned int size;
public:
	unsigned int index;
	ParticleContactsResolver(unsigned int size);
	void ResolveContacts(ParticleContact *contactsArray,
		unsigned int numContacts,
		float duration);
};
#endif