#include "ParticleWorld.h"

ParticleWorld::ParticleWorld(unsigned int maxContacts, unsigned int contactsResolverIterations)
	:
	resolver(contactsResolverIterations),
	maxContacts(maxContacts)
{
	needCalculateIterations = (contactsResolverIterations == 0);
	contacts = new ParticleContact[maxContacts];
}

ParticleWorld::~ParticleWorld()
{
	delete[] contacts;
}

void ParticleWorld::Start()
{
	for (auto it = particles.begin(); it != particles.end(); ++it)
	{
		(*it)->ClearAccumulator();
	}
}

unsigned int ParticleWorld::GenerateContacts()
{
	unsigned int limit = maxContacts;
	ParticleContact *nextContact = contacts;

	for (auto it = contactGenerators.begin(); 
		 it != contactGenerators.end();
		 ++it)
	{
		bool used = (*it)->AddContact(nextContact);
		if (used)
		{
			--limit;
			nextContact += 1;
		}

		if (limit <= 0) break;
	}

	// Return the number of contacts used.
	return maxContacts - limit;
}

void ParticleWorld::Integrate(float duration)
{
	for (auto it = particles.begin(); it != particles.end(); ++it)
	{
		(*it)->Integrate(duration);
	}
}

void ParticleWorld::RunPhysics(float duration)
{
	Start();

	registry.UpdateForces(duration);
	Integrate(duration);

	unsigned int usedContacts = GenerateContacts();

	if (usedContacts)
    {
        if (needCalculateIterations) resolver.SetIterations(usedContacts * 2);
        resolver.ResolveContacts(contacts, usedContacts, duration);
    }
}

