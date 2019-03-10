#ifndef _PhysicsEngineForFun_ParticleConstraint_
#define _PhysicsEngineForFun_ParticleConstraint_

#include "ParticleContact.h"

class ParticleConstraint :public IParticleContactGenerator
{
public:
	float GetCurrentLength() const;
	virtual bool AddContact(ParticleContact *contact) const { return false; };
	Particle * particle;
	Vector3 anchorPoint;
};

class ParticleCableConstraint : public ParticleConstraint
{
public:
	virtual bool AddContact(ParticleContact *contact) const;
	float maxLength;
	float restitutionCoefficient;
};

class ParticleConnectingRodConstraint : public ParticleConstraint
{
public:
	virtual bool AddContact(ParticleContact *contact) const;
	float Length;
};
#endif
