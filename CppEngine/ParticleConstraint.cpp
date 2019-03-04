#include "ParticleConstraint.h"

ParticleConstraint::ParticleConstraint(Particle * a, const Vector3 & b)
{
	particle = a;
	anchorPoint = b;
}

float ParticleConstraint::GetCurrentLength() const
{
	Vector3 relativePos = particle->position - anchorPoint;
	return relativePos.Magnitude();
}

ParticleCableConstraint::ParticleCableConstraint(Particle * a, const Vector3 & b):ParticleConstraint(a, b)
{
}

bool ParticleCableConstraint::AddContact(ParticleContact * contact, unsigned int limit) const
{
	float length = GetCurrentLength();

	if (length < maxLength)
	{
		return false;
	}

	contact->particles[0] = particle;
	contact->particles[1] = 0;

	Vector3 normal = anchorPoint - particle->position;
	normal.Normalize();

	contact->contactNormal = normal;

	contact->penetrationDepth = length - maxLength;
	contact->restitutionCoefficient = restitutionCoefficient;

	return true;
}
