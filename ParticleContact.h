#ifndef _PhysicsEngineForFun_ParticleContact_
#define _PhysicsEngineForFun_ParticleContact_

#include "Particle.h"

class ParticleContact
{
public:
	ParticleContact(Particle* a, Particle* b, float restitutionCoefficient);
	void Resolve(float duration);
	float CalculateSeparatingVelocity() const;
	float restitutionCoefficient;
	float penetrationDepth;
	Particle * particles[2];
	Vector3 contactNormal;
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