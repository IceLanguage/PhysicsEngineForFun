#include "ParticleConstraint.h"

float ParticleConstraint::GetCurrentLength() const
{
	Vector3 relativePos = particle->position - anchorPoint;
	return relativePos.Magnitude();
}

bool ParticleCableConstraint::AddContact(ParticleContact * contact, unsigned int limit) const
{
	float length = GetCurrentLength();

	if (length < maxLength)
	{
		return false;
	}

	contact->particle0 = particle;
	contact->particle1 = 0;

	Vector3 normal = anchorPoint - particle->position;
	normal.Normalize();

	contact->contactNormal = normal;

	contact->penetrationDepth = length - maxLength;
	contact->restitutionCoefficient = restitutionCoefficient;

	return true;
}
