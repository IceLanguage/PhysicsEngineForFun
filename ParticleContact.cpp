#include "ParticleContact.h"
#include <float.h>

ParticleContact::ParticleContact(Particle * a, Particle * b)
{
	particles[0] = a;
	particles[1] = b;
}

void ParticleContact::Resolve(float duration)
{
	ResolveVelocity(duration);
	ResolveInterpenetration(duration);
}

float ParticleContact::CalculateSeparatingVelocity() const
{
	Vector3 relativeVelocity = particles[0]->velocity - particles[1]->velocity;
	return Vector3::Dot(relativeVelocity, contactNormal);
}

void ParticleContact::ResolveVelocity(float duration)
{
	float separatingVelocity = CalculateSeparatingVelocity();

	//No need to separate
	if (separatingVelocity > 0) return;

	float newSepVelocity = - separatingVelocity * restitutionCoefficient;

	// If we've got a closing velocity due to acceleration build-up,
	// remove it from the new separating velocity
	Vector3 accCausedVelocity = particles[0]->acceleration - particles[1]->acceleration;
	float accCausedSepVelocity = Vector3::Dot(accCausedVelocity, contactNormal) * duration;
	if (accCausedSepVelocity < 0)
	{
		newSepVelocity += restitutionCoefficient * accCausedSepVelocity;
		if (newSepVelocity < 0) newSepVelocity = 0;
	}

	float deltaVelocity = newSepVelocity - separatingVelocity;

	float totalInverseMass = particles[0]->GetInverseMass() + particles[1]->GetInverseMass();

	if (totalInverseMass <= 0) return;

	float impulse = deltaVelocity / totalInverseMass;

	Vector3 impulsePerIMass = contactNormal * impulse;
	
	//The result of the collision is the impulse acting on the object
	particles[0]->velocity += impulsePerIMass * particles[0]->GetInverseMass();
	particles[1]->velocity += impulsePerIMass * particles[1]->GetInverseMass();
}

void ParticleContact::ResolveInterpenetration(float duration)
{
	if (penetrationDepth <= 0) return;

	float totalInverseMass = particles[0]->GetInverseMass() + particles[1]->GetInverseMass();

	if (totalInverseMass <= 0) return;

	Vector3 movePerIMass = contactNormal * (penetrationDepth / totalInverseMass);

	particles[0]->position += movePerIMass * particles[0]->GetInverseMass();
	particles[1]->position +=movePerIMass * -particles[1]->GetInverseMass();
}

ParticleContactsResolver::ParticleContactsResolver(unsigned int Iterations)
	:Iterations(Iterations)
{
}


void ParticleContactsResolver::ResolveContacts(std::vector<ParticleContact> contactsArray, unsigned int numContacts, float duration)
{
	unsigned int i;

	index = 0;
	while (index < Iterations)
	{
		float max = FLT_MAX;
		unsigned maxIndex = numContacts;
		for (i = 0; i < numContacts; i++)
		{
			float sepVel = contactsArray[i].CalculateSeparatingVelocity();
			if (sepVel < max &&
				(sepVel < 0 || contactsArray[i].penetrationDepth > 0))
			{
				max = sepVel;
				maxIndex = i;
			}
		}

		contactsArray[maxIndex].Resolve(duration);
		++index;
	}
}

void ParticleContactsResolver::SetIterations(unsigned int iterations)
{
	Iterations = iterations;
}
