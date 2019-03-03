#ifndef _PhysicsEngineForFun_ParticleWorld_
#define _PhysicsEngineForFun_ParticleWorld__

#include "ParticleForceGenerator.h"
#include "ParticleContact.h"
#include <vector>

class ParticleWorld
{
private:
	std::vector<Particle*> particles;
	std::vector<IParticleContactGenerator*> contactGenerators;
	ParticleForceRegistry registry;
	ParticleContactsResolver resolver;
	std::vector<ParticleContact> contacts;
	unsigned int maxContacts;
	bool needCalculateIterations;
public:
	ParticleWorld(unsigned int maxContacts, unsigned int contactsResolverIterations = 0);
	~ParticleWorld();
	void Start();
	unsigned int GenerateContacts();
	void Integrate(float duration);
	void RunPhysics(float duration);
};

#endif