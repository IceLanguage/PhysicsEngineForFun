#include "ParticleLink.h"

float ParticleLink::GetCurrentLength() const
{
	Vector3 relativePos = particle0->position - particle1->position;
	return relativePos.Magnitude();
}

bool ParticleCable::AddContact(ParticleContact * contact, unsigned int limit) const
{
	float length = GetCurrentLength();

	if (length < maxLength)
	{
		return false;
	}

	contact->particle0 = particle0;
	contact->particle1 = particle1;

	Vector3 normal = particle1->position - particle0->position;
	normal.Normalize();

	contact->contactNormal = normal;

	contact->penetrationDepth = length - maxLength;
	contact->restitutionCoefficient = restitutionCoefficient;

	return true;
}

bool ParticleConnectingRod::AddContact(ParticleContact * contact, unsigned int limit) const
{
	float curlength = GetCurrentLength();

	if (curlength == Length)
	{
		return false;
	}

	contact->particle0 = particle0;
	contact->particle1 = particle1;

	Vector3 normal = particle1->position - particle0->position;
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

