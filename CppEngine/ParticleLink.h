#ifndef _PhysicsEngineForFun_ParticleLink_
#define _PhysicsEngineForFun_ParticleLink__

#include "ParticleContact.h"

class ParticleLink:public IParticleContactGenerator
{
public:
	float GetCurrentLength() const;
	virtual bool AddContact(ParticleContact *contact) const 
	{
		return false;
	};
	Particle * particle0, *particle1;
};

class ParticleCable: public ParticleLink
{
public:
	virtual bool AddContact(ParticleContact *contact) const;
	float maxLength;
	float restitutionCoefficient;
};

class ParticleConnectingRod : public ParticleLink
{
public:
	virtual bool AddContact(ParticleContact *contact) const;
	float Length;
};
#endif