#ifndef _PhysicsEngineForFun_ParticleContact_
#define _PhysicsEngineForFun_ParticleContact_

#include "Particle.h"

class ParticleContact
{
public:
	void Resolve(float duration);
	float CalculateSeparatingVelocity() const;
	float restitutionCoefficient;
	float penetrationDepth;
	Vector3 contactNormal;
	Particle * particle0,* particle1;
	Vector3 particleMovement[2];
private:
	void ResolveVelocity(float duration);
	void ResolveInterpenetration(float duration);

};

class ParticleContactsResolver
{
private:
	unsigned int Iterations;
public:
	unsigned int index;
	ParticleContactsResolver(unsigned int Iterations);
	void ResolveContacts(ParticleContact* contactsArray,
		unsigned int numContacts,
		float duration);
	void SetIterations(unsigned int iterations);
};

class IParticleContactGenerator
{
public:
	virtual bool AddContact(ParticleContact *contact,
		unsigned int limit) const = 0;
};
#endif