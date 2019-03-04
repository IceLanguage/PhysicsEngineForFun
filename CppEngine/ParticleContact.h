#ifndef _PhysicsEngineForFun_ParticleContact_
#define _PhysicsEngineForFun_ParticleContact_

#include "Particle.h"
#include <vector>

class ParticleContact
{
public:
	void Resolve(float duration);
	float CalculateSeparatingVelocity() const;
	float restitutionCoefficient;
	float penetrationDepth;
	Vector3 contactNormal;
	Particle * particle0,* particle1;
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
	void ResolveContacts(std::vector<ParticleContact> contactsArray,
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