#ifndef _PhysicsEngineForFun_ParticleLink_
#define _PhysicsEngineForFun_ParticleLink__

#include "ParticleContact.h"

class ParticleLink:public IParticleContactGenerator
{
public:
	ParticleLink(Particle * a, Particle * b);
	float GetCurrentLength() const;
	virtual bool addContact(ParticleContact *contact,
		unsigned int limit) const = 0;
protected:
	Particle * particles[2];
};

class ParticleCable: public ParticleLink
{
public:
	ParticleCable(Particle * a, Particle * b);
	virtual bool addContact(ParticleContact *contact,
		unsigned int limit) const;
	float maxLength;
	float restitutionCoefficient;
};

class ParticleConnectingRod : public ParticleLink
{
public:
	ParticleConnectingRod(Particle * a, Particle * b);
	virtual bool addContact(ParticleContact *contact,
		unsigned int limit) const;
	float Length;
};
#endif