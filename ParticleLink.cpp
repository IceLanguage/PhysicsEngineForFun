#include "ParticleLink.h"

ParticleLink::ParticleLink(Particle * a, Particle * b)
{
	particles[0] = a;
	particles[1] = b;
}

float ParticleLink::GetCurrentLength() const
{
	Vector3 relativePos = particles[0]->position - particles[1]->position;
	return relativePos.Magnitude();
}

ParticleCable::ParticleCable(Particle * a, Particle * b): ParticleLink(a, b)
{
}

bool ParticleCable::addContact(ParticleContact * contact, unsigned int limit) const
{
	float length = GetCurrentLength();

	if (length < maxLength)
	{
		return false;
	}

	contact->particles[0] = particles[0];
	contact->particles[1] = particles[1];

	Vector3 normal = particles[1]->position - particles[0]->position;
	normal.Normalize();

	contact->contactNormal = normal;

	contact->penetrationDepth = length - maxLength;
	contact->restitutionCoefficient = restitutionCoefficient;

	return true;
}

ParticleConnectingRod::ParticleConnectingRod(Particle * a, Particle * b) : ParticleLink(a, b)
{
}

bool ParticleConnectingRod::addContact(ParticleContact * contact, unsigned int limit) const
{
	float curlength = GetCurrentLength();

	if (curlength == Length)
	{
		return false;
	}

	contact->particles[0] = particles[0];
	contact->particles[1] = particles[1];

	Vector3 normal = particles[1]->position - particles[0]->position;
	normal.Normalize();

	contact->restitutionCoefficient = 0;

	if (curlength < Length)
	{
		contact->contactNormal = -1 * normal;
		contact->penetrationDepth = Length - curlength;
	}
	else 
	{
		contact->contactNormal = normal;
		contact->penetrationDepth = curlength - Length;
	}

	return true;
}

