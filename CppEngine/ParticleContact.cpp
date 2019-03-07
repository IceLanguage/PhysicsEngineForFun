#include "ParticleContact.h"
#include <float.h>

void ParticleContact::Resolve(float duration)
{
	ResolveVelocity(duration);
	ResolveInterpenetration(duration);
}

float ParticleContact::CalculateSeparatingVelocity() const
{
	Vector3 relativeVelocity = particle0->velocity;

	if (particle1)
	{
		relativeVelocity -= particle1->velocity;
	}
	
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
	Vector3 accCausedVelocity = particle0->acceleration;
	if(particle1) accCausedVelocity -= particle1->acceleration;

	float accCausedSepVelocity = Vector3::Dot(accCausedVelocity, contactNormal) * duration;
	if (accCausedSepVelocity < 0)
	{
		newSepVelocity += restitutionCoefficient * accCausedSepVelocity;
		if (newSepVelocity < 0) newSepVelocity = 0;
	}

	float deltaVelocity = newSepVelocity - separatingVelocity;

	float totalInverseMass = particle0->GetInverseMass();
	if (particle1) totalInverseMass += particle1->GetInverseMass();

	if (totalInverseMass <= 0) return;

	float impulse = deltaVelocity / totalInverseMass;

	Vector3 impulsePerIMass = contactNormal * impulse;
	
	//The result of the collision is the impulse acting on the object
	particle0->velocity += impulsePerIMass * particle0->GetInverseMass();
	if (particle1) particle1->velocity -= impulsePerIMass * particle1->GetInverseMass();
}

void ParticleContact::ResolveInterpenetration(float duration)
{
	if (penetrationDepth <= 0) return;

	float totalInverseMass = particle0->GetInverseMass();
	if (particle1) totalInverseMass += particle1->GetInverseMass();

	if (totalInverseMass <= 0) return;

	Vector3 movePerIMass = contactNormal * (penetrationDepth / totalInverseMass);

	particleMovement[0] = movePerIMass * particle0->GetInverseMass();
	if (particle1) {
		particleMovement[1] = movePerIMass * -particle1->GetInverseMass();
	}
	else {
		particleMovement[1].Clear();
	}
	particle0->position += particleMovement[0];
	if (particle1) particle1->position += particleMovement[1];
}

ParticleContactsResolver::ParticleContactsResolver(unsigned int Iterations)
	:Iterations(Iterations)
{
}


void ParticleContactsResolver::ResolveContacts(ParticleContact* contactsArray, unsigned int numContacts, float duration)
{
	unsigned int i;

	index = 0;
	while (index < Iterations)
	{
		float max = FLT_MAX;
		unsigned int maxIndex = numContacts;
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

		if (maxIndex == numContacts) break;

		contactsArray[maxIndex].Resolve(duration);

		Vector3 *move = contactsArray[maxIndex].particleMovement;
		for (i = 0; i < numContacts; i++)
		{
			if (contactsArray[i].particle0 == contactsArray[maxIndex].particle0)
			{
				contactsArray[i].penetrationDepth -= Vector3::Dot(move[0], contactsArray[i].contactNormal);
			}
			else if (contactsArray[i].particle0 == contactsArray[maxIndex].particle1)
			{
				contactsArray[i].penetrationDepth -= Vector3::Dot(move[1], contactsArray[i].contactNormal);
			}
			if (contactsArray[i].particle1)
			{
				if (contactsArray[i].particle1 == contactsArray[maxIndex].particle0)
				{
					contactsArray[i].penetrationDepth += Vector3::Dot(move[0], contactsArray[i].contactNormal);
				}
				else if (contactsArray[i].particle1 == contactsArray[maxIndex].particle1)
				{
					contactsArray[i].penetrationDepth += Vector3::Dot(move[1], contactsArray[i].contactNormal);
				}
			}
		}
		++index;
	}
}

void ParticleContactsResolver::SetIterations(unsigned int iterations)
{
	Iterations = iterations;
}
