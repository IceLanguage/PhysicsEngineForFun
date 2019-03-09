#ifndef _PhysicsEngineForFun_ParticleWorld_
#define _PhysicsEngineForFun_ParticleWorld__

#include "ParticleForceGenerator.h"
#include "ParticleContact.h"
#include <vector>

class ParticleWorld
{
private:	
	ParticleForceRegistry registry;
	ParticleContactsResolver resolver;
	unsigned int maxContacts;
	bool needCalculateIterations;

	unsigned int GenerateContacts();
	void Integrate(float duration);

public:
	std::vector<Particle*> particles;
	std::vector<IParticleContactGenerator*> contactGenerators;
	ParticleContact* contacts;

	ParticleWorld(unsigned int maxContacts, unsigned int contactsResolverIterations = 0);
	~ParticleWorld();
	void Start();
	void RunPhysics(float duration);
};

class SimpleGroundContactGenerator :IParticleContactGenerator
{
public:
	Vector3 pointInGround;
	Vector3 normal;
	Particle* particle;
	float restitutionCoefficient;
	virtual bool AddContact(ParticleContact *contact,
		unsigned int limit) const;
};
#endif