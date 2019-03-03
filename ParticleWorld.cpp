#include "ParticleWorld.h"

ParticleWorld::ParticleWorld(unsigned int maxContacts, unsigned int contactsResolverIterations)
	:
	resolver(contactsResolverIterations),
	maxContacts(maxContacts)
{
	needCalculateIterations = (contactsResolverIterations == 0);
}

ParticleWorld::~ParticleWorld()
{
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
	unsigned limit = maxContacts;
	auto nextContact = contacts.begin();

	for (auto it = contactGenerators.begin(); 
		 it != contactGenerators.end() && nextContact!= contacts.end();
		 ++it)
	{
		bool used = (*it)->addContact(& *nextContact, limit);
		if (used)
		{
			--limit;
			++nextContact;
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
	registry.UpdateForces(duration);
	Integrate(duration);

	unsigned int usedContacts = GenerateContacts();

	if (usedContacts)
    {
        if (needCalculateIterations) resolver.SetIterations(usedContacts * 2);
        resolver.ResolveContacts(contacts, usedContacts, duration);
    }
}
