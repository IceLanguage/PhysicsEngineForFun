#include "ParticleContact.h"

ParticleContact::ParticleContact(Particle * a, Particle * b, float restitutionCoefficient)
{
	particles[0] = a;
	particles[1] = b;
	this->restitutionCoefficient = restitutionCoefficient;
}

float ParticleContact::CalculateSeparatingVelocity() const
{
	Vector3 relativeVelocity = particles[0]->velocity - particles[1]->velocity;
	return Vector3::Dot(relativeVelocity, contactNormal);
}

void ParticleContact::ResolveVelocity(float duration)
{
	float separatingVelocity = CalculateSeparatingVelocity();

	if (separatingVelocity > 0) return;

	float newSepVelocity = - separatingVelocity * restitutionCoefficient;

	float deltaVelocity = newSepVelocity - separatingVelocity;

	float totalInverseMass = particles[0]->GetInverseMass() + particles[1]->GetInverseMass();

	if (totalInverseMass <= 0) return;

	float impulse = deltaVelocity / totalInverseMass;

	Vector3 impulsePerIMass = contactNormal * impulse;

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
