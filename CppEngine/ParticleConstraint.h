#ifndef _PhysicsEngineForFun_ParticleContact_
#define _PhysicsEngineForFun_ParticleContact__

#include "ParticleContact.h"

class ParticleConstraint :public IParticleContactGenerator
{
public:
	ParticleConstraint(Particle * a, const Vector3 &b);
	float GetCurrentLength() const;
	virtual bool AddContact(ParticleContact *contact,
		unsigned int limit) const = 0;
protected:
	Particle * particle;
	Vector3 anchorPoint;
};

class ParticleCableConstraint : public ParticleConstraint
{
public:
	ParticleCableConstraint(Particle * a, const Vector3 &b);
	virtual bool AddContact(ParticleContact *contact,
		unsigned int limit) const;
	float maxLength;
	float restitutionCoefficient;
};
#endif
