#ifndef _PhysicsEngineForFun_ParticleWorld_
#define _PhysicsEngineForFun_ParticleWorld__

#include "ParticleForceGenerator.h"
#include "ParticleContact.h"
#include <vector>

class ParticleWorld
{
private:	
	ParticleContactsResolver resolver;
	unsigned int maxContacts;
	bool needCalculateIterations;

	unsigned int GenerateContacts();
	void Integrate(float duration);

public:
	std::vector<Particle*> particles;
	std::vector<IParticleContactGenerator*> contactGenerators;
	ParticleContact* contacts;
	ParticleForceRegistry registry;

	ParticleWorld(unsigned int maxContacts, unsigned int contactsResolverIterations = 0);
	~ParticleWorld();
	void Start();
	void RunPhysics(float duration, bool needRestart = true);
};

#endif