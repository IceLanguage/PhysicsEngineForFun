#ifndef _PhysicsEngineForFun_ParticleConstraint_
#define _PhysicsEngineForFun_ParticleConstraint_

#include "ParticleContact.h"

class ParticleConstraint :public IParticleContactGenerator
{
public:
	float GetCurrentLength() const;
	virtual bool AddContact(ParticleContact *contact,
		unsigned int limit) const = 0;
	Particle * particle;
	Vector3 anchorPoint;
};

class ParticleCableConstraint : public ParticleConstraint
{
public:
	virtual bool AddContact(ParticleContact *contact,
		unsigned int limit) const;
	float maxLength;
	float restitutionCoefficient;
};
#endif
